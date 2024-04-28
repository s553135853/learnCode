#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h> /* Needed for the macros */
#include <linux/ioctl.h>
#include <linux/kernel.h> /*Needed by all modules*/
#include <linux/module.h> /*Needed for KERN_* */
#include <linux/rwlock.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");

#define CLASS_NAME "char_my_class"
#define MY_DEVICE_MAGIC 'k'
#define MY_IOCTL_SET _IOW(MY_DEVICE_MAGIC, 1, int)
#define MY_IOCTL_GET _IOR(MY_DEVICE_MAGIC, 2, int)

enum LOG_LEVEL {
    INFO,
    DEBUG
};

#define CDEV_LOG(level, formt, ...)                            \
    do {                                                       \
        \    
if(level == INFO)                                              \
        {                                                      \
            printk("[----cdev-----]" formt "\n", ##__VA_ARGS__); \
        \    
}                                                  \
    } while (0)

struct p_cdev_t {
    struct cdev my_cdev;
    dev_t my_devno;
    struct class *my_class;
    struct device *dev;
    int num_data;
    rwlock_t lock;
};

static struct p_cdev_t *pdev;

static long hello_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct p_cdev_t *pv = (struct p_cdev_t *)file->private_data;
    void __user *p = (void __user *)arg;
    u8 buf_data[10];
    int ret;
    switch (cmd) {
    case MY_IOCTL_SET:
        ret = copy_from_user(buf_data, p, sizeof(int));
        memcpy(&pv->num_data, buf_data, sizeof(int));
        break;
    case MY_IOCTL_GET:
        ret = copy_to_user(p, &pv->num_data, sizeof(int));
        CDEV_LOG(INFO, "MY_IOCTL_GET:%d", pv->num_data);
        break;
    default:
        break;
    }

    return ret;
}

static int open_hello(struct inode *inode, struct file *file)
{
    CDEV_LOG(INFO, "------------open_hello------------");
    if (file->private_data == NULL) {
        file->private_data = pdev;
    }
    return 0;
}

static ssize_t write_hello(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    struct p_cdev_t *pv = (struct p_cdev_t *)file->private_data;
    u8 buf_data[32];
    CDEV_LOG(INFO, "------------write_hello------------");
    write_lock(&pv->lock);
    if (copy_from_user(buf_data, buf, sizeof(int)) < 0) {
        CDEV_LOG(INFO, "------------copy_from_user failed------------");
        write_unlock(&pv->lock);
        return 0;
    }
    memcpy(&pv->num_data, buf_data, sizeof(int));

    write_unlock(&pv->lock);
    return 0;
}

static ssize_t read_hello(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    struct p_cdev_t *pv = (struct p_cdev_t *)file->private_data;
    CDEV_LOG(INFO, "------------read_hello------------");
    read_lock(&pv->lock);
    if (copy_to_user(buf, &pv->num_data, sizeof(int)) < 0) {
        CDEV_LOG(INFO, "------------copy_to_user failed------------");
        read_unlock(&pv->lock);
        return 0;
    }
    read_unlock(&pv->lock);
    return sizeof(int);
}

static struct file_operations my_file =
    {
        .open = &open_hello,
        .write = &write_hello,
        .read = &read_hello,
        .unlocked_ioctl = &hello_ioctl
    };

static int hello_init(void)
{
    // CDEV_LOG(INFO,KERN_WARNING "Hello kernel, it's %d!",year);
    int rc = -1;

    pdev = kzalloc(sizeof(struct p_cdev_t), GFP_KERNEL);
    if (pdev == NULL) {
        CDEV_LOG(INFO, "alloc pdev failed");
    }

    rc = alloc_chrdev_region(&pdev->my_devno, 0, 1, "test");
    if (rc < 0) {
        CDEV_LOG(INFO, "apply dev number failed");
        goto fail_destroy;
    }

    cdev_init(&pdev->my_cdev, &my_file);
    rc = cdev_add(&pdev->my_cdev, pdev->my_devno, 1);
    if (rc < 0) {
        CDEV_LOG(INFO, "add my_cdev  failed");
        goto fail_destroy;
    }

    rwlock_init(&pdev->lock);
    pdev->my_class = class_create(THIS_MODULE, CLASS_NAME);
    pdev->dev = device_create(pdev->my_class, NULL, pdev->my_devno, NULL, "test_node");
    pdev->num_data = 300;

    return 0;
fail_destroy:
    return -1;
}

static void hello_exit(void)
{
    CDEV_LOG(INFO, "Bye, kernel!");
    cdev_del(&pdev->my_cdev);
    unregister_chrdev_region(pdev->my_devno, 1);
    device_destroy(pdev->my_class, pdev->my_devno);
    class_destroy(pdev->my_class);
}

/* main module function*/
module_init(hello_init);
module_exit(hello_exit);