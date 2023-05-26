#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/sched/signal.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/wait.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>
#include <linux/semaphore.h>

#define KFIFO_SIZE 32

static int major;
static struct cdev kfifo_cdev;
static spinlock_t kfifo_lock;
static struct semaphore kfifo_sem;
static DECLARE_WAIT_QUEUE_HEAD(kfifo_read_queue);
static DECLARE_WAIT_QUEUE_HEAD(kfifo_write_queue);
static char *kfifo_buffer;
static int kfifo_head;
static int kfifo_tail;
static int kfifo_count;

static int kfifo_open(struct inode *inode, struct file *filp)
{
    return 0;
}

static int kfifo_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static ssize_t kfifo_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    ssize_t retval = 0;
    int copied = 0;
    DECLARE_WAITQUEUE(wait, current);

    spin_lock(&kfifo_lock);
    while (kfifo_count == 0) {
        add_wait_queue(&kfifo_read_queue, &wait);
        set_current_state(TASK_INTERRUPTIBLE);
        spin_unlock(&kfifo_lock);
        if (down_interruptible(&kfifo_sem))
            return -ERESTARTSYS;
        schedule();
        spin_lock(&kfifo_lock);
        if (signal_pending(current)) {
            retval = -ERESTARTSYS;
            goto out;
        }
    }
    while (count > 0 && kfifo_count > 0) {
        copied = min_t(int, count, kfifo_count);
        if (kfifo_head > kfifo_tail) {
            copied = min_t(int, copied, kfifo_head - kfifo_tail);
            if (copy_to_user(buf, kfifo_buffer + kfifo_tail, copied)) {
                retval = -EFAULT;
                goto out;
            }
            kfifo_tail += copied;
        } else {
            int tail_len = kfifo_buffer + KFIFO_SIZE - kfifo_tail;
            copied = min_t(int, copied, tail_len);
            if (copy_to_user(buf, kfifo_buffer + kfifo_tail, copied)) {
                retval = -EFAULT;
                goto out;
            }
            kfifo_tail = (kfifo_tail + copied) % KFIFO_SIZE;
            if (copied < count && kfifo_head > 0) {
                int head_len = min_t(int, kfifo_head, count - copied);
                if (copy_to_user(buf + copied, kfifo_buffer, head_len)) {
                    retval = -EFAULT;
                    goto out;
                }
                kfifo_tail = head_len;
                copied += head_len;
            }
        }
        kfifo_count -= copied;
        count -= copied;
        buf += copied;
    }
    retval
