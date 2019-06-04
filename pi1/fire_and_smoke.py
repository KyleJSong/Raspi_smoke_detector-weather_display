import spidev, time
from ctypes import *

adder = CDLL('./adder.so')

spi = spidev.SpiDev()
spi.open(0,0)
spi.max_speed_hz=1000000

def analog_read_fire(channel):
	r=spi.xfer2([1,(8+channel)<<4,0])
	adc_out = ((r[1]&3)<<8)+r[2]
	return adc_out

def analog_read_smoke(channel):
	r=spi.xfer2([1,(8+channel)<<4,0])
	adc_out = ((r[1]&3)<<8)+r[2]
	return adc_out

store=0;
sum=0;

for i in range(100):
	sum=sum+analog_read_fire(0);
	time.sleep(1*(10**-2))

store=sum/100;
print("avg:");
print(store);

sum=0;

for i in range(100):
	sum=sum+analog_read_smoke(1);
	time.sleep(1*(10**-2))
     
store2=sum/100;



while True:
    
	startfire =0;
	startsmoke=0;
	fire=analog_read_fire(0);
 
	if fire < (store-20):
		startfire=1;
        
	smoke=analog_read_smoke(1);
    
	if smoke > (store2+5):
		startsmoke=1;
	adder.func(startfire,startsmoke);
	#time.sleep(1)
    

