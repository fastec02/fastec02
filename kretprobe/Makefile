obj-m += kretprobe_test.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
install:
	make
	sudo insmod kretprobe_test.ko
	make clean
