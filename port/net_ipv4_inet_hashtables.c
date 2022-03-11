int __inet_hash_connect(struct inet_timewait_death_row *death_row,
                        struct sock *sk, u32 port_offset,
                        int (*check_established)(struct inet_timewait_death_row *,
                                                 struct sock *, __u16, struct inet_timewait_sock **)) {
    int port = inet_sk(sk)->inet_num;
    struct net *net = sock_net(sk);
    struct inet_bind_bucket *tb;
    u32 remaining, offset;

    // int low;  // 临时端口号的下界
    // int high; // 临时端口号的上界
    int i, low, high;
    // hint 值为一个静态变量，表示可能可用的端口号，其记录了上一次选择的端口号加上2的结果值
    static u32 hint;

    // 读取 /proc/sys/net/ipv4/ip_local_port_range 的临时端号的上界和下界
    inet_get_local_port_range(net, &low, &high);
    high++; /* [32768, 60999] -> [32768, 61000[ */
    // remaining 是临时端口号可分配值的范围
    remaining = high - low;
    remaining &= ~1U;

    offset = (hint + port_offset) % remaining;
    /* In first pass we try ports of @low parity.
     * inet_csk_get_port() does the opposite choice.
     */
    offset &= ~1U; // 将最后一位置为 0，也就是将 offset 变为偶数
    port = low + offset; // port 与 low 的奇偶性保持一致，low 一般是偶数
    // 从 0 开始遍历，查找未被占用的端口号
    for (i = 0; i < remaining; i += 2, port += 2) {
        // 检查端口号是否属于保留端口号
        if (inet_is_local_reserved_port(net, port))
            continue;
        head = &hinfo->bhash[inet_bhashfn(net, port,
                                          hinfo->bhash_size)];
        inet_bind_bucket_for_each(tb, &head->chain) {
            // 检查 port 是否可用
            if (!check_established(death_row, sk, port, &tw))
                goto ok;
        }
    }
    ok:
    hint += i + 2;
}

/*
 * Bind a port for a connect operation and hash it.
 */
int inet_hash_connect(struct inet_timewait_death_row *death_row,
                      struct sock *sk) {
    u32 port_offset = 0;

    port_offset = inet_sk_port_offset(sk);
    return __inet_hash_connect(death_row, sk, port_offset, __inet_check_established);
}

static u32 inet_sk_port_offset(const struct sock *sk)
{
    const struct inet_sock *inet = inet_sk(sk);

    return secure_ipv4_port_ephemeral(inet->inet_rcv_saddr,
                                      inet->inet_daddr,
                                      inet->inet_dport);
}

u32 secure_ipv4_port_ephemeral(__be32 saddr, __be32 daddr, __be16 dport)
{
    // net_secret是个全局变量，其中的数据由net_secret_init函数来随机填充, 多次调用只会生成一次
    net_secret_init();
    // hash(源地址,目标地址,目标端口、随机数)
    return siphash_3u32((__force u32)saddr, (__force u32)daddr,
            (__force u16)dport, &net_secret);
}
