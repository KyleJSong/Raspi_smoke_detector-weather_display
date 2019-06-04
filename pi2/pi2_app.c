#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <string.h>

#include <stdint.h>

#define HYGRO_PATH	"/dev/hygro_dev"

#define MSG_MAX		17
#define TEMP_MAX	10	


int data[4] = {-1, -1, -1, -1};

typedef struct{
	char temperature_msg[MSG_MAX];
	char humidity_msg[MSG_MAX];
} TEMP_N_HUMID;

typedef struct{
	char dust_msg[MSG_MAX];
	char dust_density[TEMP_MAX];
} DUST_INFO;

void get_temperature(char temperature_msg[]){
	char temp[TEMP_MAX];
	
	strcpy(temperature_msg,"tempr : ");
	sprintf(temp, "%d", data[2]);
	strcat(temperature_msg, temp);
	strcat(temperature_msg, ".");
	sprintf(temp, "%d", data[3]);
	strcat(temperature_msg, temp);
	strcat(temperature_msg, " C");
}
	
void get_humidity(char humidity_msg[]){
	char temp[TEMP_MAX];
	
	strcpy(humidity_msg, "humid : ");
	sprintf(temp, "%d", data[0]);
	strcat(humidity_msg, temp);
	strcat(humidity_msg, ".");
	sprintf(temp, "%d", data[1]);
	strcat(humidity_msg, temp);
	strcat(humidity_msg, " %");
	}



void *t_get_temp_n_humid(void *temp_n_humid){
	
	int fd= open(HYGRO_PATH,O_RDWR);
	
	if(fd <0)
	{
		printf("%d \n", fd);
	}
	
	TEMP_N_HUMID *my_temp_n_humid = (TEMP_N_HUMID *)temp_n_humid;
	
	while(1){	
		read(fd, data,sizeof(int)*4);
		if(data[0]==-1){
			//printf("no data input\n");
			continue;
		}
		
		get_temperature(my_temp_n_humid->temperature_msg);
		get_humidity(my_temp_n_humid->humidity_msg);
		
		//printf("tmperature = %d.%d C humidity = %d.%d %%\n", data[2], data[3], data[0],data[1]);
		
		sleep(1);
		
	}
}

void *t_get_dust(void *dust_info){
	
	DUST_INFO *my_dust_info = (DUST_INFO *)dust_info;

	printf("into loop!\n");
	get_dust_density(my_dust_info->dust_density, my_dust_info->dust_msg);
	
}

int main(void)
{
	TEMP_N_HUMID *temp_n_humid;
	temp_n_humid = (TEMP_N_HUMID *)malloc(sizeof(TEMP_N_HUMID));
	DUST_INFO *dust_info;
	dust_info = (DUST_INFO *)malloc(sizeof(DUST_INFO));
	
	pthread_t p_thread[2];
	int thr_id;
	
	printf("start\n");
	
	i2c_start(); 
   
	lcd_display_init();	
	lcd_clear();
	printf("lcd init! \n");
	
	thr_id = pthread_create(&p_thread[0], NULL, t_get_temp_n_humid, (void *)temp_n_humid);
	 if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
    
	thr_id = pthread_create(&p_thread[1], NULL, t_get_dust, (void *)dust_info);
	if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
    
    sleep(10);
	
	while(1){

		lcd_clear();
		lcd_print(temp_n_humid->temperature_msg, 0, 0);
		lcd_print(temp_n_humid->humidity_msg, 1, 0);
	
		sleep(5);		
		
		lcd_clear();
		lcd_print(dust_info->dust_msg, 0, 0);
		lcd_print("TEAM11 ^_____^ ", 1, 0);
		
		sleep(5);
	}

    i2c_stop(); 
	
	return 0;
}
