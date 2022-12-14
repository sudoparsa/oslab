#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Parsa Hosseini");
MODULE_DESCRIPTION("print syscall address");
MODULE_VERSION("1.0");

static unsigned long *syscall_table = (unsigned long *) 0xffffffff836002a0;

static int __init km_init(void) {
	printk(KERN_INFO "Syscall table is at %lx\n", syscall_table[0]);
	printk(KERN_INFO "Read syscall address is at %lx\n", syscall_table[__NR_read]);
	printk(KERN_INFO "Write syscall address is at %lx\n", syscall_table[__NR_write]);
	printk(KERN_INFO "Open syscall address is at %lx\n", syscall_table[__NR_open]);
	printk(KERN_INFO "Close syscall address is at %lx\n", syscall_table[__NR_close]);
	printk(KERN_INFO "Last syscall address is at %lx\n", syscall_table[NR_syscalls - 1]);
	printk(KERN_INFO "Number of system calls in the system calls table: %d\n", NR_syscalls);
	return 0;
}

static void __exit km_exit(void) {
	printk(KERN_INFO "End\n");
}

module_init(km_init);
module_exit(km_exit);

