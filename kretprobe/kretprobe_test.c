#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/ptrace.h>
#include<linux/sched.h>
#include<linux/rcupdate.h>
#include<linux/kallsyms.h>
#include<linux/threads.h>
#include<linux/kprobes.h>

MODULE_LICENSE("GPL");

static int mprotect_ret_handler(struct kretprobe_instance *ri,struct pt_regs *regs)
{
	printk(KERN_INFO "Original return address: 0x%lx\n",(unsigned long)ri->ret_addr);
	return 0;
}

static struct kretprobe mprotect_kretprobe =
{
	.handler = mprotect_ret_handler,
	.maxactive = NR_CPUS
};

int init_module(void)
{
	mprotect_kretprobe.kp.addr = (kprobe_opcode_t *)kallsyms_lookup_name("sys_mprotect");
	register_kretprobe(&mprotect_kretprobe);
	printk(KERN_INFO "[INIT]KRET\n");
}

int exit_module(void)
{
	unregister_kretprobe(&mprotect_kretprobe);
	printk(KERN_INFO "[EXIT]KRET\n");
}
