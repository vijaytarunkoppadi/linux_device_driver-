
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

//dev_t dev = MKDEV(235,0);
dev_t dev = 0;

static int __init ldd_module_init(void){
    //register_chrdev_region(dev,1,"chrdev");
    if((alloc_chrdev_region(&dev,0,1,"chrdev")) < 0){
        printk(KERN_INFO "cannot allocate major number for device 1\n");
    }
    printk(KERN_INFO"Major =%d Minor =%d\n",MAJOR(dev),MINOR(dev));
    printk(KERN_INFO "initializing of kernel module\n");
    return 0;
}

static void __exit ldd_module_exit(void){
    unregister_chrdev_region(dev,1); 
    printk(KERN_INFO "exit kernel module\n");
} 


module_init(ldd_module_init); 
module_exit(ldd_module_exit); 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("vijay -<vijaytarunkoppadi125@gmail.com>");
MODULE_DESCRIPTION("Character device driver --> registering driver major and minor number");
MODULE_VERSION("2.0");