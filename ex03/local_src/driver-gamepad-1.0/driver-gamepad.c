//
// Created by didrik on 31.10.18.
//
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
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>
#include <linux/interrupt.h>
#include "efm32gg.h"


#define DEVICE_NAME "gamepad"
/* User program opens the driver */
static int open(struct inode *inode, struct file *filp);
/* User program closes the driver */
static int release(struct inode *inode);
/*User program reads from the driver */
static ssize_t read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
/*User program writes to the driver */
static ssize_t write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);
/* File operations*/
static struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = read,
        .write = write,
        .open = open,
        .release = release
};
struct cdev cdev;     /* Pointer to operation structure */
struct class *c1;   /* Device class */
dev_t devno = 1;        /* Device number */
/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */
static int __init
init(void) {
    printk("Loading module: %s \n", DEVICE_NAME);

    /* Allocate device number */
    if(alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME)) {
        printk(KERN_ALERT "Device number allocation failed.\n");
        return 1;
    }

    /* Allocate memory map */
    if (!request_mem_region(GPIO_PC_MODEL, 1, DEVICE_NAME)) {
        printk(KERN_ALERT "Unable to request GPIO_PC_MODEL memory region!\n");
        return 2;
    }
    if (!request_mem_region(GPIO_PC_DOUT, 1, DEVICE_NAME)) {
        printk(KERN_ALERT "Unable to request GPIO_PC_DOUT memory region!\n");
        return 3;
    }
    if (!request_mem_region(GPIO_PC_DIN, 1, DEVICE_NAME)) {
        printk(KERN_ALERT "Unable to request GPIO_PC_DIN memory region!\n");
        return 4;
    }

    /* Request memory region*/
    //request_mem_region();
    /*Initialize cdev*/
    cdev_init(&cdev, &fops);
    /* Make driver appear as a file in the /dev directory */
    c1 = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(c1, NULL, devno, NULL, DEVICE_NAME);
    iowrite32(2, GPIO_PA_CTRL);
    iowrite32(0x55555555, GPIO_PA_MODEH);
    iowrite32(0x0000, GPIO_PA_DOUT);
    return 0;
}
/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */
static void __exit
cleanup(void) {
    printk("Module stopped being used\n");
}

/* File operations */

static int open(struct inode *inode, struct file *filp){
    printk("OPEN\n");
    return 0;
}

static int release(struct inode *inode){
    printk("RELEASE\n");
    return 0;
}

static ssize_t read(struct file *filp, char __user *buff, size_t count, loff_t *offp){
    printk("READ\n");
    return (ssize_t)0;
}

static ssize_t write(struct file *filp, const char __user *buff, size_t count, loff_t *offp){
    printk("WRITE\n");
    return (ssize_t)0;
}

/***/

module_init(init);
module_exit(cleanup);
MODULE_DESCRIPTION("Gamepad char device driver.");
MODULE_LICENSE("GPL");