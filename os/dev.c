#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <linux/kfifo.h>

#define BUFFER_SIZE 32

struct kfifo {
    char *buffer;
    int in;
    int out;
    int size;
    int count;
    wait_queue_head_t read_queue;
    wait_queue_head_t write_queue;
};

static struct kfifo *fifo;

static ssize_t kfifo_write(struct file *file, const char __user *buf, size_t len, loff_t *pos);

static int kfifo_open(struct inode *inode, struct file *file) {
    return 0;
}

static int kfifo_release(struct inode *inode, struct file *file) {
    return 0;
}

static ssize_t kfifo_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
    ssize_t ret = 0;
    while (len && fifo->count > 0) {  // 从缓冲区中读取数据
        put_user(fifo->buffer[fifo->out], buf++);
        fifo->out = (fifo->out + 1) % fifo->size;
        fifo->count--;
        len--;
        ret++;
    }
    wake_up_interruptible(&fifo->write_queue);  // 唤醒等待写操作的进程
    return ret;
}

static ssize_t kfifo_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    ssize_t ret = 0;
    while (len) {
        if (fifo->count >= fifo->size) {  // 没有足够的空位可写，进入睡眠状态
            if (wait_event_interruptible(fifo->write_queue, fifo->count < fifo->size)) {
                return -ERESTARTSYS;
            }
        }
        fifo->buffer[fifo->in] = *buf++;
        fifo->in = (fifo->in + 1) % fifo->size;
        fifo->count++;
        len--;
        ret++;
    }
    wake_up_interruptible(&fifo->read_queue);  // 唤醒等待读操作的进程
    return ret;
}

static const struct file_operations kfifo_fops = {
    .owner = THIS_MODULE,
    .open = kfifo_open,
    .release = kfifo_release,
    .read = kfifo_read,
    .write = kfifo_write,
};

static dev_t devno;
static struct cdev cdev;

static int __init kfifo_init(void) {
    int err;
    devno = MKDEV(0, 0);
    err = alloc_chrdev_region(&devno, 0, 1, "kfifo");
    if (err < 0) {
        printk(KERN_ERR "alloc_chrdev_region failed\n");
        return err;
    }
    cdev_init(&cdev, &kfifo_fops);
    cdev.owner = THIS_MODULE;
    err = cdev_add(&cdev, devno, 1);
    if (err < 0) {
        printk(KERN_ERR "cdev_add failed\n");
unregister_chrdev_region(devno, 1);
return err;
}
}

// 初始化FIFO缓冲区
fifo = kmalloc(sizeof(struct kfifo), GFP_KERNEL);
if (!fifo) {
    cdev_del(&cdev);
    unregister_chrdev_region(devno, 1);
    return -ENOMEM;
}
fifo->buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
if (!fifo->buffer) {
    kfree(fifo);
    cdev_del(&cdev);
    unregister_chrdev_region(devno, 1);
    return -ENOMEM;
}
fifo->in = 0;
fifo->out = 0;
fifo->size = BUFFER_SIZE;
fifo->count = 0;
init_waitqueue_head(&fifo->read_queue);
init_waitqueue_head(&fifo->write_queue);

printk(KERN_INFO "kfifo module loaded\n");
return 0;

_ERR "cdev_add failed\n");
unregister_chrdev_region(devno, 1);
return err;
}

// 初始化FIFO缓冲区
fifo = kmalloc(sizeof(struct kfifo), GFP_KERNEL);
if (!fifo) {
    cdev_del(&cdev);
    unregister_chrdev_region(devno, 1);
    return -ENOMEM;
}
fifo->buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
if (!fifo->buffer) {
    kfree(fifo);
    cdev_del(&cdev);
    unregister_chrdev_region(devno, 1);
    return -ENOMEM;
}
fifo->in = 0;
fifo->out = 0;
fifo->size = BUFFER_SIZE;
fifo->count = 0;
init_waitqueue_head(&fifo->read_queue);
init_waitqueue_head(&fifo->write_queue);

printk(KERN_INFO "kfifo module loaded\n");
return 0;
}

static void __exit kfifo_exit(void) {
kfree(fifo->buffer);
kfree(fifo);
cdev_del(&cdev);
unregister_chrdev_region(devno, 1);
printk(KERN_INFO "kfifo module unloaded\n");
}

module_init(kfifo_init);
module_exit(kfifo_exit);

MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A kernel FIFO buffer module");
MODULE_LICENSE("GPL");
