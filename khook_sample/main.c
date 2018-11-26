#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>


#include "engine/engine.h"
#include "engine/engine.c"

MODULE_LICENSE("GPL");

KHOOK(sys_read);
static ssize_t khook_sys_read(unsigned int fd,char __user *buf,size_t count)
{
	struct file *f;
	ssize_t ret;

	KHOOK_GET(sys_read);
	if(f){
		ret = vfs_read(f,buf,count,&f->f_pos);

		printk(KERN_INFO "<!!FOOK>sys_read\n");
	}else{
		ret = KHOOK_ORIGIN(sys_read,fd,buf,count);
	}

	KHOOK_PUT(sys_read);

	return ret;
}


KHOOK(sys_getdents);
static int khook_sys_getdents(unsigned int fd,struct linux_dirent __user *dirent,unsigned int count)
{
	int ret;

	KHOOK_GET(sys_getdents);
	ret = KHOOK_ORIGIN(sys_getdents,fd,dirent,count);
	printk(KERN_INFO "<!!HOOK>sys_getdents\n");
	KHOOK_PUT(sys_getdents);

	return ret;
}

//KHOOK(sys_uname);
//static int khook_sys_uname(struct old_utsname *buf)
//{
//       int ret;
//        struct old_utsname *fakename = {0,"","","",""};
//       KHOOK_GET(sys_uname);
//        ret = KHOOK_ORIGIN(sys_uname,fakename);
//
//        printk(KERN_INFO "<!!HOOK>sys_uname\n");
//        KHOOK_PUT(sys_uname);
//
//        return ret;
//}

int init_module(void)
{
	return khook_init();
}

void cleanup_module(void)
{
	khook_cleanup();
}
