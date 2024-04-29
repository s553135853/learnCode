#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kernel.h> /*Needed by all modules*/
#include <linux/module.h> /*Needed for KERN_* */
#include <linux/platform_device.h>
#include <linux/string.h>
#include <linux/err.h>

enum LOG_LEVEL {
    INFO,
    DEBUG
};

#define PLAT_LOG(level, formt, ...)                                  \
    do {                                                             \
        \    
if(level == INFO)                                                    \
        {                                                            \
            printk("[----platform-----]" formt "\n", ##__VA_ARGS__); \
        \    
}                                                        \
    } while (0)

struct pl_hello_plat_t {
    struct platform_device *pdev;
    int gpio_num;
};

static ssize_t plm_switch_show(struct device *dev, struct device_attribute *attr,
                              char *buf)
{
    return snprintf(buf, PAGE_SIZE, "%d", 0);
}

static ssize_t plm_switch_store(struct device *dev, struct device_attribute *attr,
                               const char *buf, size_t len)
{
    struct pl_hello_plat_t *pl_p = dev_get_drvdata(dev);
    u8 _data;
    kstrtou8(buf, 10, &_data);
    PLAT_LOG(INFO, "data:%d", _data);
    return len;
}

static struct device_attribute dev_attr_plm_switch = {
    .attr = {
        .name = __stringify(plm_switch),
        .mode = S_IWUSR | S_IWGRP | S_IWOTH | S_IROTH},
    .store = plm_switch_store,
    .show = plm_switch_show,
};
struct platform_device *m_pdev;

static int pl_hello_driver_probe(struct platform_device *pdev)
{
    struct pl_hello_plat_t *pl_p = devm_kzalloc(&pdev->dev, sizeof(struct pl_hello_plat_t), GFP_KERNEL);
    int ret;
    PLAT_LOG(INFO, "pl_hello_driver_probe");
    if (IS_ERR(pl_p)) {
        PLAT_LOG(INFO, "alloc pl_hello_plat_t failed");
    }

    pl_p->pdev = pdev;
    m_pdev = pdev;
    ret = device_create_file(&pdev->dev, &dev_attr_plm_switch);
    if (ret) {
        printk(KERN_ERR "Cannot create sysfs file......\n");
        return -1;
    }
    platform_set_drvdata(pdev, pl_p);
    
    return 0;
}

static struct platform_driver pl_hello_driver = {
    .driver = {
        .name = "pl_hello_driver",
    },
    .probe = pl_hello_driver_probe,
};

static int __init pl_hello_driver_init(void)
{
	int ret;
	ret = platform_driver_register(&pl_hello_driver);
	return ret;
}
module_init(pl_hello_driver_init);

static void __exit pl_hello_driver_exit(void)
{
    PLAT_LOG(INFO, "pl_hello_driver_exit");
	platform_driver_unregister(&pl_hello_driver);
    platform_device_del(m_pdev);
}
module_exit(pl_hello_driver_exit);

MODULE_LICENSE("GPL");