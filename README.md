# uptime_report

A simple Linux kernel module that reports system uptime via a `/proc` entry.  
It returns both the number of seconds since boot and a human-readable HH:MM:SS format.

---

**Build Instructions**

Make sure your Linux kernel headers are installed. Then run:

```bash
make
````
**Usage**

1. Load the Module

Use `insmod` to insert the compiled kernel module:

```bash
sudo insmod uptime_report.ko
```

2. Read Uptime from `/proc`

Access the uptime information by reading the proc file:

```bash
cat /proc/uptime_report
```

Example output:

```
Uptime: 42816 seconds (11:53:36)
```

This shows how long the system has been running in both seconds and HH:MM:SS format.

3. Unload the Module

To remove the module from the kernel:

```bash
sudo rmmod uptime_report
```

4. View Kernel Logs

To verify module activity:

```bash
dmesg | grep uptime_report
```

