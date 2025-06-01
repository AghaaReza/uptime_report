#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include <linux/ktime.h>

#define PROC_NAME "uptime_report"

static struct proc_dir_entry *proc_file;

// Helper: converts seconds into HH:MM:SS string
static void format_time(unsigned long total_seconds, char *buffer, size_t buf_size) {
    unsigned int hours, minutes, seconds;

    hours = total_seconds / 3600;
    minutes = (total_seconds % 3600) / 60;
    seconds = total_seconds % 60;

    snprintf(buffer, buf_size, "%02u:%02u:%02u", hours, minutes, seconds);
}

// Read handler for /proc/uptime_report
static ssize_t proc_read(struct file *file, char __user *buffer, size_t count, loff_t *offset) {
    static char msg[64];
    static int finished = 0;

    if (finished) {
        finished = 0;
        return 0;
    }

    unsigned long uptime_secs = jiffies / HZ;
    char time_str[16];

    format_time(uptime_secs, time_str, sizeof(time_str));
    snprintf(msg, sizeof(msg), "Uptime: %lu seconds (%s)\n", uptime_secs, time_str);

    if (copy_to_user(buffer, msg, strlen(msg)))
        return -EFAULT;

    finished = 1;
    return strlen(msg);
}

static const struct proc_ops proc_file_ops = {
    .proc_read = proc_read,
};

// Module initialization
static int __init uptime_report_init(void) {
    proc_file = proc_create(PROC_NAME, 0, NULL, &proc_file_ops);
    if (!proc_file) {
        pr_alert("uptime_report: Failed to create /proc entry\n");
        return -ENOMEM;
    }
    pr_info("uptime_report: Module loaded. /proc/%s created\n", PROC_NAME);
    return 0;
}

// Module cleanup
static void __exit uptime_report_exit(void) {
    proc_remove(proc_file);
    pr_info("uptime_report: Module unloaded. /proc/%s removed\n", PROC_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Reza Esmaeilzadeh");
MODULE_DESCRIPTION("Linux kernel module to report system uptime via /proc");

module_init(uptime_report_init);
module_exit(uptime_report_exit);
