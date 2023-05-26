#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "my_device" // 设备名称
#define SUCCESS 0
#define BUF_LEN 80

MODULE_LICENSE("GPL"); // 开源协议

static int Major; // 主设备号
static char msg[BUF_LEN]; // 存储消息的缓冲区
static char *msg_ptr;

// 函数声明
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static long device_ioctl(struct file *, unsigned int, unsigned long);

// 设备操作函数集合
static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .unlocked_ioctl = device_ioctl,
    .open = device_open,
    .release = device_release
};

// 初始化函数
int init_module(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", Major);
        return Major;
    }

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");

    return SUCCESS;
}

// 清理函数
void cleanup_module(void)
{
    unregister_chrdev(Major, DEVICE_NAME);
}

// 设备打开函数
static int device_open(struct inode *inode, struct file *file)
{
    msg_ptr = msg;
    try_module_get(THIS_MODULE);

    return SUCCESS;
}

// 设备关闭函数
static int device_release(struct inode *inode, struct file *file)
{
    module_put(THIS_MODULE);

    return SUCCESS;
}

// 设备读取函数
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    int bytes_read = 0;

    while (length && *msg_ptr) {
        put_user(*(msg_ptr++), buffer++);
        length--;
        bytes_read++;
    }

    return bytes_read;
}

// 设备写入函数
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
    int bytes_written = 0;

    while (len && (*msg_ptr = get_user(*buff))) {
        msg_ptr++;
        buff++;
        len--;
        bytes_written++;
    }

    return bytes_written;
}

// 设备IOCTL函数
static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int x, y, op;
    switch (cmd) {
    case 1: // 求和
        if (copy_from_user(&x, (int *)arg, sizeof(int)))
            return -EFAULT;
        if (copy_from_user(&y, (int *)(arg + sizeof(int)), sizeof(int)))
            return -EFAULT;
        op = x + y;
        if (copy_to_user((int *)arg, &op, sizeof(int)))
            return -EFAULT;
        break;

    case 2: // 求差
        if (copy_from_user(&x, (int *)arg, sizeof(int)))
            return -EFAULT;
        if (copy_from_user(&y, (int *)(arg + sizeof(int)), sizeof(int)))
            return -EFAULT;
        op = x - y;
        if (copy_to_user((int *)arg, &op, sizeof(int)))
            return -EFAULT;
        break;

    case 3: // 求最大值
        if (copy_from_user(&x, (int *)arg, sizeof(int)))
            return -EFAULT;
        if (copy_from_user(&y, (int *)(arg + sizeof(int)), sizeof(int)))
            return -EFAULT;
        op = x > y ? x : y;
        if (copy_to_user((int *)arg, &op, sizeof(int)))
            return -EFAULT;
        break;

    default: // 非法操作码
        return -EINVAL;
}

return SUCCESS;
}
