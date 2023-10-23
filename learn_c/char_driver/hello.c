#include <linux/kernel.h> /*Needed by all modules*/
#include <linux/module.h> /*Needed for KERN_* */
#include <linux/init.h> /* Needed for the macros */
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

MODULE_LICENSE("GPL");


#define CLASS_NAME "my_char_class"

static int year=2014;


static dev_t my_devno;

struct class* my_class = NULL;
struct device* my_device = NULL;
static int open_hello(struct inode *, struct file *)
{
    printk("------------open_hello------------\n");
    return 0;
}


static ssize_t write_hello(struct file *, const char __user *, size_t, loff_t *)
{
    printk("------------write_hello------------\n");
    return 0;
}




static ssize_t read_hello(struct file *, char __user *, size_t, loff_t *)
{
    printk("------------read_hello------------\n");
    return 0;
}







static struct file_operations my_file = 
{
    .open = &open_hello,
    .write = &write_hello,
    .read  = &read_hello,

};

struct cdev my_cdev;

static int hello_init(void)
{
  // printk(KERN_WARNING "Hello kernel, it's %d!\n",year);
  int rc = -1;
  
  
  rc = alloc_chrdev_region(&my_devno,0,1,"test");
  if(rc < 0)
  {
      printk("apply dev number failed\n");
      goto fail_destroy;
  }
  printk("MAJOR is %d\n", MAJOR(my_devno));
  printk("MINOR is %d\n", MINOR(my_devno));

  cdev_init(&my_cdev,&my_file);

  rc = cdev_add(&my_cdev,my_devno,1);
  if(rc < 0)
  {
      printk("add my_cdev  failed\n");
      goto fail_destroy;
  } 
  my_class = class_create(THIS_MODULE, CLASS_NAME);
  my_device = device_create(my_class, NULL, my_devno, NULL, "test_node");
    

  return 0;
fail_destroy:
  return -1;

  
}


static void hello_exit(void)
{
  printk("Bye, kernel!\n");
  cdev_del(&my_cdev);
  unregister_chrdev_region(my_devno,1);
  device_destroy(my_class, my_devno);
  class_destroy(my_class);

}

/* main module function*/
module_init(hello_init);
module_exit(hello_exit);