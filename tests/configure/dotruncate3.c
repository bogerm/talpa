#ifndef KBUILD_BASENAME
#define KBUILD_BASENAME "#dotruncatetest"
#endif

#include "autoconf.h"
#include <linux/fs.h>

/* do_truncate(struct dentry*, loff_t, unsigned int, struct file*) */
int main()
{
    struct dentry *dentry = NULL;
    loff_t start = 0;
    struct file *filp = NULL;
    unsigned int time_attrs = 0;

    return do_truncate(dentry, start, time_attrs, filp);
}
