/* Automatic Water Level Controller
 * Microcontroller : AT Tiny 2313
 * TINY_2313_LCD.c
 *
 * Created: 17/06/17 09:12:36
 *  Author: Ashin Basil John
 */ 


#include<avr/io.h>
#define F_CPU 16000000UL // internal clock
#include <util/delay.h>
#include<string.h>

void lcd_cmd (char cmd);
void lcd_data (char dat);
void printlcd(const char *CPtr);

#define safety (PIND & 0x08) // input from safety probe  
#define high (PIND & 0x10) // input from high probe
#define low (PIND & 0x20) // input from low probe

#define pump_on (PORTD |= _BV(PORTD1))   //switch ON PUMP
#define pump_off (PORTD &= ~_BV(PORTD1))  //switch OFF PUMP

#define buzz_on (PORTD |= _BV(PORTD0))   //switch ON Buzzer
#define buzz_off (PORTD &= ~_BV(PORTD0))  //switch OFF Buzzer



int main (void)
{
	

	
	
int i;

DDRB = 0xFF; // for LCD module
DDRD = 0xC7; // for buzzer and probes

lcd_cmd(0x33);  // one line
lcd_cmd(0x32);  
lcd_cmd(0x28);  // 4 bit mode
lcd_cmd(0x06);  // auto addressing
lcd_cmd(0x0C);  // cursor off
lcd_cmd(0x01);  // clear display
lcd_cmd(0x80);  // first row first location


printlcd("PUMP CONTROLLER");
lcd_cmd(0xC0); 
printlcd("SYSTEM READY");

buzz_on;
_delay_ms(50);
buzz_off;
_delay_ms(50);
buzz_on;
_delay_ms(50);
buzz_off;

int error=0;
int low_count = 0;
int high_count = 0;
int filling_tank = 0;



while (1)
{


	if (safety==0){
		pump_off;
		error=1;
		filling_tank=0;
		
		lcd_cmd(0x33);  // one line
		lcd_cmd(0x32);
		lcd_cmd(0x28);  // 4 bit mode
		lcd_cmd(0x06);  // auto addressing
		lcd_cmd(0x0C);  // cursor off
		lcd_cmd(0x01);  // clear display		
		
		lcd_cmd(0x80);  
		printlcd("PROBE ERROR     ");
		lcd_cmd(0xC0);
		printlcd("PUMP OFF     ");
	
	}
	else if (high==0){
		high_count++;
		_delay_ms(10);
		
		if (high_count>5){
			high_count = 0;
			pump_off;
			
			lcd_cmd(0x33);  // one line
			lcd_cmd(0x32);
			lcd_cmd(0x28);  // 4 bit mode
			lcd_cmd(0x06);  // auto addressing
			lcd_cmd(0x0C);  // cursor off
			
			lcd_cmd(0x01);  // clear display
			lcd_cmd(0x80);  // first row first location
			printlcd("PUMP CONTROLLER");
			lcd_cmd(0xC0);
			printlcd("TANK FULL     ");
			_delay_ms(300);
			lcd_cmd(0xC0);
			printlcd("PUMP OFF     ");
			filling_tank=0;
			buzz_off;
		}
	}	
	
	else if (high==1){
			high_count=0;
		}
	
		
	else if (low==0){
		low_count++;
		_delay_ms(10);
		
		if (low_count>5){
			pump_on;
			low_count=0;
			filling_tank=1;
			
			lcd_cmd(0x33);  // one line
			lcd_cmd(0x32);
			lcd_cmd(0x28);  // 4 bit mode
			lcd_cmd(0x06);  // auto addressing
			lcd_cmd(0x0C);  // cursor off
			
			lcd_cmd(0x01);  // clear display
			lcd_cmd(0x80);  // first row first location
			printlcd("PUMP CONTROLLER");
			lcd_cmd(0xC0);
			printlcd("TANK EMPTY     ");
			_delay_ms(300);
			lcd_cmd(0xC0);
			printlcd("PUMP ON      ");
		}
	}

	else if (low==1){
		low_count=0;
	}
	
	else if(filling_tank==1){

		lcd_cmd(0xC0);
		printlcd("FILLING TANK     ");
		buzz_on;
		_delay_ms(20);
		buzz_off;
	}

}
}

void lcd_cmd (char cmd) // deals with command register
{


PORTB &=(~0x80); // setting RS 0 for command register



PORTB &=(~0x40); // X0XX XXXX  setting ~write
				 
PORTB = (PORTB & 0xF0)|((cmd>>4) & 0x0F); // send higher nibble
PORTB |= 0x20; 		// enable high
PORTB &= (~0x20);	// enable low
_delay_ms(10);		// delay for slow device


PORTB = (PORTB & 0xF0)|(cmd & 0x0F); 

PORTB |= 0x20; 		// enable high
PORTB &= (~0x20);	// enable low
_delay_ms(10);		// delay for slow device




}

void lcd_data (char dat)  // Data Register
{
PORTB |= 0x80;    // high RS for data reg
PORTB &= (~0x40); // low as writing onto lcd screen 


PORTB = (PORTB & 0xF0)|((dat>>4) & 0x0F); // send higher nibble
PORTB |= 0x20; 		// enable high
PORTB &= (~0x20);	// enable low
_delay_ms(10);		// delay for slow device


PORTB = (PORTB & 0xF0)|(dat & 0x0F); 

PORTB |= 0x20; 		// enable high
PORTB &= (~0x20);	// enable low
_delay_ms(10);		// delay for slow device

}

void printlcd(const char *CPtr)
{
	while(*CPtr != '\0'){
		lcd_data(*CPtr);
		++CPtr;
	}
}



