/////////////////////////////////////////////////////////////////////////////
// Include files
// These lines allow us to use code routines (libraries) from other files,
// so that we don't have to write everything by ourselves.
/////////////////////////////////////////////////////////////////////////////

#include <p18lf25k22.h>	    // This file includes definitions for all of the
                            // registers on our chip

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

    // Set RA0(LED1) to low
    TRISA = 0x0;
	LATA = 0;
    ANSELAbits.ANSA0 = 0;
    PORTAbits.RA0 = 0;
	
	// PWM Period = 4 x Tosc x (PR2 + 1) x TMR2 Prescale Value
	// Setup RC2/CCP1 for PWM
	PORTC = 0x00;
	TRISCbits.RC2 = 0;		//Step 1 - Corresponding TRIS bit must be cleared
	CCPTMRS0bits.C1TSEL = 0b00; //Step 2 - Choose timer source (00 for timer 2)
	PR2 = 250;	//Step 3 - Load selected timer with PWM value
	CCP1CONbits.CCP1M = 0b1100; //Step 4 - Configure CCP module to enable PWM instead of compare
	CCP1CONbits.DC1B = 0; //Step 5 - Configure lowest 2 bytes of duty cycle
	CCPR1L = 0; //Step 6 - 0 initial duty cycle
	TMR2 = 0; //Step 7 - Initialize timer to 0
	T2CONbits.TMR2ON = 1; //Step 8 - Turn Timer ON
	
}

// This function is called repeatedly
void loop(void) {
	int duty_cycle = 0x00;
	int i = 0;
	CCPR1L = duty_cycle;
	
	for(duty_cycle = 0x00; duty_cycle < 0xFF; duty_cycle++)
	{
		for(i=0; i < 10000; i++);
		CCPR1L = duty_cycle;
	}	
	
}