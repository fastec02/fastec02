#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kprobes.h>
#include<linux/ptrace.h>
#include "kprobe_reference.h"
#define TARGET_SYMBOL "sys_getdents"
#define TARGET_ADDR 0xffffffffaa08b5b0
MODULE_LICENSE("GPL");

static int pre(struct kprobe *kp,struct pt_regs *regs)
{
	printk(KERN_INFO "+\n");
	printk(KERN_INFO "[FOOK]PRE FUNCTION '%s'\n",TARGET_SYMBOL);
	printk_thread("PRE",kp,regs);
	printk_regs("PRE",kp,regs);
	printk_sp("SP");
	printk(KERN_INFO "+\n");
	return 0;
}

static void post(struct kprobe *kp,struct pt_regs *regs,unsigned long flags)
{
        //printk(KERN_INFO "*\n");
        //printk(KERN_INFO "[FOOK]POST FUNCTION '%s'\n",TARGET_SYMBOL);
        //printk_thread("POST",kp,regs);
        //printk_regs("POST",kp,regs);
        //printk(KERN_INFO "*\n");
}

static int fault(struct kprobe *kp,struct pt_regs *regs,int tranpnr)
{
	printk(KERN_INFO "|\n");
	printk(KERN_INFO "[FOOK]FAULT FUNCTION '%s'\n",TARGET_SYMBOL);
	printk_thread("FAULT",kp,regs);
	printk_regs("FAULT",kp,regs);
	printk(KERN_INFO "|\n");
	return 0;
}


static struct kprobe kp =
{
	.symbol_name = TARGET_SYMBOL,
	.pre_handler = pre,
	.post_handler = post,
	.fault_handler = fault,
	//.addr = TARGET_ADDR
};

int kprobe_init(void)
{
	if(register_kprobe(&kp) != 0){
		printk(KERN_INFO "[FAILED]Can't find %s from kallsyms",TARGET_SYMBOL);
		return -1;
	}else{
                printk(KERN_INFO "[SUCCESS]Find %s from kallsyms",TARGET_SYMBOL);
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
