/////////////////////////////////////////////////////////////////////////////
// Include files
// These lines allow us to use code routines (libraries) from other files,
// so that we don't have to write everything by ourselves.
/////////////////////////////////////////////////////////////////////////////

#include <p18lf25k22.h>	    // This file includes definitions for all of the
                            // registers on our chip
#include <portb.h>			// This file incldues defenitions for portb interrupts
#include <timers.h>			// This file incldues defenitions for timers of which
							// we will use T0 for debouncing our input button

/////////////////////////////////////////////////////////////////////////////
// Pragma statements
// These lines tell the microcontroller what configuration to use when it
// when it turns on. The most important part for now is to tell the
// microcontroller what to use for a clock input.
/////////////////////////////////////////////////////////////////////////////

// Use the internal oscillator as a clock
#pragma config FOSC = INTIO67

// Configure it to run at 16 MHz
#pragma config PLLCFG = OFF

// Make sure that the internal watchdog timer is off. The watchdog is a
// a special feature of the processor, that runs separately from the main
// program and can be used to clea
#pragma config WDTEN = OFF

/////////////////////////////////////////////////////////////////////////////
// Function declarations
// Declare any user functions that you want to use here
/////////////////////////////////////////////////////////////////////////////
void main (void);
void setup(void);
void loop(void);
unsigned int is_pressd(void);
void buttonPress(void);			// prototypes for interrupts
void timerExpired(void);			// prototypes for interrupts

// Add Interrupt handler for Port RB0
#pragma interrupt buttonPress		// buttonPress is an interrupt
#pragma interrupt timerExpired
#pragma code high_vector = 0x8
void high_vector(void)
{
	if(INTCONbits.INT0IF == 1)
	{
		_asm goto buttonPress _endasm
	}
	else if(INTCONbits.TMR0IF == 1)
	{
		_asm goto timerExpired _endasm
	}	
}
#pragma code

void buttonPress(void)
{
	INTCONbits.INT0IF = 0;
	INTCONbits.TMR0IE = 1;
	WriteTimer0(0);		//Make sure we start our timer with value 0
}	

void timerExpired(void)
{
	INTCONbits.TMR0IE = 0;
	INTCONbits.TMR0IF = 0;
	if(PORTBbits.RB0 == 0) //Button is still pressed
	{
		PORTAbits.RA5 = !PORTAbits.RA5;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Function definitions
// Define any user functions that you want to use here
/////////////////////////////////////////////////////////////////////////////

// The main() function is where the program 'starts' when the microcontroller
// is turned on. For this project, we will use it to call setup() and loop()
// functions, then use those similar to an Arduino sketch.
void main (void)
{
    // Call the setup function once to put everything in order
    setup();

    // Then call the loop() function over and over
    while (1)
    {
        loop();
    }
}

// This function is called once, when the microcontroller is turned on.
void setup(void) {
    // Configure the oscillator to use a 16 MHz input.
    OSCCONbits.IRCF = 111;

    // Set port RA1 to be an input
    // Set RA0(LED1) to low
    TRISA = 0x0;
    ANSELAbits.ANSA1 = 0;
	ANSELAbits.ANSA5 = 0;
    PORTAbits.RA0 = 0;
	PORTAbits.RA5 = 1;

	//Set port RB0 to be a external interrupt
	TRISB = 0xFF;
	ANSELBbits.ANSB0 = 0;
	RCONbits.IPEN = 1;
	INTCONbits.GIEH = 1;
	OpenRB0INT( PORTB_CHANGE_INT_ON &
				PORTB_PULLUPS_ON &
				FALLING_EDGE_INT);
	
	//Setup our timer which we will use for debouncing
	OpenTimer0( T0_16BIT &
				T0_SOURCE_INT &
				T0_EDGE_RISE &
				T0_PS_1_4 &
				TIMER_INT_ON &
				TIMER_INT_OFF);
}

// This function is called repeatedly
void loop(void) {
	int i;

	for(i = 0; i < 30000; i++);
	PORTAbits.RA0 = 1;
    for(i = 0; i < 30000; i++);
	PORTAbits.RA0 = 0;

	Sleep();
}