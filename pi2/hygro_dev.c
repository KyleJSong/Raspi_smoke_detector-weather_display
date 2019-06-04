#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/gpio.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/delay.h>



#define HYGRO_GPIO  13
#define HYGRO_MAJOR	241

#define HIGH 1
#define LOW  0

MODULE_LICENSE("GPL");

int hygro_status=1;

int hygro_read(struct file * flip, char * buf, size_t counts, loff_t * f_pos) {
	printk(KERN_ALERT "read\n");
  // gpio_direction_input(FIREGPIO);//
  int data[5]={0,0,0,0,0};
  int count =0;
int index=-1;
int prev = HIGH;
int dcount =0;
int i =0;


gpio_direction_output(HYGRO_GPIO, HIGH);
gpio_set_value(HYGRO_GPIO , HIGH);
mdelay(30);
gpio_set_value(HYGRO_GPIO , LOW);
mdelay(18);
gpio_set_value(HYGRO_GPIO , HIGH);
udelay(40);
gpio_direction_input(HYGRO_GPIO);

for(i=0;i<90; i++)
{
	count=0;
	while(1)
	{    
		
		if( gpio_get_value(HYGRO_GPIO)!=prev)
		break;
		
		count++;
		udelay(1);
		if(count>100)break;
		
	}
	if(count>100)break;
	
	//read(fd, &button_status,sizeof(int));
	//prev= button_status;
	prev= gpio_get_value(HYGRO_GPIO);
	
	
	
	if((i>=4)&&(i%2==0))
	{
		
		if((dcount%8)==0)
		index++;
		 
		data[index] = data[index] * 2;
		if(count>50)
		{
		data[index] = data[index] + 1;
	    }
	    
		dcount++;
	}
	
}
	int wrong =-1;
	printk(KERN_ALERT "data 0 : %d\n", data[0]);
if((dcount>=40)&& (data[4]==(data[0] + data[1] + data[2] +data[3]) )&&data[0]!=0)
{

	//printf("humidity = %d.%d %% tmperature = %d.%d C\n", data[0], data[1], data[2],data[3]);
	
	copy_to_user(buf, data, sizeof(int)*4);
}
else
{
	copy_to_user(buf, &wrong, sizeof(int));
}

  //
	
	return counts;
}

static struct file_operations fops = {
	.read = hygro_read,
};

int __init hygro_init(void) 
{
	printk(KERN_ALERT "HYGRO : init \n");

	register_chrdev(HYGRO_MAJOR, "hygro_dev", &fops);

	gpio_request(HYGRO_GPIO, "HYGRO_GPIO");
	gpio_direction_output(HYGRO_GPIO, HIGH);
	//gpio_direction_input(FIREGPIO);

	printk(KERN_INFO "HYGRO: init done \n");

	return 0;
}

void __exit hygro_exit(void){
	gpio_free(HYGRO_GPIO);
	
	unregister_chrdev(HYGRO_MAJOR, "hygro_dev");
	
	printk(KERN_INFO "HYGRO : Exit");
}

module_init(hygro_init);
module_exit(hygro_exit);
