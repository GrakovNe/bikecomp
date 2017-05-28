#include <avr/io.h>
#include <util/delay.h>
#include "ds1307_lib.h"

void ds_1307_init(void) {
	TWBR = 2;
	TWSR = (1 << TWPS1)|(1 << TWPS0); 
	TWCR |= (1 << TWEN); 
	
	uint8_t temp;
	ds_1307_read(0x00,&temp);
	temp &= ~(1 << 7); 
	ds_1307_write(0x00,temp);
}

void i2c_bus_start(void) {
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTA);
	while(!(TWCR & (1 << TWINT)));
}

void i2c_bus_stop(void) {
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTO); 
	while(TWCR & (1 << TWSTO)); 
}


uint8_t i2c_bus_send_byte(uint8_t data) {
	TWDR = data; 
	TWCR = (1 << TWEN)|(1 << TWINT); 
	while(!(TWCR & (1 << TWINT))); 
	
	return ((TWSR & 0xF8) == 0x18 || (TWSR & 0xF8) == 0x28 || (TWSR & 0xF8) == 0x40);
}


uint8_t i2c_bus_read_byte(uint8_t *data,uint8_t ack) {
	if(ack) {	
		TWCR |= (1 << TWEA);
	}
	else {
		TWCR &= ~(1 << TWEA);
	}
	
	TWCR |= (1 << TWINT);
	while(!(TWCR & (1 << TWINT))); 
	
	if((TWSR & 0xF8) == 0x58 || (TWSR & 0xF8) == 0x50){
		*data = TWDR; 
		return 1;
	}
	else
		return 0; 
}

uint8_t ds_1307_read(uint8_t address,uint8_t *data)
{
	uint8_t res; 
	i2c_bus_start(); 
	res = i2c_bus_send_byte(0b11010000); 
	if(!res)    return 0; 
	
	res = i2c_bus_send_byte(address);
	if(!res)    return 0; 
	i2c_bus_start(); 
	res = i2c_bus_send_byte(0b11010001); 
	if(!res)    return 0; 
	
	res = i2c_bus_read_byte(data,0);
	if(!res)    return 0; 
	i2c_bus_stop(); 
	return 1;
}

uint8_t ds_1307_write(uint8_t address,uint8_t data)
{
	uint8_t res; 
	i2c_bus_start(); 
	res = i2c_bus_send_byte(0b11010000); 
	if(!res)    return 0; 
	
	res = i2c_bus_send_byte(address);
	if(!res)    return 0; 
	res = i2c_bus_send_byte(data); 
	if(!res)    return 0; 
	i2c_bus_stop(); 
	return 1;
}

void ds_1307_read_time(int *hours, int *minutes, int *seconds){
	unsigned char temp;
	ds_1307_read(0x00,&temp); 
	*seconds = (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);
	ds_1307_read(0x01,&temp); 
	*minutes = (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);
	ds_1307_read(0x02,&temp); 
	*hours = (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);
}


void ds_1307_write_time(int hours, int minutes, int seconds){
	uint8_t temp;
	
	temp = ((hours/10) << 4)|(hours%10);
	ds_1307_write(0x02, temp);
	
	temp = ((minutes/10) << 4)|(minutes%10);
	ds_1307_write(0x01, temp);
	
	temp = ((seconds/10) << 4)|(seconds%10);
	ds_1307_write(0x00, temp);
}
