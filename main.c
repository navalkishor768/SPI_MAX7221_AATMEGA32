/*
 * MAX7221_AT32.c
 *
 * Created: 18-Aug-22 8:54:35 PM
 * Author : admin
 */ 
#undef	F_CPU
#define F_CPU	16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <compat/deprecated.h>

#define MOSI		5
#define SCK			7
#define SS			4
#define DIGIT0		0X01
#define DIGIT1		0X02
#define DECODE		0X09
#define DECODE_R	0XFF

void spi_init()
{
	DDRB = (1<<MOSI)|(1<<SCK)|(1<<SS);  // set pin direction output
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void max7221_tx(unsigned char max7221_cmd, unsigned char dec_num)
{
	//cbi(PORTB,SS);
	PORTB &= ~(1<<SS);
	
	SPDR = max7221_cmd;			// decimal select for data write
	while(!(SPSR & (1<<SPIF)));	// wait till transfer finish
	
	SPDR = dec_num;			// write decimal data
	while(!(SPSR & (1<<SPIF)));	// wait till transfer finish
	
	//sbi(PORTB,SS);
	PORTB |= (1<<SS);
}

int main(void)
{	
	char count=0;
	
    /* Replace with your application code */
	spi_init();
	
	/*Enable Digit decode for all digits*/
	max7221_tx(DECODE,DECODE_R);
	/*Scan two 7-segments 0 and 1*/
	max7221_tx(0X0B,0X01);
	/*Turn On The Chip - Normal Operation*/
	max7221_tx(0X0C,0X01);
	/*Set The Intensity To 10/16*/
	max7221_tx(0X0A,0X08);
	
    while (1)
    {
		
		
		max7221_tx(DIGIT0,count/10);
		
		max7221_tx(DIGIT1,count%10);
		
		_delay_ms(500);
		
		count++;
		
		if(count == 99)
		{
			count = 0;
		}
		
		
    }
}

