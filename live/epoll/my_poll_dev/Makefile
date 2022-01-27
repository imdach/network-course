obj-m += my_poll_dev.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
insmod:
	sudo insmod my_poll_dev.ko
rmmod:
	sudo rmmod my_poll_dev.ko
