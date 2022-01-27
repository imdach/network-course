#include <linux/debugfs.h>
#include <linux/delay.h> /* usleep_range */
#include <linux/errno.h> /* EFAULT */
#include <linux/fs.h>
#include <linux/kernel.h> /* min */
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/poll.h>
#include <linux/printk.h> /* printk */
#include <linux/uaccess.h> /* copy_from_user, copy_to_user */
#include <linux/wait.h> /* wait_queue_head_t, wait_event_interruptible, wake_up_interruptible  */
#include <uapi/linux/stat.h> /* S_IRUSR */
#include <linux/device.h>


#define DEVICE_NAME "mypolldev"
int major_num;
static struct class *dev_class;

static char readbuf[1024];
static size_t readbuflen;
static struct task_struct *thread;
static wait_queue_head_t waitqueue;

static ssize_t read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    ssize_t ret;
    if (copy_to_user(buf, readbuf, readbuflen)) {
        ret = -EFAULT;
    } else {
        ret = readbuflen;
    }
    readbuflen = 0;
    return ret;
}

unsigned int my_dev_poll(struct file *filp, struct poll_table_struct *wait)
{
    pr_info("poll\n");
    poll_wait(filp, &waitqueue, wait);
    if (readbuflen) {
        pr_info("return POLLIN\n");
        return POLLIN;
    } else {
        pr_info("return 0\n");
        return 0;
    }
}

static int kthread_func(void *data)
{
    int counter = 0;
    while (!kthread_should_stop()) {
        ++counter;
        readbuflen = snprintf(
                readbuf,
                sizeof(readbuf),
                "hello, %i",
                counter
        );
        usleep_range(1000000, 1000001);
        pr_info("wake_up\n");
        wake_up(&waitqueue);
    }
    return 0;
}

static const struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = read,
        .poll = my_dev_poll
};

static int myinit(void)
{
    // 注册 char device
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    dev_class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(dev_class, NULL, MKDEV(major_num, 0), NULL, DEVICE_NAME);
    // 初始化 wait queue
    init_waitqueue_head(&waitqueue);
    // 启动一个内核线程
    thread = kthread_create(kthread_func, NULL, "mythread");
    wake_up_process(thread);
    return 0;
}

static void myexit(void)
{
    device_destroy(dev_class, MKDEV(major_num, 0));
    class_destroy(dev_class);
    unregister_chrdev(major_num, DEVICE_NAME);

    kthread_stop(thread);
}

module_init(myinit)
module_exit(myexit)
MODULE_LICENSE("GPL");