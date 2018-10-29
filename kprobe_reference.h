#include<linux/sched.h>
#include<linux/rwlock.h>
static void printk_regs(const char* msg,struct kprobe *kp,struct pt_regs *regs);
static void printk_thread(struct kprobe *kp,struct pt_regs *regs);


static void printk_regs(const char* msg,struct kprobe *kp,struct pt_regs *regs)
{
	printk(KERN_INFO "--[KERNEL REGS]\n");
        printk(KERN_INFO "----[MESG] %s\n",msg);
        printk(KERN_INFO "----[INFO]p->addr = 0x%p\n",kp->addr);
        printk(KERN_INFO "----[INFO]ip      = %lx\n",regs->ip);
        printk(KERN_INFO "----[INFO]<RAX>   = 0x%lx\n",regs->ax);
        printk(KERN_INFO "----[INFO]<RBX>   = 0x%lx\n",regs->bx);
        printk(KERN_INFO "----[INFO]<RCX>   = 0x%lx\n",regs->cx);
        printk(KERN_INFO "----[INFO]<RDX>   = 0x%lx\n",regs->dx);
}

static void printk_thread(struct kprobe *kp,struct pt_regs *regs)
{
	struct task_struct *task;

	struct thread_info *current_thread = task_thread_info(task);

	printk(KERN_INFO "--[THREAD_INFO]\n");
	printk(KERN_INFO "----[INFO]<PID>	= %x\n",task->pid);
	printk(KERN_INFO "----[INFO]<PTR>	= %lx\n",task->thread_info);
	printk(KERN_INFO "----[INFO]<STATUS>	= %lx\n",task->thread_info.status);
	printk(KERN_INFO "----[INFO]<FLAGS>	= %lx\n",task->thread_info.flags);

}

