#include<avr/io.h>
#define F_CPU 16000000UL // internal clock
#include <util/delay.h>
#include<string.h>

void lcd_cmd (char cmd);
void lcd_data (char dat);



int main (void)
{
char let[] ={ " ashin "} ;
int i;

DDRB = 0xFF;  

//  intialising lcd 
/* require 2 line display
   to improve sucess rates proving 2 one line also */

lcd_cmd(0x33);  // one line
lcd_cmd(0x32);  
lcd_cmd(0x28);  // 4 bit mode
lcd_cmd(0x06);  // auto addressing
lcd_cmd(0x0C);  // cursor off
lcd_cmd(0x01);  // clear display
lcd_cmd(0x80);  // first row first location



while (1) // non recuring loop
{

for (i=0;i<strlen(let);i++)
{
lcd_data(let[i]);  // data to show on lcd
}


}

}

void lcd_cmd (char cmd) // deals with command register
{
/*.............CONNECTION.............. 

   	PORT C - RS RW E
			  7  6 5
			  
			  RS 0 - command register
			  RS 1 - data 	 register

	PORT D - D0 D1 .... D7    8 bit data
			
    0x 'x' for hexadecimal
*/

PORTB &=(~0x80); // setting RS 0 for command register

	/*  setting the first bit low , need for providing 
	
	80 - 1000 0000 
	~    0111 1111
	&    0   	- 0XXX XXXX
	     1		- 0XXX XXXX
	
	find hex with high on the pin needed.	
	do &=(~  to set that pin as low
	do |=        				high
	*/

PORTB &=(~0x40); // X0XX XXXX  setting ~write
				 
PORTB = (PORTD & 0xF0)|((cmd>>4) & 0x0F); // send higher nibble
PORTB |= 0x20; 		// enable high
PORTB &= (~0x20);	// enable low
_delay_ms(10);		// delay for slow device


PORTB = (PORTD & 0xF0)|(cmd & 0x0F); 

PORTB |= 0x20; 		// enable high
PORTB &= (~0x20);	// enable low
_delay_ms(10);		// delay for slow device




}

void lcd_data (char dat)  // Data Register
{
PORTB |= 0x80;    // high RS for data reg
PORTB &= (~0x40); // low as writing onto lcd screen 


PORTB = (PORTD & 0xF0)|((dat>>4) & 0x0F); // send higher nibble
PORTB |= 0x20; 		// enable high
PORTB &= (~0x20);	// enable low
_delay_ms(10);		// delay for slow device


PORTB = (PORTD & 0xF0)|(dat & 0x0F); 

PORTB |= 0x20; 		// enable high
PORTB &= (~0x20);	// enable low
_delay_ms(10);		// delay for slow device

}




