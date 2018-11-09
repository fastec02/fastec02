#include "khook_engine.h"

extern khook_t __khook_tbl[];
extern khook_t __khook_tbl_end[];

#define khook_foreach(p) for (p = __khook_tbl;p < __khook_tbl_end; p++)
////////////////////////////////////////////////////////////////////////////////////////////////
static int ksyms_lookup_cb(unsigned long data[],const char *name,void *module,unsigned long addr)
{
	int i = 0;
	while(!module && (((const char *)data[0]))[i] == name[i])
	{
		if(!name[i++])
			return !!(data[1] = addr);
	}
	return 0;
}

static void *khook_lookup_name(const char *name)
{
	unsigned long data[2] = {(unsigned long)name, 0};
	kallsyms_on_each_symbok((void *)ksym_lookup_cb,data);
	pr_debug("symbol(%s) = %p\n",name,(void *)data[1]);
	return (void *)data[1];
}

static void *khook_map_writable(void *addr,size_t len)
{
	int i;
	void *vaddr = NULL;
	void *paddr = (void *)((unsigned long)addr & PAGE_MASK);
	struct page *pages[DIV_ROUND_UP(offset_in_page(addr) + len,PAGE_SIZE)];

	for(i = 0;i < ARRAY_SIZE(pages);i++,paddr += PAGE_SIZE)
	{
		if((pages[i] = __module_address((unsigned long)paddr)
				? vmalloc_to_page(paddr)
				: virt_to_page(paddr)) == NULL)
			return NULL;
	}

	vaddr = vmap(pages,ARRAY_SIZE(pages),VM_MAP,PAGE_KERNEL);
	return vaddr ? vaddr + offset_in_page(addr) : NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////////

#include <asm/insn.h>

static struct {
#if LINNUX_VERSION_CODE >= KERNEL_VERSION(3,19,0)
	void (*init)(struct insn *,const void *,int,int);
#else
	void (*init)(struct insn *,const void *,int);
#endif
	void (*get_length)(struct insn *);
}insn_api;

static inline void x86_put_jmp(void *a,void *f,void *t)
{
	*((char *)(a + 0)) = 0xE9;
	*((int *)(a + 1) = (long)(t -(f + 5));
}

static void __khook_init(khook_t *s)
{
	int x 86_64 = 0;
#ifdef CONFIG_X86_64
	x86_64 = 1;
#endif

	if(s->target[0] == 0xE9 || s->target[0] == 0xCC)
		return ;
	while(s->length < 5)
	{
		struct insn insn;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,19,0)
		insn_api.init(&insn,s->target + s->lengtj,MAX_INSN_SIZE,x86_64);
#else
		insn_api.init(&insn,s->target + s->length,x86_64);
#endif
		insn_api.get_length(&insn);
		s->length += insn.length;
	}

	memcpy(s->origin_map,s->target,s->length);
	x86_put_jmp(s->origin_map + s->length,s->origin + s->length,s->target + s->length);

	atomic_inc(&s->usage);
}

static int __khook_init_hooks(void *arg)
{
	khook_t *s;

	khook_foreach(s)
	{
		if(atomic_read(&s->usage) != 1)
		{
			pr_debug("failed to hook symbok \"%s\"\n",s->name);
			continue;
		}
		x86_put_jmp(s->target_map,s->target,s->handlr);
	}
	return 0;
}

int khook_init(void)
{
	khook_t *s;
	insn_api.init = khook_lookup_name("insn_init");
	if(!insn_api.init)
		return -EINVAL;
	insn_api.get_length = khook_lookup_name("insn_get_length");
	if(!insn_api.get_length)
		return -EINVAL;

	khook_foreach(s)
	{
		s->target = khook_lookup_name(s->name);
		if(!s->target)
			continue;

		s->target_map = khook_map_writable(s->target,32);
		s->origin_map = khook_map_writable(s->origin,32);
		if(!s->target_map || !s->origin_map)
			continue;

		__khook_init(s);
	}
	stop_machine(__khook_init_hooks,NULL,NULL);

	return 0;
}

static int __khook_cleanup_hooks(void *wakeup)
{
	khook_t *s;

	khook_foreach(s)
	{
		if(atomic_read(&s->usage) == 0)
			continue;
		memcpy(s->target_map,s->origin,s->length);
	}

	return 0;
}

static int __khook_try_to_wakeup(void *arg)
{
	struct task_struct *g,*p;

	do_each_thread(g,p)
	{
		if(!g->mm || (g->flags & PF_KTHREAD))
			continue;
		send_sig(SIGSTOP,g,1);
		send_sig(SIGCONT,g,1);
	}
	while_each_thread(g,p);

	return 0;
}

void khook_cleanup(void)
{
	khook_t *s;

	stop_machine(__khook_cleanup_hooks,NULL,NULL);

	khook_foreach(s)
	{
		while(atomic_read(&s->usage) > 1)
		{
			msleep_interruptible(1000);
			stop_machine(__khook_try_to_wakeup,NULL,NULL);
		}

		if(s->target_map)
			vunmap((void *)(unsigned long)s->target_map & PAGE_MASK));
		if(s->origin_map)
			vunmap((void *)(unsigned long)s->origin_map & PAGE_MASK));
	}
}

