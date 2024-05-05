#include <asm/atomic.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h> /*Needed for KERN_* */
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>



#define CLASS_NAME "mics_class"




enum LOG_LEVEL {
    INFO,
    DEBUG
};

#define CDEV_LOG(level, formt, ...)                              \
    do {                                                         \
        \    
if(level >= INFO)                                                \
        {                                                        \
            printk("[----mics-----]" formt "\n", ##__VA_ARGS__); \
        \    
}                                                    \
    } while (0)

struct misc_cdev_t {
    struct cdev misc_cdev;
    dev_t misc_devno;
    struct class *misc_class;
    struct device *misc_dev;
    atomic_t misc_count;
    struct timer_list misc_timer;
    struct delayed_work misc_work;
};

#define to_misc_dev(dev)  container_of(dev, struct misc_cdev_t, misc_timer)
#define to_delayed_work(dev)  container_of(dev, struct delayed_work, work)
#define to_misc_dev_work(dev)  container_of(dev, struct misc_cdev_t, misc_work)



static void timer_handle(struct timer_list *tl);



static struct misc_cdev_t *m_cdev;

static void misc_work_func(struct work_struct *work)
{
    struct delayed_work *misc_delayed_work = to_delayed_work(work);
    struct misc_cdev_t *mcdev = to_misc_dev_work(misc_delayed_work);

    atomic_inc(&mcdev->misc_count);
    schedule_delayed_work(&mcdev->misc_work, msecs_to_jiffies(500));
    CDEV_LOG(INFO, "misc_work_func count:%d", mcdev->misc_count.counter);
}

int misc_open(struct inode *node, struct file *file)
{
    CDEV_LOG(INFO, "misc_open");
    file->private_data = m_cdev;

    //启动定时器
    #if 0
    add_timer(&m_cdev->misc_timer);
    atomic_set(&m_cdev->misc_count, 0);
    #endif
    schedule_delayed_work(&m_cdev->misc_work, msecs_to_jiffies(500));   
    return 0;
}

/*

int (*release) (struct inode *, struct file *);
ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);

*/

ssize_t misc_read(struct file *file, char __user *buf, size_t size, loff_t *lft)
{
    struct misc_cdev_t *mcdev = file->private_data;
    int rc;
    int count;
    count = atomic_read(&mcdev->misc_count);
    rc = copy_to_user(buf, &count, sizeof(int));
    if (rc < 0) {
        CDEV_LOG(DEBUG, "copy_to_user data failed");
    }

    return sizeof(int);
}

int misc_release(struct inode *node, struct file *file)
{
    struct misc_cdev_t *mcdev = file->private_data;
    return cancel_delayed_work(&mcdev->misc_work);
    // return del_timer(&mcdev->misc_timer);
}

static void timer_handle(struct timer_list *tl)
{
    struct misc_cdev_t *mdcev = to_misc_dev(tl);
    mod_timer(&mdcev->misc_timer, jiffies + HZ);
    atomic_inc(&mdcev->misc_count);
    CDEV_LOG(INFO, "timer_handle count:%d", mdcev->misc_count.counter);
}

static struct file_operations misc_fp = {
    .open = misc_open,
    .read = misc_read,
    .release = misc_release};

static int mics_init(void)
{
    CDEV_LOG(INFO, "mics_init");
    int rc;
    m_cdev = (struct misc_cdev_t *)kmalloc(sizeof(struct misc_cdev_t), GFP_KERNEL);
    if (IS_ERR_OR_NULL(m_cdev)) {
        CDEV_LOG(DEBUG, "alloc misc_cdev_t failed");
    }

    rc = alloc_chrdev_region(&m_cdev->misc_devno, 0, 1, "misc_test");
    if (rc < 0) {
        CDEV_LOG(DEBUG, "aalloc_chrdev_region misc_test failed");
    }

    cdev_init(&m_cdev->misc_cdev, &misc_fp);
    rc = cdev_add(&m_cdev->misc_cdev, m_cdev->misc_devno, 1);
    if (rc < 0) {
        CDEV_LOG(DEBUG, "cdev_add misc_test failed");
    }

    //定时器初始化
    #if 0
    m_cdev->misc_timer.expires = jiffies + HZ;
    timer_setup(&m_cdev->misc_timer, timer_handle, 0);
    #endif

    INIT_DELAYED_WORK(&m_cdev->misc_work, misc_work_func);

    m_cdev->misc_class = class_create(THIS_MODULE, CLASS_NAME);
    m_cdev->misc_dev = device_create(m_cdev->misc_class, NULL, m_cdev->misc_devno, NULL, "misc_test");

    return 0;
}

static void mics_exit(void)
{
    CDEV_LOG(INFO, "mics_exit");
    cdev_del(&m_cdev->misc_cdev);
    unregister_chrdev_region(m_cdev->misc_devno, 1);
    device_destroy(m_cdev->misc_class, m_cdev->misc_devno);
    class_destroy(m_cdev->misc_class);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("shenglin");

module_init(mics_init);
module_exit(mics_exit);