#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kprobes.h>
#include<linux/ptrace.h>

MODULE_LICENSE("GPL");

static int pre(struct kprobe *kp,struct pt_regs *regs)
{
	printk(KERN_INFO "[FOOK]PRE FUNCTION\n");
	return 0;
}

static struct kprobe kp =
{
	.symbol_name = "sys_open",
	.pre_handler = pre,
	.post_handler = NULL,
	.fault_handler = NULL,
};

int kprobe_init(void)
{
	printk(KERN_INFO "[INIT]---------------------\n");
	printk(KERN_INFO "register_kprobe:{%d}\n",register_kprobe(&kp));
	return 0;
}

void kprobe_exit(void)
{
	unregister_kprobe(&kp);
	printk(KERN_ALERT "[EXIT]--------------------\n");
}

module_init(kprobe_init);
module_exit(kprobe_exit);
