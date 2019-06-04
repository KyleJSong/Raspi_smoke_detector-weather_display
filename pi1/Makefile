#KERNELDIR = /lib/modules/$(shell uname -r)/build
KERNELDIR = /lib/modules/4.14.98-v7+/build

obj-m := buzzer_dev.o

PWD := $(shell pwd)

all :
	make -C $(KERNELDIR) M=$(PWD) modules
      
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
	rm adder.so

exe:
	sudo python fire_and_smoke.py

in:
	sudo insmod buzzer_dev.ko
	sudo mknod -m 666 /dev/buzzer_dev c 242 0
	gcc -shared -Wl,-soname,adder.so -o adder.so -fPIC app.c

   
out:
	sudo rmmod buzzer_dev
	sudo rm -rf /dev/buzzer_dev
