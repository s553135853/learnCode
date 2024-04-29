#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/init.h>

static struct platform_device *pl_hello_device;

static int __init hello_init(void)
{
    int ret;

    // 创建和注册平台设备
    pl_hello_device = platform_device_register_simple("pl_hello_driver", -1, NULL, 0);
    if (IS_ERR(pl_hello_device)) {
        pr_err("Failed to register platform device\n");
        return PTR_ERR(pl_hello_device);
    }

    pr_info("Platform device registered\n");
    return 0;
}

static void __exit hello_exit(void)
{
    // 注销平台设备
    platform_device_unregister(pl_hello_device);
    pr_info("Platform device unregistered\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");