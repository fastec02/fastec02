#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kprobes.h>
#include<linux/ptrace.h>
#define TARGET "_do_fork"
MODULE_LICENSE("GPL");

static int pre(struct kprobe *kp,struct pt_regs *regs)
{
	printk(KERN_INFO "[FOOK]PRE FUNCTION\n");
	return 0;
}

static struct kprobe kp =
{
	.symbol_name = TARGET,
	.pre_handler = pre,
	.post_handler = NULL,
	.fault_handler = NULL,
};

int kprobe_init(void)
{
	if(register_kprobe(&kp) != 0){
		printk(KERN_INFO "[FAILED]Can't find %s from kallsyms",TARGET);
		return -1;
	}else{
                printk(KERN_INFO "[SUCCESS]Find %s from kallsyms",TARGET);
	}
	printk(KERN_INFO "[INIT]---------------------\n");

	return 0;
}

void kprobe_exit(void)
{
	unregister_kprobe(&kp);
	printk(KERN_INFO "[EXIT]---------------------\n");
}

module_init(kprobe_init);
module_exit(kprobe_exit);
