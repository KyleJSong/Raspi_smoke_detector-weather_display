#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <wiringSerial.h>

/*
int dust_init(){
	int serial_port;

	if ((serial_port = serialOpen ("/dev/ttyS0", 9600)) < 0)	// open serial port.
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return -1;
  }

  if (wiringPiSetup () == -1)					// initializes wiringPi setup.
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return -1;
  }
  printf("s_port : %d \n", serial_port);
  
  return serial_port;
}
*/
void get_dust_msg(char dust_msg[], char dust_density[]){
	strcpy(dust_msg, "dust : ");
	strcat(dust_msg, dust_density);
	strcat(dust_msg, "mg/m3");
	//printf("dust dens : %s\n", dust_density);
	//printf("dust msg : %s\n", dust_msg);
}

void get_dust_density (char dust_density[], char dust_msg[])
{
	char dat;
	int serial_port;

	if ((serial_port = serialOpen ("/dev/ttyS0", 9600)) < 0)	// open serial port.
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
  }

  if (wiringPiSetup () == -1)					// initializes wiringPi setup.
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
  }
  //printf("s_port : %d \n", serial_port);
	
	//printf("serial port : %d \n", serial_port);
	while(1){
		if(serialDataAvail (serial_port)){
			//printf("***get dust OK***\n");
			for(int i = 0; i < 4; i++){
				dat = serialGetchar(serial_port);		/* receive character serially*/	
				if(dat == NULL){
					sleep(1);
					continue;
				}
				//printf ("%c", dat);
				fflush (stdout) ;
				serialPutchar(serial_port, dat);		/* transmit character serially on port */
				dust_density[i] = dat;
			}
		
			dust_density[4] = '\0';
			//printf("\ndust density : %s\n", dust_density);
			
			if(dust_density[0] != '.' && dust_density[1] == '.' && dust_density[2] != '.' && dust_density[3] != '.'){
				get_dust_msg(dust_msg, dust_density);
			}
			//printf("***end get dust***\n");
			dust_density[0] = '\0';
		
			for(int i = 0; i < 2; i++){
				dat = serialGetchar (serial_port);
				fflush(stdout);
				serialPutchar(serial_port, dat);
			}
			
			
			sleep(1);
		}
	}
  
}

 


