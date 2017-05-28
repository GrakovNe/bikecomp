/*
 * bikecomp.c
 *
 * Created: 27.05.2017 20:18:29
 * Author : GrakovNe
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include "lcd_lib.h"
#include "ds1307_lib.h"
#include "ds18b20_lib.h"
#include <stdlib.h>

#define UP_LEFT_CORNER     0x00
#define UP_RIGHT_CORNER    0x01
#define DOWN_LEFT_CORNER   0x02
#define DOWN_RIGHT_CORNER  0x03
#define DEGREE_SYMBOL      0x04
#define THERMOSTAT_SYMBOL  0x05

#define CONFIRM_BUTTON_PIN 2
#define THROUGH_BUTTON_PIN 3
#define BUTTONS_PORT       PORTD
#define BUTTONS_PIN        PORTD
#define BUTTONS_DDR        DDRD

#define RPM_SENSOR_PORT        PORTB
#define RPM_SENSOR_PIN         PINB
#define RPM_SENSOR_DDR         DDRB
#define RPM_SENSOR_DIRECT_PIN  2
#define RPM_SENSOR_INVERSE_PIN 3

int wheel_rpm = 103;
int temperature = 16;
int averange_speed = 35;
int total_distance = 54;
char display_buffer[17];

struct {
	int hours;
	int minutes;
	} curent_time;

struct {
	int hours;
	int minutes;
	int seconds;
	} travel_time;
	
void lcd_goto_xy(int x, int y){
	int res_x = x % 15;
	int res_y = y % 2;
	
	if ((y > res_y)){
		res_x += 16;
	}
	
	LCD_Goto(res_x, res_y);
}

inline void draw_current_speed(){
	lcd_goto_xy(2, 1);
	lcd_send_symbol(UP_LEFT_CORNER);
	
	sprintf(display_buffer, " %03d KM/H ", averange_speed);
	
	lcd_send_string(display_buffer);
	lcd_send_symbol(UP_RIGHT_CORNER);
}

inline void draw_total_distance(){
	lcd_goto_xy(2, 2);
	lcd_send_symbol(DOWN_LEFT_CORNER);
	
	sprintf(display_buffer, " %06d M ", total_distance);
	
	lcd_send_string(display_buffer);
	lcd_send_symbol(DOWN_RIGHT_CORNER);
}

inline void draw_current_temperature(){
	lcd_goto_xy(10, 0);
	
	sprintf(display_buffer,"%+02d ", temperature);
	
	lcd_send_string(display_buffer);
	lcd_send_symbol(DEGREE_SYMBOL);
	lcd_send_string("C");
}

inline void draw_current_time(){
	lcd_goto_xy(0, 0);
	sprintf(display_buffer, "%02d:%02d", curent_time.hours, curent_time.minutes);
	lcd_send_string(display_buffer);
}

inline void draw_travel_time(){
	lcd_goto_xy(8,3);
	sprintf(display_buffer, "%02d:%02d:%02d", travel_time.hours, travel_time.minutes, travel_time.seconds);
	lcd_send_string(display_buffer);
}

inline void draw_wheel_rpm(){
	lcd_goto_xy(0, 3);
	sprintf(display_buffer, "%05d", wheel_rpm);
	lcd_send_string(display_buffer);
}

inline void update_stats_display(){
	draw_current_speed();
	draw_total_distance();
	draw_current_temperature();
	draw_current_time();
	draw_travel_time();
	draw_wheel_rpm();
}

inline void lcd_load_cusom_symbols(){
	char specialSymbols[6][8] = {
		{0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},  // Code for up-left corner
		{0x1F, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01},  // Code for up-right corner
		{0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x00},  // Code for down-left corner
		{0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1F, 0x00},  // Code for down-right corner
		{0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00, 0x00, 0x00},  // Code for degree symbol
		{0x04, 0x0E, 0x1F, 0x00, 0x1F, 0x0E, 0x04, 0x00}  // Code for thermostat symbol
	};
	for (int i = 0; i < 6; i++){
		display_ad_custom_symbol(specialSymbols[i], i);
	}

}

inline _init_(){
	/* initialize wh1604 */
	lcd_init();
	lcd_clear();
	lcd_load_cusom_symbols();
	
	/*initialize IO*/
	BUTTONS_DDR  &= ~((1 << CONFIRM_BUTTON_PIN) | (1 << THROUGH_BUTTON_PIN));
	BUTTONS_PORT |= (1 << CONFIRM_BUTTON_PIN | 1 << THROUGH_BUTTON_PIN);
	
	RPM_SENSOR_DDR  &= ~((1 << RPM_SENSOR_DIRECT_PIN) | (1 << RPM_SENSOR_INVERSE_PIN));
	RPM_SENSOR_PORT |= (1 << RPM_SENSOR_DIRECT_PIN | 1 << RPM_SENSOR_INVERSE_PIN);
	
	
	
	/*initialize DS1307*/
	ds_1307_init();
	ds_1307_read_time(&curent_time.hours, &curent_time.minutes, 0);
}


int main(void)
{
	_init_();
	
	one_wire_init();
	temperature = ds_18b20_measure_temp();
	
	update_stats_display();
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

