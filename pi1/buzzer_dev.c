#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

#define BUZZER_GPIO 20 // I/O
#define DEV_NAME "buzzer_dev"
#define DEV_NUM 242

#define BUZZER_ON 1
#define BUZZER_OFF 0

MODULE_LICENSE("GPL");

#define IOCTL_MAGIC_NUMBER 'j'

#define IOCTL_BUZZER_ON		_IO(IOCTL_MAGIC_NUMBER, 0)
#define IOCTL_BUZZER_OFF	_IO(IOCTL_MAGIC_NUMBER, 1)

int buzzer_open(struct inode *pinode, struct file *pfile){ 
	printk(KERN_INFO "open buzzer_dev\n"); 
	gpio_request(BUZZER_GPIO,"buzzer"); 
	gpio_direction_output(BUZZER_GPIO,BUZZER_OFF); // open with "OFF" state 
	return 0; 
} 

int buzzer_close(struct inode *pinode, struct file *pfile){ 
	printk(KERN_INFO "release buzzer_dev\n"); 
	gpio_set_value(BUZZER_GPIO,BUZZER_OFF); 
	return 0; 
} 

long buzzer_ioctl(struct file * filp, unsigned int cmd, unsigned long arg){
	
	switch(cmd){
		case IOCTL_BUZZER_ON:
			gpio_set_value(BUZZER_GPIO, BUZZER_ON);
			break;
				 
		case IOCTL_BUZZER_OFF:
			gpio_set_value(BUZZER_GPIO, BUZZER_OFF);  
			break;
	}       	 
	
	return 0;
}

struct file_operations fop = { 
	.owner = THIS_MODULE, 
	.open = buzzer_open, 
	.unlocked_ioctl = buzzer_ioctl,
	.release = buzzer_close, 
}; 

int __init buzzer_init(void){ 
	printk(KERN_INFO "BUZZER : init\n"); 
	register_chrdev(DEV_NUM, DEV_NAME, &fop);
        
	gpio_request(BUZZER_GPIO,"GPIO20");
	gpio_direction_output(BUZZER_GPIO, BUZZER_ON);
        
	printk(KERN_INFO "BUZZER : init done\n"); 
	return 0; 
}

void __exit buzzer_exit(void){ 
	gpio_free(BUZZER_GPIO);
	unregister_chrdev(DEV_NUM,DEV_NAME);
	printk(KERN_INFO "BUZZER : exit\n"); 
} 

module_init(buzzer_init); 
module_exit(buzzer_exit); 



