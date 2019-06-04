#ifdef _DEBUG
   #undef _DEBUG 
   #include <python3.5/Python.h>
   #define _DEBUG
#else
   #include <python3.5/Python.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>


#define IOCTL_MAGIC_NUMBER 'j'

#define IOCTL_BUZZER_ON      _IO(IOCTL_MAGIC_NUMBER, 0)
#define IOCTL_BUZZER_OFF   _IO(IOCTL_MAGIC_NUMBER, 1)

#define DEV_PATH "/dev/buzzer_dev"
void func(int firecheck, int smokecheck)
{
	int buz_fd =0;
	char *buffer;
	if((buz_fd =open(DEV_PATH, O_RDWR | O_NONBLOCK)) <0){
		perror("open()");
		exit(1);
	}
	printf("open success!\n");
   
	int channel = 0;
	//float fire_res = 0;
  
	printf("while in\n");
	int smoke=0; 
	int fire=0;
	    
	if(firecheck==1)
	{
		fire=1;
	}
	else if(smokecheck==1)
	{
		smoke=1;
	}
	printf("%d %d \n",firecheck, smokecheck);

	if(fire==1){ //when fire sensor is on
		printf("Turnning on BUZZER_ver1\n");
		for(int k=0;k<30;k++)
			{
				int i=0;
				while(i<200){
					ioctl(buz_fd, IOCTL_BUZZER_ON, 0);
					usleep(1);   
					ioctl(buz_fd, IOCTL_BUZZER_OFF, 0);
					usleep(1);
					i++;
				}  
			}
	}
	  
	if(smoke==1){ 
		printf("Turnning on BUZZER_ver2\n");
		 
		for(int k=0;k<3;k++)
			{
				int i=0;
				while(i<200){
					ioctl(buz_fd, IOCTL_BUZZER_ON, 0);
					usleep(1);   	
					ioctl(buz_fd, IOCTL_BUZZER_OFF, 0);
					usleep(1);			
					i++;
				}
			usleep(500000);
			}
	}
		
   close(buz_fd);
   printf("close success!\n");
   
   return;
}



int main(int argc, char** argv){
   
   return 0;
}
