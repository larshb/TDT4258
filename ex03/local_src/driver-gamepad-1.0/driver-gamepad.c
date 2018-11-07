#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>
#include "efm32gg.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group 31");
MODULE_DESCRIPTION("Gamepad char device driver.");
MODULE_VERSION("1.0");

#define DEVICE_NAME "gamepad"

static int open(struct inode*, struct file*);
static int release(struct inode*, struct file*);
static ssize_t read(struct file *filp, char __user*, size_t, loff_t*);
static ssize_t write(struct file *filp, const char __user*, size_t, loff_t*);

static struct file_operations fops = {
        owner: THIS_MODULE,
        read: read,
        write: write,
        open: open,
        release: release
};

static int major;
struct cdev cdev;   /* Pointer to operation structure */
struct class *c1;   /* Device class */
dev_t devno = 1;    /* Device number */

#define req_reg(reg, regstr, errno) \
    if (!request_mem_region((unsigned long)reg, 1, DEVICE_NAME)) { \
        printk(KERN_ALERT "Unable to request %s memory region!\n", regstr); \
        return errno; \
    }

static int __init init(void) {
    printk("Loading module: %s \n", DEVICE_NAME);

    major = register_chrdev(0, DEVICE_NAME, &fops);

    /* Allocate device number */
    if(alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME)) {
        printk(KERN_ALERT "Device number allocation failed.\n");
        return 1;
    }

    /* Request memory region*/
    req_reg(GPIO_PC_MODEL, "lower GPIO PC", 2); // buttons
    req_reg(GPIO_PC_DIN,   "GPIO PC data input", 3);
    req_reg(GPIO_PA_MODEH, "higher GPIO PC", 4); // leds
    req_reg(GPIO_PA_DOUT,  "GPIO PA data input", 5);

    /*Initialize cdev*/
    cdev_init(&cdev, &fops);

    /* Make driver appear as a file in the /dev directory */
    c1 = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(c1, NULL, devno, NULL, DEVICE_NAME);

    /* Configure registers */

    /* Read buttons */
    iowrite32(0x33333333, GPIO_PC_MODEL);
    iowrite32(0xFF, GPIO_PC_DOUT);

    /* DEBUG Turn on all LEDs (only D5..7 works this way) */
    iowrite32(2, GPIO_PA_CTRL);
    iowrite32(0x55555555, GPIO_PA_MODEH);
    iowrite32(0x0000, GPIO_PA_DOUT);
    
    return 0;
}

static void __exit cleanup(void) {
    printk("Module stopped being used\n");
    unregister_chrdev(major, DEVICE_NAME);
}

/* File operations */

static int open(struct inode *inode, struct file *filp){
    printk("OPEN\n");
    return 0;
}

static int release(struct inode* inode, struct file* file){
    printk("RELEASE\n");
    return 0;
}

static ssize_t read(struct file *filp, char __user *buff, size_t count, loff_t *offp){
    uint32_t data = ioread32(GPIO_PC_DIN);
    copy_to_user(buff, &data, 1);
    printk(KERN_INFO "Read %u from buttons\n", data);
    return (ssize_t)1;
}

static ssize_t write(struct file *filp, const char __user *buff, size_t count, loff_t *offp){
    return (ssize_t)0;
}

module_init(init);
module_exit(cleanup);
