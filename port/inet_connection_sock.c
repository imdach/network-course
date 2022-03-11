/*
 * Find an open port number for the socket.  Returns with the
 * inet_bind_hashbucket lock held.
 */
static struct inet_bind_hashbucket *
inet_csk_find_open_port(struct sock *sk, struct inet_bind_bucket **tb_ret, int *port_ret)
{
    // hinfo 是全局变量 tcp_hashinfo，通过 tcp_hashinfo 可以获得 bhash
    struct inet_hashinfo *hinfo = sk->sk_prot->h.hashinfo;
    int port = 0;
    struct inet_bind_hashbucket *head;
    struct net *net = sock_net(sk);
    int i, low, high, attempt_half;
    struct inet_bind_bucket *tb;
    u32 remaining, offset;

    attempt_half = (sk->sk_reuse == SK_CAN_REUSE) ? 1 : 0;
other_half_scan:
    inet_get_local_port_range(net, &low, &high);
    high++; /* [32768, 60999] -> [32768, 61000[ */
    remaining = high - low;
    offset = prandom_u32() % remaining;

    /* __inet_hash_connect() favors ports having @low parity
     * We do the opposite to not pollute connect() users.
     */
    offset |= 1U;

other_parity_scan:
    port = low + offset;
    for (i = 0; i < remaining; i += 2, port += 2) {
        if (inet_is_local_reserved_port(net, port))
            continue;
        // bhash 表示 inet_bind_hashbucket，key 是源端口
        // 根据端口算出 hash 值，然后根据这个值找到 bhash 中对应的slot
        head = &hinfo->bhash[inet_bhashfn(net, port, hinfo->bhash_size)];
        // 遍历 slot 指向的链表，找到 port 对应的值
        inet_bind_bucket_for_each(tb, &head->chain)
            if (net_eq(ib_net(tb), net) && tb->port == port) {
                // 判断是否占用
                if (!inet_csk_bind_conflict(sk, tb, false, false))
                    goto success;
                goto next_port;
            }
        tb = NULL;
        goto success;
next_port:
        spin_unlock_bh(&head->lock);
        cond_resched();
    }

    offset--;
    if (!(offset & 1))
        goto other_parity_scan;

    return NULL;
success:
    *port_ret = port;
    *tb_ret = tb;
    return head;
}

