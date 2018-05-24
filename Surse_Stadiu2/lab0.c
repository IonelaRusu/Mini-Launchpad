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



static const uint8_t btncolumnpins[NUM_BTN_COLUMNS] = {PB0, PB1, PB2, PB3};
static const uint8_t btnrowpins[NUM_BTN_ROWS]       = {PB4, PB5, PB6, PB7};
static const uint8_t ledcolumnpins[NUM_LED_COLUMNS] = {PC0, PC1, PC2, PC3};
static const uint8_t colorpins[NUM_LED_ROWS]        = {PC4, PC5, PC6, PC7};

/* Aprinde led-ul PC0 la apasarea butonului PB2. */
void setup(void) {
	
		/* Daca butonul este apasat. */
		//if ((PINC & (1 << PC2)) == 0)
	
	
	/* Coloane leduri */
		//pinMode(btncolumnpins[i], OUTPUT);
		 DDRB &= ~(1<< PC0);
		//digitalWrite(btncolumnpins[i], HIGH); 
		 PORTC |= (1<< PC0);
		 
		 DDRC &= ~(1<< PC1);
		 PORTC |= (1<< PC1);
		 
		 DDRC &= ~(1<< PC2);
		 PORTC |= (1<< PC2);
		 
		 DDRC &= ~(1<< PC3);
		 PORTC |= (1<< PC3);
		 
	/* Coloane Butoane */
		 DDRB &= ~(1<< PB0);
		 PORTB |= (1<< PB0);
		 
		 DDRB &= ~(1<< PB1);
		 PORTB |= (1<< PB1);
		 
		 DDRB &= ~(1<< PB2);
		 PORTB |= (1<< PB2);
		 
		 DDRB &= ~(1<< PB3);
		 PORTB |= (1<< PB3);
	
	/* Linii Butoane */
		DDRB |= (1<< PB4);  
		PORTB |= (1<< PB4);
		DDRB |= (1<< PB5);  
		PORTB |= (1<< PB5);
		DDRB |= (1<< PB6);  
		PORTB |= (1<< PB6);
		DDRB |= (1<< PB7);  
		PORTB |= (1<< PB7);

	/* Linii LED-uri */
		 DDRC &= ~(1<< PC4);
		//digitalWrite(btncolumnpins[i], HIGH);
		 PORTC &= ~(1<< PC4);
		 
		 DDRC &= ~(1<< PC5);
		 PORTC &= ~(1<< PC5);
		 
		 DDRC &= ~(1<< PC6);
		 PORTC &= ~(1<< PC6);
		 
		 DDRC &= ~(1<< PC7);
		PORTC &= ~(1<< PC7);
	
	// Initialize the debounce counter array
	for (uint8_t i = 0; i < NUM_BTN_COLUMNS; i++)
	{
		for (uint8_t j = 0; j < NUM_BTN_ROWS; j++)
		{
			debounce_count[i][j] = 0;
		}
	}
}


static void scan()
{
	static uint8_t current = 0;
    uint8_t val;
    uint8_t i, j;

	USART0_print(" in scan \n");
	
	
	
	PORTB &= ~(1 << btncolumnpins[current]);
	PORTC &= ~(1 << ledcolumnpins[current]);
	
	if(LED_buffer[current][0]){
	USART0_print(" * \n");
		PORTC |= (1<< PC4);
	}
	if(LED_buffer[current][1]){
		PORTC |= (1<< PC5);
			USART0_print(" ** \n");
	}
	if(LED_buffer[current][2]){
		PORTC |= (1<< PC6);
			USART0_print(" *** \n");
	}
	if(LED_buffer[current][3]){
		PORTC |= (1<< PC7);
			USART0_print(" **** \n");
	}

	
/*	PORTB |= (1 << current);
	
	PORTB &= ~(1<< PB0);
	PORTB &= ~(1<< PB1);
	PORTB &= ~(1<< PB2);
	PORTB &= ~(1<< PB3);
		
	current++;
	if (current >= NUM_LED_COLUMNS)
  {
    current = 0;
  }*/


// pause a moment
  _delay_ms(5);
  
   // Read the button inputs
		j = 0;
  		USART0_print(" btn:PB4\n");
		//daca butonul e apasat
		if ((PINB & (1<<PB4))==0)
		{
		USART0_print(" E apasat PB4****?\n");
		  if ( debounce_count[current][j] < MAX_DEBOUNCE)
		  {
				debounce_count[current][j]++;
				if ( debounce_count[current][j] == MAX_DEBOUNCE )
				{
				  USART0_print("Key Up\n" );
				  //USART0_print((char*)(current * NUM_BTN_ROWS) + j);

				  // Do whatever you want to with the button press here:
				  // toggle the current LED state
				  //LED_buffer[current][j] = !LED_buffer[current][j];	
				 LED_buffer[current][j] = !LED_buffer[current][j];
				
				
				}
			}
		}
		else{
				// otherwise, button is released
				USART0_print("NU E apasat PB4 ---?\n");
			  if ( debounce_count[current][j] > 0)
			  {
				debounce_count[current][j]--;
				if ( debounce_count[current][j] == 0 )
				{
					USART0_print("Key not pressed Up \n");
				  // If you want to do something when a key is released, do it here:

				}
			  }
			}
		j = 1;
		USART0_print(" btn:PB5\n");
		//daca butonul e apasat
		if ((PINB & (1<<PB5))==0)
		{
		USART0_print(" E apasat PB5****?\n");
		  if ( debounce_count[current][j] < MAX_DEBOUNCE)
		  {
				debounce_count[current][j]++;
				if ( debounce_count[current][j] == MAX_DEBOUNCE )
				{
				  USART0_print("Key Up\n" );
				  //USART0_print((char*)(current * NUM_BTN_ROWS) + j);

				  // Do whatever you want to with the button press here:
				  // toggle the current LED state
				  //LED_buffer[current][j] = !LED_buffer[current][j];	
				   LED_buffer[current][j] = !LED_buffer[current][j];
				}
			}
		}
		else{
				// otherwise, button is released
				USART0_print("NU E apasat PB5 ---?\n");
			  if ( debounce_count[current][j] > 0)
			  {
				debounce_count[current][j]--;
				if ( debounce_count[current][j] == 0 )
				{
					USART0_print("Key not pressed Up \n");
				  // If you want to do something when a key is released, do it here:

				}
			  }
			}
			j = 2;
		USART0_print(" btn:PB6\n");
		//daca butonul e apasat
		if ((PINB & (1<<PB6))==0)
		{
		USART0_print(" E apasat PB6****?\n");
		  if ( debounce_count[current][j] < MAX_DEBOUNCE)
		  {
				debounce_count[current][j]++;
				if ( debounce_count[current][j] == MAX_DEBOUNCE )
				{
				  USART0_print("Key Up\n" );
				  //USART0_print((char*)(current * NUM_BTN_ROWS) + j);

				  // Do whatever you want to with the button press here:
				  // toggle the current LED state
				  //LED_buffer[current][j] = !LED_buffer[current][j];
					LED_buffer[current][j] = !LED_buffer[current][j];	
				}
			}
		}
		else{
				// otherwise, button is released
				USART0_print("NU E apasat PB6 ---?\n");
			  if ( debounce_count[current][j] > 0)
			  {
				debounce_count[current][j]--;
				if ( debounce_count[current][j] == 0 )
				{
					USART0_print("Key not pressed Up \n");
				  // If you want to do something when a key is released, do it here:

				}
			  }
			}
			j = 3;
		USART0_print(" btn:PB7\n");
		//daca butonul e apasat
		if ((PINB & (1<<PB7))==0)
		{
		USART0_print(" E apasat PB7**9uf9?\n");
		  if ( debounce_count[current][j] < MAX_DEBOUNCE)
		  {
				debounce_count[current][j]++;
				if ( debounce_count[current][j] == MAX_DEBOUNCE )
				{
				  USART0_print("Key Up\n" );
				  //USART0_print((char*)(current * NUM_BTN_ROWS) + j);

				  // Do whatever you want to with the button press here:
				  // toggle the current LED state
				  //LED_buffer[current][j] = !LED_buffer[current][j];	
				   LED_buffer[current][j] = !LED_buffer[current][j];
				}
			}
		}
		else{
				// otherwise, button is released
				USART0_print("NU E apasat PB7 ---?\n");
			  if ( debounce_count[current][j] > 0)
			  {
				debounce_count[current][j]--;
				if ( debounce_count[current][j] == 0 )
				{
					USART0_print("Key not pressed Up \n");
				  // If you want to do something when a key is released, do it here:

				}
			  }
			}
  
  
  
  
	_delay_ms(5);//cat de rar sa se faca 
	PORTB |= (1<< btncolumnpins[current]);
	PORTC |= (1<< ledcolumnpins[current]);

	
	
		//digitalWrite(btncolumnpins[i], HIGH);
		 PORTC &= ~(1<< PC4);
		 PORTC &= ~(1<< PC5);
		PORTC &= ~(1<< PC6);
		PORTC &= ~(1<< PC7);
		
		current++;
		if (current >= NUM_LED_COLUMNS)
		  {
			current = 0;
		  }


}

int main(void) {
	
	USART0_init();
	/*task1();*/
	/*task2();*/
	/*task3();*/
	USART0_print("Starting Setup... \n");


  // setup hardware
	setup();
	//next_scan = millis() + 1;

	next_advance =  millis() + 1000;
	led_index = 0;
	
	
	 for(uint8_t i = 0; i < NUM_LED_COLUMNS; i++)
	  {
		for(uint8_t j = 0; j < NUM_LED_ROWS; j++)
		{
		  LED_buffer[i][j] = 0;
		}
	  }

	USART0_print(" Setup...Complete\n");

	
	while (1){
		 //if(millis() >= next_scan)
		///  {
		//	next_scan = millis()+1;
		
			scan();
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
	}

	return 0;
}
