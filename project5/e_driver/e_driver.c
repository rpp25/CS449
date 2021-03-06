//author rpp25. E Driver
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

#include <asm/uaccess.h>
#include "e.h"

/*
 * e_driver_read is the function called when a process calls read() on
 * /dev/e.  It writes "Hello, world!" to the buffer passed in the
 * read() call.
 */

static ssize_t e_driver_read(struct file * file, char * buf,
			  size_t count, loff_t *ppos)
{
	int num_digits = count + *ppos; //size of the file + pointer position
	char buffer[num_digits];
	
	e(buffer, num_digits); //get digits of e

	/*
	 * Besides copying the string to the user provided buffer,
	 * this function also checks that the user has permission to
	 * write to the buffer, that it is mapped, etc.
	 */
	if (copy_to_user(buf, &buffer[*ppos], count))
		return -EINVAL;
	/*
	 * Tell the user how much data we wrote.
	 */

	*ppos = count;
	//return num_digits;
	return count;
}
/*
 * The only file operation we care about is read.
 */

static const struct file_operations e_driver_fops = {
	.owner		= THIS_MODULE,
	.read		= e_driver_read,
};

static struct miscdevice e_driver_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/e_driver.
	 */
	"e_driver",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&e_driver_fops
};

static int __init
e_driver_init(void)
{
	int ret;

	/*
	 * Create the "e_driver" device in the /sys/class/misc directory.
	 * Udev will automatically create the /dev/e_driver device using
	 * the default rules.
	 */
	ret = misc_register(&e_driver_dev);
	if (ret)
		printk(KERN_ERR
		       "Unable to register \"e_driver\" misc device\n");

	return ret;
}

module_init(e_driver_init);

static void __exit
e_driver_exit(void)
{
	misc_deregister(&e_driver_dev);
}

module_exit(e_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Valerie Henson <val@nmt.edu>");
MODULE_DESCRIPTION("\"Hello, world!\" minimal module");
MODULE_VERSION("dev");
