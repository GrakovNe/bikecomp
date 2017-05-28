//***************************************************************************
//  File........: lcd_lib.h
//
//  Author(s)...: Pashgan    chipenable.ru
//
//  Target(s)...: ATMega...
//
//  Compiler....: GNU Compiler Collection (GCC) 4.3.2 
//
//  Description.: Драйвер знакосинтезирующего жк дисплея
//
//  Data........: 28.10.09  
//
//***************************************************************************
#ifndef LCD_LIB_H
#define LCD_LIB_H
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

//порт к которому подключена шина данных ЖКД
#define PORT_DATA PORTA
#define PIN_DATA  PINA
#define DDRX_DATA DDRA

//порт к которому подключены управляющие выводы ЖКД
#define PORT_SIG PORTA
#define PIN_SIG  PINA
#define DDRX_SIG DDRA

//Номера выводов к которым подключены управляющие выводы ЖКД 
#define RS 0
#define RW 1
#define EN 2

//#define CHECK_FLAG_BF
#define BUS_4BIT
#define HD44780
//******************************************************************************
//макросы

/*позиционирование курсора*/
#define LCD_Goto(x,y) lcd_write_command((((y)*0x40)+(x))|128) 
#define lcd_send_symbol(sym) lcd_write_data(sym)

//прототипы функций
void lcd_init(void);//инициализация портов и жкд
void lcd_clear(void);//очистка дисплея
void lcd_write_data(unsigned char data); //выводит байт данных на жкд
void lcd_write_command(unsigned char data); //посылает команду жкд
void lcd_send_string_flash(char *str); 
void lcd_send_string(char *str);
#endif