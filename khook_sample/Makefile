all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$$PWD
clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$$PWD clean
install:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$$PWD
	sudo insmod khook-sample.ko
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$$PWD clean
