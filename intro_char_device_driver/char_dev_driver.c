
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>


static int __init ldd_module_init(void){
    printk(KERN_INFO "initializing of kernel module\n");
    return 0;
}

static void __exit ldd_module_exit(void){
    printk(KERN_INFO "exit kernel module\n");
} 


module_init(ldd_module_init); 
module_exit(ldd_module_exit); 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("vijay -<vijaytarunkoppadi125@gmail.com>");
MODULE_DESCRIPTION("Character device driver");
MODULE_VERSION("1.0");