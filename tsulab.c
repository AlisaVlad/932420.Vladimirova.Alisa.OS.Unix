#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/jiffies.h>

#define PROCFS_NAME "tsulab"

static struct proc_dir_entry* proc_file;

static ssize_t procfile_read(struct file* file, char __user* buffer, size_t buffer_length, loff_t* offset)
{
    char message[128];
    int len;
    unsigned long uptime_sec;

    if (*offset != 0)
        return 0;

    uptime_sec = jiffies_to_msecs(get_jiffies_64()) / 1000;

    len = snprintf(message, sizeof(message), "OperSys work: %lu second\n", uptime_sec);

    if (len > buffer_length)
        len = buffer_length;

    if (copy_to_user(buffer, message, len))
        return -EFAULT;

    *offset += len;
    return len;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static const struct proc_ops proc_file_ops = {
    .proc_read = procfile_read,
};
#else
static const struct file_operations proc_file_ops = {
    .read = procfile_read,
};
#endif

static int __init tsulab_init(void)
{
    proc_file = proc_create(PROCFS_NAME, 0644, NULL, &proc_file_ops);
    if (!proc_file) {
        pr_err("Failed to create /proc/%s\n", PROCFS_NAME);
        return -ENOMEM;
    }

    pr_info("Welcome to the Tomsk State University\n");
    return 0;
}

static void __exit tsulab_exit(void)
{
    proc_remove(proc_file);
    pr_info("Tomsk State University forever!\n");
}

module_init(tsulab_init);
module_exit(tsulab_exit);

MODULE_LICENSE("GPL");