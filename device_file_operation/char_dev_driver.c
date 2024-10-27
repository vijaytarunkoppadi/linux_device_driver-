
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>


#define BUFFER_SIZE 1024

//dev_t dev = MKDEV(235,0);
dev_t dev = 0;
static struct class *cl;
static struct cdev c_dev;
static char device_buffer[BUFFER_SIZE];



static int my_open(struct inode *inode,struct file *file){
    printk("Driver open function called...\n");
    return 0;
}

static int my_release(struct inode *inode,struct file *file){
    printk("Driver close function called...\n");
    return 0;
}

static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
   size_t bytes_to_copy = BUFFER_SIZE - *off < len ? BUFFER_SIZE - *off : len;

    if (bytes_to_copy == 0)
        return 0; // End of file reached

    if (copy_to_user(buf, device_buffer + *off, bytes_to_copy) != 0)
        return -EFAULT;

    *off += bytes_to_copy;
    pr_info("Read %zu bytes from device : %s\n", bytes_to_copy,device_buffer-*off);
     printk("Driver read function called...\n");
    return bytes_to_copy;
}

static ssize_t my_write(struct file *filp, const char *buf, size_t len, loff_t * off){
    size_t bytes_to_copy = BUFFER_SIZE - *off < len ? BUFFER_SIZE - *off : len;

    if (bytes_to_copy == 0)
        return -ENOMEM; // No space left on device

    if (copy_from_user(device_buffer + *off, buf, bytes_to_copy) != 0)
        return -EFAULT;

    *off += bytes_to_copy;
    pr_info("Wrote %zu bytes to device\n", bytes_to_copy);
     printk("Driver write function called...\n");
    return bytes_to_copy;
}

static struct file_operations fops={
    .open    = my_open,
    .read    = my_read,
    .write   = my_write,
    .release = my_release,
};

static int __init ldd_module_init(void){
    //register_chrdev_region(dev,1,"chrdev");
    if((alloc_chrdev_region(&dev,0,1,"chrdev")) < 0){
        printk(KERN_ERR "cannot allocate major number for device 1\n");
        return -1;
    }
    cdev_init(&c_dev,&fops);
    if((cdev_add(&c_dev,dev,1))<0){
        printk(KERN_ERR "cannot add the device to the system\n");
        unregister_chrdev_region(dev,1);
        return -1;
    }
    cl=class_create("char");
    if(IS_ERR(cl)){
        printk(KERN_ERR "cannot create the struct class for device\n");
        unregister_chrdev_region(dev,1);
        return -1;
    }
    if(IS_ERR(device_create(cl,NULL,dev,NULL,"mychar%d",0))){
        printk(KERN_ERR "cannot create the device\n");
        class_destroy(cl);
        return -1;
    }
    printk(KERN_INFO "Major =%d Minor =%d\n",MAJOR(dev),MINOR(dev));
    printk(KERN_INFO "initializing of kernel module\n");
    return 0;
}

static void __exit ldd_module_exit(void){
    device_destroy(cl,dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev,1); 
    printk(KERN_INFO "exit kernel module\n");
} 


module_init(ldd_module_init); 
module_exit(ldd_module_exit); 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("vijay -<vijaytarunkoppadi125@gmail.com>");
MODULE_DESCRIPTION("Character device driver --> registering driver major and minor number");
MODULE_VERSION("3.0");
