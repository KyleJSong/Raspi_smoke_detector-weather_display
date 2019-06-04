#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>

#define I2C_BUS		"/dev/i2c-1" // I2C Bus 
#define I2C_ADDR	0x27         // I2C slave address for the LCD module

#define LCD_RS		0 // LCD RS : 0 = command, 1 = data
#define LCD_RW		1 // LCD R/W : 0 = write, 1 = read
#define LCD_EN		2 // LCD EN : 0 = disable, 1 = enable
#define LCD_BL 		3 // LCD BL : 0 = backlight off, 1 = backlight on


char address; 
int i2cFile;

void i2c_start() {
   if((i2cFile = open(I2C_BUS, O_RDWR)) < 0) {
      printf("Error failed to open I2C bus [%s].\n", I2C_BUS);
      exit(-1);
   }
   // set the I2C slave address for all subsequent I2C device transfers
   if (ioctl(i2cFile, I2C_SLAVE, I2C_ADDR) < 0) {
      printf("Error failed to set I2C address [%s].\n", I2C_ADDR);
      exit(-1);
   }
}

void i2c_stop() { close(i2cFile); }

void i2c_send_byte(unsigned char data) {
   unsigned char byte[1];
   byte[0] = data;
   write(i2cFile, byte, sizeof(byte)); 
 
   usleep(1000);
}

void i2c_command(unsigned char command){
	
	unsigned char control[2] = {0x00, 0x00};
	control[0] = command & 0xF0;
	control[1] = (command & 0x0F) << 4;
	
	usleep(40);
	
	for(int i = 0; i < 2 ; i ++){
	
		control[i] &= ~(1 << LCD_BL); // BL = 0
		control[i] &= ~(1 << LCD_RS); // RS = 0
		control[i] &= ~(1 << LCD_RW); // RW = 0
		control[i] |= (1 << LCD_EN); // EN = 1
	
		i2c_send_byte(control[i]);
	
		control[i] &= ~(1 << LCD_EN); // EN = 0
	
		i2c_send_byte(control[i]);
	}
	
}

void i2c_data(unsigned char data){
	
	unsigned char control[2] = {0x00, 0x00};
	control[0] = data & 0xF0;
	control[1] = (data & 0x0F) << 4;
	
	usleep(40);
	
	for(int i = 0; i < 2 ; i ++){
	
		control[i] |= (1 << LCD_BL); // BL = 1
		control[i] |= (1 << LCD_RS); // RS = 1
		control[i] &= ~(1 << LCD_RW); // RW = 0
		control[i] |= (1 << LCD_EN); // EN = 1
	
		i2c_send_byte(control[i]);
	
		control[i] &= ~(1 << LCD_EN); // EN = 0
	
		i2c_send_byte(control[i]);
	}
}

void lcd_display_init(){

   usleep(15000);             
   i2c_send_byte(0b00110100); // <COMMAND/WRITE> lcd display init
   i2c_send_byte(0b00110000); 
   usleep(4100);
   i2c_send_byte(0b00110100); 
   i2c_send_byte(0b00110000); 
   usleep(100);               
   i2c_send_byte(0b00110100); 
   i2c_send_byte(0b00110000); // <COMMAND/WRITE> 8-bit mode init
   usleep(4100);             
   i2c_send_byte(0b00100100); 
   i2c_send_byte(0b00100000); // <COMMAND/WRITE> switched to 4-bit mode

   i2c_command(0x28); // <COMMAND/WRITE> configuring function set. Data bus = 0x28(D3 : 2 lines, D2 : 5X8 char)

   i2c_command(0x08); // <COMMAND/WRITE> display off. Data bus = 0x08 (D2 : display off, D1 : cusror off, D0 : cusror blink)

   i2c_command(0b00001110); // <COMMAND/WRITE> display on. Data bus = 0x0E (D3 : 1, D2 : display on, D1 : cursor on, D0 : cursor blink)
   
}

unsigned char to_hex_ascii(char input){
	
	unsigned char hex_bin = 0x00;
	hex_bin = 0xFF & input;
	
	return hex_bin;
    
	}
	
void lcd_clear(){
	i2c_command(0x01); // <COMMAND/WRITE> display clear, cursor home. Data bus = 0x01;
}

void lcd_set_cursor(int row, int col){
	unsigned char cursor_com = 0x00;
	cursor_com = 0x80 | (col + row * 0x40);
	i2c_command(cursor_com);
}
	
void lcd_print(char* input, int row, int col){
	
	unsigned char hex_bin = 0x00;
	
	i2c_command(0x06); // <COMMAND/WRITE> set cursor direction. print left to right.
	lcd_set_cursor(row, col);

	for(int i = 0; i < strlen(input); i++){
		hex_bin = to_hex_ascii(input[i]); // convert input char in string to hex ASCII 
		i2c_data(hex_bin); // <DATA/WRITE> write input to LCD
	}
}

/*
void main() { 
   i2c_start(); 
   
   lcd_display_init();

   while(1){
	   input[0] = '\0';
	   printf("input : \n");
	   scanf("%s", input);
	   printf("input mode 0 / 1 /2: \n");
	   scanf("%d", &input_mode);
	   lcd_print(input, input_mode);
	   }
	

   sleep(1);

   i2c_stop(); 
}
*/
