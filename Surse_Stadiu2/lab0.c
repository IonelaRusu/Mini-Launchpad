/*
 * PM - 2017
 * lab0.c
 */

#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include <avr/interrupt.h>

#define DELAY_SECVENTA		250
#define NUM_SECVENTE		6

#define MORSE_LUNG		150
#define MORSE_SCURT		50
#define MORSE_PAUZA		50

#define NUM_NOTE		8

// define 
#define NUM_LED_COLUMNS		4
#define NUM_LED_ROWS		4
#define NUM_BTN_COLUMNS		4
#define NUM_BTN_ROWS		4
#define NUM_COLORS		1
#define MAX_DEBOUNCE		3



volatile uint64_t millis_prv = 0;

void millis_init()
{
 TCCR0A = 0;
 // set timer0 with CLKio/8 prescaler
 TCCR0B = _BV(CS01) | _BV(CS00);
 // clear any TOV1 Flag set when the timer overflowed
 TIFR0 &= ~TOV0;
 // set timer0 counter initial value to 0
 TCNT0 = 0x0;
 // enable timer overflow interrupt for Timer0
 TIMSK0 = _BV(TOIE0);
 // clear the Power Reduction Timer/Counter0
 PRR &= ~PRTIM0;
}

// TIMER0 interrupt handler
ISR(TIMER0_OVF_vect)
{
 // reset the counter (overflow is cleared automatically)
 TCNT0 = (uint8_t)(0xFF - ((F_CPU/8)/1000)); // use CLKio/8 prescaler (set CS0n accordingly above)
 millis_prv++;
}

// return elapsed time in milliseconds
uint64_t millis()
{
 return millis_prv;
}

// Global variables
static int32_t next_scan;
static int8_t debounce_count[NUM_BTN_COLUMNS][NUM_BTN_ROWS];

static bool LED_buffer[NUM_LED_COLUMNS][NUM_LED_ROWS];
static int32_t next_advance;
static uint8_t led_index;


static const uint8_t btnrowpins[NUM_BTN_ROWS]       = {PB0, PB1, PB2, PB3};
static const uint8_t btncolumnpins[NUM_BTN_COLUMNS] = {PB4, PB5, PB6, PB7};
static const uint8_t colorpins[NUM_LED_ROWS]        = {PA0, PA1, PA2, PA3};
static const uint8_t ledcolumnpins[NUM_LED_COLUMNS]  = {PA4, PA5, PA6, PA7};

void setup(void) {
	
		/* Daca butonul este apasat. */
		//if ((PINC & (1 << PA2)) == 0)
	/* Coloane Butoane */
		DDRB |= (1<< PB4);  //iesire
		PORTB |= (1<< PB4);
		DDRB |= (1<< PB5);  
		PORTB |= (1<< PB5);
		DDRB |= (1<< PB6);  
		PORTB |= (1<< PB6);
		DDRB |= (1<< PB7);  
		PORTB |= (1<< PB7); //aprins

	
	/* Linii Butoane */
		DDRB &= ~(1<< PB0);  //intrare
		PORTB |= (1<< PB0);
		DDRB &= ~(1<< PB1);  
		PORTB |= (1<< PB1);
		DDRB &= ~(1<< PB2);  
		PORTB |= (1<< PB2);
		DDRB &= ~(1<< PB3);  
		PORTB |= (1<< PB3); //aprins

	/* Linii LED-uri */
		
		 DDRA |= (1 << PA0); //iesire
		 PORTA &= ~(1<< PA0);
		 
		 DDRA |= (1<< PA1);
		 PORTA &= ~(1<< PA1); //stins
		 
		 DDRA |= (1<< PA2);
		 PORTA &= ~(1<< PA2);
		 
		 DDRA |= (1<< PA3);
		 PORTA &= ~(1<< PA3);
	 
	 /* Coloane LED-uri*/
		DDRA |= (1 << PA4); //iesire
		 PORTA |= (1<< PA4);
		 
		 DDRA |= (1<< PA5);
		 PORTA |= (1<< PA5); //aprins
		
		 DDRA |= (1<< PA6);
		 PORTA |= (1<< PA6);
		 
		 DDRA |= (1<< PA7);
		 PORTA |= (1<< PA7);
	// Initialize the debounce counter array
	
	//prima etapa
	PORTA |= (1 << PA0);//pronim linia
	PORTA |= (1 << PA3);

	PORTA &=~(1 << PA4);
	PORTA &=~(1 << PA5);
	PORTA &=~(1 << PA6);
	PORTA &=~(1 << PA7);
	

	 _delay_ms(1500);
	 
	PORTA &=~(1 << PA0);
	PORTA &=~(1 << PA3);
	
	PORTA |=(1 << PA4);
	PORTA |=(1 << PA5);
	PORTA |=(1 << PA6);
	PORTA |=(1 << PA7);
	
	
		 _delay_ms(1500);
		 
	// a doua etapa
	
	PORTA |= (1 << PA1);//pronim linia
	PORTA |= (1 << PA2);
		 

	PORTA &=~(1 << PA5);
	PORTA &=~(1 << PA6);
	
	 _delay_ms(1500);
	
	PORTA &=~(1 << PA1);
	PORTA &=~(1 << PA2);
	
		
	PORTA |=(1 << PA5);
	PORTA |=(1 << PA6);
	
	  
	 
}

static uint8_t current = 0;

static void scan()
{    uint8_t val;
    uint8_t i, j;

	//USART0_print(" in scan \n");
	PORTA &= ~(1<< ledcolumnpins[current]);
	PORTB &= ~(1<< btncolumnpins[current]);
/*
	
	if(LED_buffer[current][0]){
	USART0_print(" * \n");
		PORTA |= (1<< PA0);
	}
	if(LED_buffer[current][1]){
		PORTA |= (1<< PA1);
			USART0_print(" ** \n");
	}
	if(LED_buffer[current][2]){
		PORTA |= (1<< PA2);
			USART0_print(" *** \n");
	}
	if(LED_buffer[current][3]){
		PORTA |= (1<< PA3);
			USART0_print(" **** \n");
	}
*/

// pause a moment
  _delay_ms(5);

 
		//daca butonul e apasat
		if ((PINB & (1<<PB0))==0)
		{
			PORTA |= (1 << PA0);
			//USART0_print(" A0\n");
			PORTA &=~(1 << ledcolumnpins[current]);
		}
		else{
				// otherwise, button is released
				//USART0_print("N0 ---?\n");
			  
					//PORTA &= ~(1<< PA0);
				//	PORTA |= (1<<ledcolumnpins[current]);
		}

		if ((PINB & (1<<PB1))==0)
		{
				//USART0_print(" A1****?\n");

				PORTA |= (1 << PA1);
				PORTA &=~(1 << ledcolumnpins[current]);
		}
		else{
				// otherwise, button is released
				//USART0_print("N1 ---?\n");
				//PORTA &= ~(1<< PA1);
				//PORTA |= (1<<ledcolumnpins[current]);
		
		}

		//daca butonul e apasat
		if ((PINB & (1<<PB2))==0)
		{
			//USART0_print(" A2\n");
			
			PORTA |= (1<< PA2);
			PORTA &=~(1 << ledcolumnpins[current]);

		}
		else{
				// otherwise, button is released
			//USART0_print("N2 ---?\n");
			//PORTA &= ~(1<< PA2);
			//PORTA |= (1<<ledcolumnpins[current]);

		}
	

		//daca butonul e apasat
		if ((PINB & (1<<PB3))==0)
		{
			//USART0_print(" A3\n");
			PORTA |= (1 << PA3);
			PORTA &=~(1 << ledcolumnpins[current]);
		}
		else{
				// otherwise, button is released
			//USART0_print("N3 ---?\n");
			//PORTA &= ~(1<< PA3);
			//PORTA |= (1<<ledcolumnpins[current]);
		}
			  
			
  
 
  
	_delay_ms(5); 

	/*se aprinde o linie a butonului apasat si vrem sa se aprinda doar butonul curent*/
	PORTB |= (1 << btncolumnpins[current]);
	PORTA |= (1 << ledcolumnpins[current]);

	/* fara astea se aprind linii si raman aprinse mereu*/
	PORTA &= ~(1<< PA0);
	PORTA &= ~(1<< PA1);
	PORTA &= ~(1<< PA2);
	PORTA &= ~(1<< PA3);
	
	
		
	current++;
	if (current >= NUM_LED_COLUMNS )
	{
		current = 0;
	}

}

int main(void) {
	
	//USART0_init();
	//USART0_print("Starting Setup... \n");

	setup();

	//next_advance =  millis() + 1000;
	//led_index = 0;
	/*
		DDRA &= ~(1<< PA0);
	for(uint8_t i = 0; i < NUM_LED_COLUMNS; i++)
	{
		for(uint8_t j = 0; j < NUM_LED_ROWS; j++)
		{
		  LED_buffer[i][j] = 0;
		}
	}
	*/
/*	DDRA |= (15<< PA0);
	PORTA |= (15 << PA0);
	DDRA |= (1 << PA4);
	PORTA |= (1 << PA4);
	DDRB &= ~(1<<PB0);
	PORTB |= (1<<PB0);
	*/
	//USART0_print(" Setup...Complete\n");

	while (1){
	
	
		scan();
		 //if(millis() >= next_scan)
		///  {
		//	next_scan = millis()+1;
		
			//PORTA |=(1<<PA0);
			/*USART0_print(" dupa scan \n");
			 if( millis() >= next_advance)
			  {
				next_advance = millis()+1000;

				LED_buffer[led_index/NUM_LED_COLUMNS][led_index%NUM_LED_COLUMNS] = false;
				led_index++;
				led_index %= (NUM_LED_COLUMNS * NUM_LED_ROWS);
				LED_buffer[led_index/NUM_LED_COLUMNS][led_index%NUM_LED_COLUMNS] = true;
			  }
		
			
		 // }*/
		 
	/*	 if ((PINB & (1<<PB0))==0)
		{
			PORTA &= ~(1 << PA0);
		//USART0_print(" A0\n");
		}
		else
		{
			PORTA |= (1<<PA0);
		}
		*/
		 
	}

	return 0;
}
