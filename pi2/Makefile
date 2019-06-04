KERNELDIR = /lib/modules/4.14.98-v7+/build

obj-m := hygro_dev.o

PWD := $(shell pwd)

all :
	make -C $(KERNELDIR) M=$(PWD) modules
	gcc -o pi2_app pi2_app.c lcd_test.c dust.c -lwiringPi -lpthread
      
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
	rm pi2_app

in:
	sudo insmod hygro_dev.ko
	sudo mknod -m 666 /dev/hygro_dev c 241 0
   
out:
	sudo rmmod hygro_dev.ko
	sudo rm -rf /dev/hygro_dev
