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
static char matrice[NUM_BTN_COLUMNS][NUM_BTN_ROWS];
static char matrice_s[NUM_BTN_COLUMNS][NUM_BTN_ROWS] = {'.',',','*','<','>','+','-','=','_','~','(',')','#','@','&','%'};
static uint8_t matrice_f[NUM_BTN_COLUMNS][NUM_BTN_ROWS];

void setup(void) {

		char ch = 'a';
		int i, j; 
	
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
	 
		 DDRC |= (1<< PC0);
		 PORTC &= ~(1<< PC0);
		
		 DDRC |= (1<< PC1);
		 PORTC &= ~(1<< PC1);
		
		 DDRC |= (1<< PC2);
		 PORTC &= ~(1<< PC2);
		
		/* Coloane LED-uri*/
		 DDRA |= (1 << PA4); //iesire
		 PORTA |= (1<< PA4);
		 
		 DDRA |= (1<< PA5);
		 PORTA |= (1<< PA5); //aprins
		
		 DDRA |= (1<< PA6);
		 PORTA |= (1<< PA6);
		 
		 DDRA |= (1<< PA7);
		 PORTA |= (1<< PA7);

		/* prima etapa */
		PORTA |= (1 << PA0);//pronim linia
		PORTA |= (1 << PA3);

	for( i = 4; i< 8; i++){
	
		PORTA &=~(1 << i);
		 _delay_ms(500);
	} 
	 
	PORTA &=~(1 << PA0);
	PORTA &=~(1 << PA3);
	
	for( i = 4; i< 8; i++){

		PORTA |=(1 << i);
		 _delay_ms(500);
	} 
	
		 
	/* a doua etapa */
	
	PORTA |= (1 << PA1);//pronim linia
	PORTA |= (1 << PA2);
		 
	PORTA &=~(1 << PA5);
	PORTA &=~(1 << PA6);
	
	 _delay_ms(500);
	
	PORTA &=~(1 << PA1);
	PORTA &=~(1 << PA2);
	
	PORTA |=(1 << PA5);
	PORTA |=(1 << PA6);
	
	 _delay_ms(500);
		
		
	/* a treia etapa */
	PORTA |= (1 << PA1);//pronim linia
	PORTA |= (1 << PA2);
		 

	for( i = 5; i < 7 ; i++){
	
		PORTA &=~(1 << i);
		//PORTC &=~(1 << PC2);
		 _delay_ms(500);

	}
	
	PORTA &=~(1 << PA1);
	PORTA &=~(1 << PA2);
	
	for( i = 5; i < 7  ; i++){
	
		PORTA |=(1 << i);
		//PORTC |=(1 << PC2);
		 _delay_ms(500);
	
	}
	

	/* a patra  etapa */
	
	PORTA |= (1 << PA1);//pronim linia	 
	//PORTA &=~(1 << PA5);
	PORTC &=~(1 << PC1);
		
	_delay_ms(500);


	
	PORTA &=~(1 << PA1);
//	PORTA |=(1 << PA5);
	PORTC |=(1 << PC1);
	_delay_ms(500);

	
	
	PORTA |= (1 << PA2);
		 
	//PORTA &=~(1 << PA6);
	PORTC &=~(1 << PC2);
	
	_delay_ms(500);
	PORTA &=~(1 << PA2);
	
	//PORTA |=(1 << PA6);
	PORTC |=(1 << PC2);
	_delay_ms(800);
	
	
	/* a cincea etapa  */
	
	PORTA |= (1 << PA2);
	PORTA &=~(1 << PA5);
		//PORTC &=~(1 << PC1);
	_delay_ms(500);

	PORTA &=~(1 << PA2);
	PORTA |=(1 << PA5);
		//PORTC |=(1 << PC1);
	_delay_ms(500);
	
	
	/* a sasea  etapa */
	
	PORTA |= (1 << PA1);//pronim linia	 
	PORTA &=~(1 << PA6);
		//PORTC &=~(1 << PC1);
	_delay_ms(500);

	
	PORTA &=~(1 << PA1);
	PORTA |=(1 << PA6);
		//PORTC |=(1 << PC1);
	_delay_ms(500);
	
	for( i = 0; i < NUM_BTN_ROWS; i++){
		for( j = 0; j < NUM_BTN_COLUMNS; j++){
				matrice[i][j] = ch;
				ch++;
		}
	}

	for( i = 0; i < NUM_BTN_ROWS; i++){
		for( j = 0; j < NUM_BTN_COLUMNS; j++){
				matrice_f[i][j] = 0;
				
		}
	}  
	
}

static uint8_t current = 0;

static void scan()
{   
	uint8_t val;
    uint8_t i, j;

	char vect_ch[2];
	vect_ch[1]='\0';

	PORTA &= ~(1<< ledcolumnpins[current]);
	PORTB &= ~(1<< btncolumnpins[current]);


// pause a moment
  _delay_ms(5);

 
		//daca butonul e apasat
		if ((PINB & (1<<PB0))==0)
		{
			PORTA |= (1 << PA0);
			PORTC |= (1 << PC0);
			PORTA &=~(1 << ledcolumnpins[current]);
			PORTC &=~(1 << ledcolumnpins[current]);
			
			vect_ch[0]= matrice[0][current];
			if(matrice_f[0][current] == 0){
				USART0_print(vect_ch);
				matrice_f[0][current] = 1;
			}
		}
		else{
			if(matrice_f[0][current] == 1){
				vect_ch[0]= matrice_s[0][current];
				USART0_print(vect_ch);
				matrice_f[0][current] = 0;
			}
		}

		if((PINB & (1<<PB1))==0)
		{
				PORTA |= (1 << PA1);
				PORTA &=~(1 << ledcolumnpins[current]);
				vect_ch[0]= matrice[1][current];
				if(matrice_f[1][current] == 0){
				USART0_print(vect_ch);
				matrice_f[1][current] = 1;
			}
		}
		else{
				if(matrice_f[1][current] == 1){
				vect_ch[0]= matrice_s[1][current];
				USART0_print(vect_ch);
				matrice_f[1][current] = 0;
			}
		
		}

		//daca butonul e apasat
		if((PINB & (1<<PB2))==0)
		{
			PORTA |= (1<< PA2);
			PORTA &=~(1 << ledcolumnpins[current]);
			vect_ch[0]= matrice[2][current];
			if(matrice_f[2][current] == 0){
				USART0_print(vect_ch);
				matrice_f[2][current] = 1;
			}

		}
		else{
			if(matrice_f[2][current] == 1){
				vect_ch[0]= matrice_s[2][current];
				USART0_print(vect_ch);
					matrice_f[2][current] = 0;
			}
		}
	

		//daca butonul e apasat
		if((PINB & (1<<PB3))==0)
		{
			PORTA |= (1 << PA3);
			PORTA &=~(1 << ledcolumnpins[current]);
			vect_ch[0]= matrice[3][current];
			if(matrice_f[3][current] == 0){
				USART0_print(vect_ch);
				matrice_f[3][current] = 1;
			}
		}
		else{
	
			if(matrice_f[3][current] == 1){
				vect_ch[0]= matrice_s[3][current];
				USART0_print(vect_ch);
				matrice_f[3][current] = 0;
			}
		}
  
	_delay_ms(5); 

	/*se aprinde o linie a butonului apasat si vrem sa se aprinda doar butonul curent*/
	PORTB |= (1 << btncolumnpins[current]);
	PORTA |= (1 << ledcolumnpins[current]);
	PORTC |= (1 << ledcolumnpins[current]);

	/* fara astea se aprind linii si raman aprinse mereu*/
	PORTA &= ~(1<< PA0);
	PORTA &= ~(1<< PA1);
	PORTA &= ~(1<< PA2);
	PORTA &= ~(1<< PA3);
	PORTC &= ~(1<< PC0);
	PORTC &= ~(1<< PC1);
	PORTC &= ~(1<< PC2);
	
	current++;
	if (current >= NUM_LED_COLUMNS )
	{
		current = 0;
	}

}

int main(void) {
	
	USART0_init();
	setup();

	while (1){
	
		scan(); 
	}

	return 0;
}
