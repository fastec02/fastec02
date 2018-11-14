#pragma once

#include<linux/sched.h>
#include<linux/rcupdate.h>

static void printk_regs(const char* msg,struct kprobe *kp,struct pt_regs *regs);
static void printk_thread(const char* msg,struct kprobe *kp,struct pt_regs *regs);
static struct task_struct *find_task(pid_t pid);



static void printk_regs(const char* msg,struct kprobe *kp,struct pt_regs *regs)
{

	printk(KERN_ALERT "--[KERNEL REGS]\n");
        printk(KERN_INFO "----[MESG] %s\n",msg);
        printk(KERN_INFO "----[INFO]<THREAD>		= 0x%p\n",kp->addr);
        printk(KERN_INFO "----[INFO]<IP>   		= 0x%lx\n",regs->ip);
        printk(KERN_INFO "----[INFO]<RAX>   		= 0x%lx\n",regs->ax);
        printk(KERN_INFO "----[INFO]<RBX>		= 0x%lx\n",regs->bx);
        printk(KERN_INFO "----[INFO]<RCX>		= 0x%lx\n",regs->cx);
        printk(KERN_INFO "----[INFO]<RDX>		= 0x%lx\n",regs->dx);
	printk(KERN_INFO "----[INFO]<SP>   		= 0x%lx\n",regs->sp);
	printk(KERN_INFO "----[INFO]<BP>    		= 0x%lx\n",regs->bp);

	printk(KERN_INFO "----[INFO]<&SP>		= 0x%lx\n",&regs->sp);
	printk(KERN_INFO "----[INFO]<&THREAD>		= 0x%lx\n",&kp->addr);

}
static void printk_thread(const char* msg,struct kprobe *kp,struct pt_regs *regs)
{
	struct task_struct *task;
	struct task_struct *cur = current;

	rcu_read_lock();
	list_for_each_entry_rcu(task,&init_task.tasks,tasks){
		if(task->pid == cur->pid){
			printk(KERN_ALERT "--[THREAD_INFO]\n");
			printk(KERN_INFO "----[MESG] %s\n",msg);
                        printk(KERN_INFO "----[INFO]<FUNCTION>		= %s\n",task->comm);
			printk(KERN_INFO "----[INFO]<ADDR>		= 0x%lx\n",kp->addr);
			printk(KERN_INFO "----[INFO]<THREAD>		= 0x%lx\n",task->thread);
			printk(KERN_INFO "----[INFO]<PID>		= %d\n",task->pid);
			printk(KERN_INFO "----[INFO]<PTR>		= 0x%lx\n",task->thread_info);
			printk(KERN_INFO "----[INFO]<STATUS>		= 0x%lx\n",task->thread_info.status);
			printk(KERN_INFO "----[INFO]<FLAGS>		= 0x%lx\n",task->thread_info.flags);
			printk(KERN_INFO "----[INFO]<SP>		= 0x%lx\n",task->thread.sp);

			printk(KERN_INFO "----[INFO]<&SP>		= 0x%lx\n",&task->thread.sp);
			printk(KERN_INFO "----[INFO]<&THREAD>		= 0x%lx\n",&task->thread);
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
