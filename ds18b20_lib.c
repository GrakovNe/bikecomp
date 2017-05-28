#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#define TEMPERATURE_SENSOR_PORT PORTB
#define TEMPERATURE_SENSOR_PIN  PINB
#define TEMPERATURE_SENSOR_DDR  DDRB
#define TEMPERATURE_SENSOR_BIT  0

/*initialization of 1-Wire bus*/
char one_wire_init(){

	char IsAnswered;
	TEMPERATURE_SENSOR_DDR |= 1<<TEMPERATURE_SENSOR_BIT;
	TEMPERATURE_SENSOR_PORT |= 1<<TEMPERATURE_SENSOR_BIT;
	_delay_ms (1);
	TEMPERATURE_SENSOR_PORT &= ~(1<<TEMPERATURE_SENSOR_BIT);
	_delay_us(480);
	TEMPERATURE_SENSOR_PORT |= 1<<TEMPERATURE_SENSOR_BIT;
	TEMPERATURE_SENSOR_DDR &= ~(1<<TEMPERATURE_SENSOR_BIT);
	_delay_us (60);
	if (TEMPERATURE_SENSOR_PIN & (1<<TEMPERATURE_SENSOR_BIT))
	IsAnswered = 1;
	else
	IsAnswered = 0;
	_delay_us(480);
	return IsAnswered;
}

/*send byte to 1-wire*/
void one_wire_send(char byte){
	char Symbol = byte;
	TEMPERATURE_SENSOR_DDR |= (1 << TEMPERATURE_SENSOR_BIT);
	for (int i = 0; i < 8; i++){
		TEMPERATURE_SENSOR_PORT |= (1 << TEMPERATURE_SENSOR_BIT);
		_delay_us(6);
		TEMPERATURE_SENSOR_PORT &= ~(1 << TEMPERATURE_SENSOR_BIT);
		_delay_us(10);
		if (Symbol & 1){
			TEMPERATURE_SENSOR_PORT |= (1 << TEMPERATURE_SENSOR_BIT);
		}
		_delay_us(110);
		Symbol = Symbol >> 1;
	}
	TEMPERATURE_SENSOR_PORT |= (1 << TEMPERATURE_SENSOR_BIT);
	TEMPERATURE_SENSOR_DDR &= ~(1 << TEMPERATURE_SENSOR_BIT);
}

/*read byte from 1-wire*/
char one_wire_read(){
	char Symbol = 0x00;
	TEMPERATURE_SENSOR_PORT |= (1 << TEMPERATURE_SENSOR_BIT);
	_delay_us(2);
	for (int i = 0; i < 8; i++){
		TEMPERATURE_SENSOR_DDR |= (1 << TEMPERATURE_SENSOR_BIT);
		
		TEMPERATURE_SENSOR_PORT |= (1 << TEMPERATURE_SENSOR_BIT);
		_delay_us(2);
		
		TEMPERATURE_SENSOR_PORT &= ~(1<<TEMPERATURE_SENSOR_BIT);
		_delay_us(5);
		
		TEMPERATURE_SENSOR_DDR &= ~(1<<TEMPERATURE_SENSOR_BIT);
		TEMPERATURE_SENSOR_PORT |= (1 << TEMPERATURE_SENSOR_BIT);
		_delay_us(10);
		
		if (TEMPERATURE_SENSOR_PIN & (1<<TEMPERATURE_SENSOR_BIT))
		Symbol |= 0x80>>(7 - i);
		_delay_us(110);
	}
	return Symbol;
}

/*full algorithm for temperature measuring without delays for cooking new value*/
int ds_18b20_measure_temp(){
	one_wire_init();
	one_wire_send(0xCC);
	one_wire_send(0xBE);

	int Temp[2];
	Temp[0] = one_wire_read();
	Temp[1] = one_wire_read();
	int Result = ((Temp[1]<<8) + Temp[0]) >> 4;

	one_wire_init();
	one_wire_send(0xCC);
	one_wire_send(0x44);
	
	return Result;
}