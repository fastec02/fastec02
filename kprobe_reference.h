#pragma once

#include<linux/sched.h>
#include<linux/rcupdate.h>

static void printk_regs(const char* msg,struct kprobe *kp,struct pt_regs *regs);
static void printk_thread(const char* msg,struct kprobe *kp,struct pt_regs *regs);
static struct task_struct *find_task(pid_t pid);
static void printk_sp(const char* msg);


static int thread_sp;
static int reg_sp;

static void printk_sp(const char* msg)
{
	printk(KERN_ALERT "--[SP DIFFER]\n");
	printk(KERN_INFO "----[INFO]<THREAD_SP>     = 0x%x\n",thread_sp);
	printk(KERN_INFO "----[INFO]<REGS_SP>       = 0x%x\n",reg_sp);
	printk(KERN_INFO "----[INFO]<OFFSET>        = 0x%x\n",reg_sp - thread_sp);
}

static void printk_regs(const char* msg,struct kprobe *kp,struct pt_regs *regs)
{
	static int spreg;
	static int threadreg;
	static int bpreg;
	static int adreg;

	spreg = (int)regs->sp;
	threadreg = (int)kp->addr;
	bpreg = (int)regs->bp;
	adreg = (int)regs;

	reg_sp = (int)regs->sp;

	if(regs->ax != 1){
	printk(KERN_ALERT "--[KERNEL REGS]\n");
        printk(KERN_INFO "----[MESG] %s\n",msg);
        printk(KERN_INFO "----[INFO]<THREAD>        = 0x%x\n",kp->addr);
	//printk(KERN_INFO "----[INFO]<PT_REGS>       = 0x%x\n",regs);
        printk(KERN_INFO "----[INFO]<IP>            = 0x%x\n",regs->ip);
        //printk(KERN_INFO "----[INFO]<RAX>           = 0x%x\n",regs->ax);
        //printk(KERN_INFO "----[INFO]<RBX>           = 0x%x\n",regs->bx);
        //printk(KERN_INFO "----[INFO]<RCX>           = 0x%x\n",regs->cx);
        printk(KERN_INFO "----[INFO]<RDX>           = 0x%x\n",regs->dx);
	printk(KERN_INFO "----[INFO]<SP>            = 0x%x\n",regs->sp);
	printk(KERN_INFO "----[INFO]<BP>            = 0x%x\n",regs->bp);

	printk(KERN_INFO "----[INFO]<&SP>           = 0x%x\n",&regs->sp);
	printk(KERN_INFO "----[INFO]<&THREAD>       = 0x%x\n",&kp->addr);
	printk(KERN_INFO "----[INFO]<&PT_REGS>      = 0x%x\n",&regs);
	printk(KERN_INFO "----[SUBS]<THREAD_SIZE>   = %d\n",THREAD_SIZE);
	printk(KERN_INFO "----[SUBS]<RETURNADDR>    = 0x%x\n",regs->bp + 4);
	printk(KERN_INFO "----[SUBS]<BP - SP>       = %d\n",bpreg - spreg);
	printk(KERN_INFO "----[SUBS]<THR - SP>      = %d\n",spreg - threadreg);
	printk(KERN_INFO "----[SUBS]<SP - REG>      = %d\n",spreg - adreg);

	//printk(KERN_INFO "----[MSRS]MSR_LBR_TOS      = 0x%p\n",MSR_LBR_TOS);
	}

}
static void printk_thread(const char* msg,struct kprobe *kp,struct pt_regs *regs)
{
	struct task_struct *task;
	struct task_struct *cur = current;

	//static int spreg;
	//static int threadreg;

	rcu_read_lock();
	list_for_each_entry_rcu(task,&init_task.tasks,tasks){
		if(task->pid == cur->pid){
			//spreg = (int)task->&thread.sp;
			//threadreg = (int)task->thread;
			thread_sp = (int)task->thread.sp;
			printk(KERN_ALERT "--[THREAD_INFO]\n");
			printk(KERN_INFO "----[MESG] %s\n",msg);
                        printk(KERN_INFO "----[INFO]<FUNCTION>      = %s\n",task->comm);
			printk(KERN_INFO "----[INFO]<ADDR>          = 0x%x\n",kp->addr);
			printk(KERN_INFO "----[INFO]<THREAD>        = 0x%x\n",&task->thread);
			printk(KERN_INFO "----[INFO]<PID>           = %d\n",task->pid);
			printk(KERN_INFO "----[INFO]<PTR>           = 0x%x\n",&task->thread_info);
			printk(KERN_INFO "----[INFO]<BASE>          = 0x%x\n",&task->thread_info + THREAD_SIZE);
			printk(KERN_INFO "----[INFO]<STATUS>        = 0x%x\n",task->thread_info.status);
			printk(KERN_INFO "----[INFO]<FLAGS>         = 0x%x\n",task->thread_info.flags);
			printk(KERN_INFO "----[INFO]<SP>            = 0x%x\n",task->thread.sp);

			printk(KERN_INFO "----[INFO]<&SP>           = 0x%x\n",&task->thread.sp);
			printk(KERN_INFO "----[INFO]<&THREAD>       = 0x%x\n",&task->thread);
			printk(KERN_INFO "+++++++++\n");
			//printk(KERN_INFO "----[SUBS]<STACK_SIZE>	= %d\n",spreg - threadreg);

		}
	}
	rcu_read_unlock();
}

struct task_struct *find_task(pid_t pid)
{
	struct task_struct *p = current;
	struct task_struct *ret = NULL;

	rcu_read_lock();
	for_each_process(p)
	{
		if(p->pid == pid){
			get_task_struct(p);
			ret = p;
		}
	}
	rcu_read_unlock();
	printk(KERN_ALERT "--[FIND_TASK]\n");
	printk(KERN_ALERT "----[INFO]<CURRENT_PID>	= %d\n",current->pid);
	return ret;
}
