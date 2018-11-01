#include<linux/sched.h>
#include<linux/rcupdate.h>

static void printk_regs(const char* msg,struct kprobe *kp,struct pt_regs *regs);
static void printk_thread(struct kprobe *kp,struct pt_regs *regs);

static unsigned int sp_register;

static void printk_regs(const char* msg,struct kprobe *kp,struct pt_regs *regs)
{
	printk(KERN_ALERT "--[KERNEL REGS]\n");
        printk(KERN_INFO "----[MESG] %s\n",msg);
        printk(KERN_INFO "----[INFO]p->addr = 0x%p\n",kp->addr);
        printk(KERN_INFO "----[INFO]ip      = %lx\n",regs->ip);
        printk(KERN_INFO "----[INFO]<RAX>   = 0x%lx\n",regs->ax);
        printk(KERN_INFO "----[INFO]<RBX>   = 0x%lx\n",regs->bx);
        printk(KERN_INFO "----[INFO]<RCX>   = 0x%lx\n",regs->cx);
        printk(KERN_INFO "----[INFO]<RDX>   = 0x%lx\n",regs->dx);
	printk(KERN_INFO "----[INFO]<SP>    = 0x%lx\n",regs->sp);
	sp_register = regs->sp;
}

static void printk_thread(struct kprobe *kp,struct pt_regs *regs)
{
	struct task_struct *task;
	rcu_read_lock();
	list_for_each_entry_rcu(task,&init_task.tasks,tasks){
	//if(sp_register == task->thread.sp){
			printk(KERN_ALERT "--[THREAD_INFO]\n");
			printk(KERN_INFO "----[INFO]<ADDR>	= %lx\n",kp->addr);
			printk(KERN_INFO "----[INFO]<FUNCTION>  = %s\n",task->comm);
			printk(KERN_INFO "----[INFO]<THREAD>	= %lx\n",task->thread);
			printk(KERN_INFO "----[INFO]<REG-SP>	= %lx\n",task->thread.sp);
			printk(KERN_INFO "----[INFO]<PID>	= %d\n",task->pid);
			printk(KERN_INFO "----[INFO]<PTR>	= %lx\n",task->thread_info);
			printk(KERN_INFO "----[INFO]<STATUS>	= %lx\n",task->thread_info.status);
			printk(KERN_INFO "----[INFO]<FLAGS>	= %lx\n",task->thread_info.flags);
		//}
	}
	rcu_read_unlock();
}
