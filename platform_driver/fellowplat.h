#ifndef __FELLOW_MISC_H
#define __FELLOW_MISC_H
#include <linux/ioctl.h>

//#define FELLOW_MISC_MAJOR 199
//#define FELLOW_MISC_NR 2

struct miscdata {
    int val;
    char *str;
    unsigned int size;
};

#define FELLOW_MISC_IOC_MAGIC 'f'
#define FELLOW_MISC_IOC_PRINT _IO(FELLOW_MISC_IOC_MAGIC, 1)
#define FELLOW_MISC_IOC_GET _IOR(FELLOW_MISC_IOC_MAGIC, 2, struct miscdata)
#define FELLOW_MISC_IOC_SET _IOW(FELLOW_MISC_IOC_MAGIC, 3, struct miscdata)
#define FELLOW_MISC_IOC_MAXNR 3

#endif /* __FELLOW_MISC_H */
