#include <linux/module.h>
#include <linux/init.h>
//#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "fellowplat.h"

struct fellowmisc_dev{
    struct miscdevice misc;
    struct miscdata data;
};

struct fellowmisc_dev *fellowmisc_devp;
static struct platform_device *fellow_platform_device;

int fellowmisc_open(struct inode *inode, struct file *filep)
{
    filep->private_data = fellowmisc_devp;
    return 0;
}
int fellowmisc_release(struct inode *inode, struct file *filep)
{
    return 0;
}

long fellowmisc_ioctl(struct file *filep,unsigned int cmd,unsigned long arg)
{
    int ret = 0;
    struct fellowmisc_dev *devp = (struct fellowmisc_dev *)(filep->private_data);

    if (_IOC_TYPE(cmd) != FELLOW_MISC_IOC_MAGIC)
        return -EINVAL;

    if (_IOC_NR(cmd) > FELLOW_MISC_IOC_MAXNR)
        return -EINVAL;

    switch(cmd) {
        case FELLOW_MISC_IOC_PRINT:
            printk("FELLOW_MISC_IOC_PRINT\n");
            printk("val:%d, size: %d, str: %s\n", devp->data.val, devp->data.size, devp->data.str);
            break;
        case FELLOW_MISC_IOC_SET:
            printk("FELLOW_MISC_IOC_SET\n");
            ret = copy_from_user((unsigned char*)&(devp->data), (unsigned char *)arg, sizeof(struct miscdata));
            printk("set val:%d, size: %d, str: %s\n", devp->data.val, devp->data.size, devp->data.str);
            break;
        case FELLOW_MISC_IOC_GET:
            printk("FELLOW_MISC_IOC_GET\n");
            ret = copy_to_user((unsigned char*)arg,(unsigned char*)&(devp->data), sizeof(struct miscdata));
            break;
        default:
            return -EINVAL;
    }

    return ret;
}

static const struct file_operations fellowmisc_fops ={
    .owner = THIS_MODULE,
    .open = fellowmisc_open,
    .release = fellowmisc_release,
    .unlocked_ioctl = fellowmisc_ioctl,
};

static struct miscdevice fellow_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "fellowplat",
    .fops = &fellowmisc_fops,
};

static int fellow_plat_drv_probe(struct platform_device *dev)
{
    int ret;

    printk("fellow_plat_drv_probe\n");

    fellowmisc_devp = kmalloc(sizeof(struct fellowmisc_dev), GFP_KERNEL);
    if (!fellowmisc_devp) {
        ret = -ENOMEM;
        return ret;
    }

    memset(&(fellowmisc_devp->data), 0, sizeof(fellowmisc_devp->data));

    fellowmisc_devp->misc = fellow_misc;

    ret = misc_register(&fellowmisc_devp->misc);

    return ret;
}

static int fellow_plat_drv_remove(struct platform_device *dev)
{
    struct fellowmisc_dev *fdev;

    fdev = platform_get_drvdata(dev);
    if (!fdev)
        return -ENODATA;

    if (fellowmisc_devp)
        kfree(fellowmisc_devp);

    misc_deregister(&fdev->misc);

    return 0;
}

static struct platform_driver fellow_platform_driver = {
    .probe = fellow_plat_drv_probe,
    .remove = fellow_plat_drv_remove,
    .driver = {
        .name = "fellow",
    },
};

static int fellowplat_init(void)
{
    int ret;

    printk("fellowplat_init\n");
    printk("fellow register driver\n");

    ret = platform_driver_register(&fellow_platform_driver);
    if (ret)
        return ret;

    fellow_platform_device = platform_device_alloc("fellow", -1);
    if (!fellow_platform_device) {
        ret = -ENOMEM;
        goto err_driver_unregister;
    }

    printk("fellow register device\n");

    ret = platform_device_add(fellow_platform_device);
    if (ret)
        goto err_free_device;

    return 0;

err_free_device:
    platform_device_put(fellow_platform_device);

err_driver_unregister:
    platform_driver_unregister(&fellow_platform_driver);

    return ret;
}

static void fellowplat_exit(void)
{
    platform_device_unregister(fellow_platform_device);
    platform_driver_unregister(&fellow_platform_driver);
}

MODULE_AUTHOR("fellow");
MODULE_DESCRIPTION("A sample platform driver code");
MODULE_LICENSE("GPL");

module_init(fellowplat_init);
module_exit(fellowplat_exit);

