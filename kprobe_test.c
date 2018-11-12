#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kprobes.h>
#include<linux/ptrace.h>
#include "kprobe_reference.h"
#define TARGET "sys_getdents"
MODULE_LICENSE("GPL");

static int pre(struct kprobe *kp,struct pt_regs *regs)
{
	printk(KERN_INFO "+\n");
	printk(KERN_INFO "[FOOK]PRE FUNCTION '%s'\n",TARGET);
	printk_thread("PRE",kp,regs);
	printk_regs("PRE",kp,regs);
	printk(KERN_INFO "+\n");
	//find_task(1);
	return 0;
}

static void post(struct kprobe *kp,struct pt_regs *regs,unsigned long flags)
{
        //printk(KERN_INFO "*\n");
        //printk(KERN_INFO "[FOOK]PRE FUNCTION '%s'\n",TARGET);
        //printk_thread("POST",kp,regs);
        //printk_regs("POST",kp,regs);
        //printk(KERN_INFO "*\n");
        //find_task(1);
        return 0;
}


static struct kprobe kp =
{
	.symbol_name = TARGET,
	.pre_handler = pre,
	.post_handler = post,
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
