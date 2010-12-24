/////////////////////////////////////////////////////////////////////////////
// Include files
// These lines allow us to use code routines (libraries) from other files,
// so that we don't have to write everything by ourselves.
/////////////////////////////////////////////////////////////////////////////

#include <p18lf25k22.h>	    // This file includes definitions for all of the
                            // registers on our chip
#include <stdio.h>			// For the sprintf() function
#include <usart.h>			// For communication with computer

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

    // Set port RA1 to be an input
    // Set RA0(LED1) to low
    TRISA = 0x0;
    ANSELAbits.ANSA1 = 0;
    PORTAbits.RA0 = 0;
	
	// Serial port configuration
    TRISCbits.TRISC6 = 0;	  // Make TX pin an output
    TRISCbits.TRISC7 = 1;	  // and RX pin an input
    ANSELCbits.ANSC7 = 0;     // Specifically, an analog input

    // Configure the serial port to run at 9600 baud
    // (see manual, page 275)
    BAUD1CONbits.BRG16 = 0;
    SPBRG1 = 25;
    TXSTA1bits.BRGH = 0;      // Baud rate select

    RCSTA1bits.CREN = 1;      // Enable receive mode on the serial port

    // Turn on the serial port
    TXSTA1bits.TXEN = 1;      // Enable transmitter
    RCSTA1bits.SPEN = 1;      // Enable receiver
}

// This function is called repeatedly
void loop(void) {
	// Temporary storage variables
    unsigned char data;
    char buffer[40];

    // Check if the serial port has overflown, and clear the event if that happened.
    if (RCSTAbits.OERR) {
      RCSTA1bits.CREN = 0;
      RCSTA1bits.CREN = 1;
    }

    // Check if there is serial data waiting for us
    if(PIR1bits.RC1IF) {
        data = RCREG1;
		switch(data) {
			case 'd':
				sprintf(buffer,"LED disabled\n\r");
           	    puts1USART(buffer);
				PORTAbits.RA0 = 0;
				break;
			case 'e':
				sprintf(buffer,"LED enabled\n\r");
           	    puts1USART(buffer);
				PORTAbits.RA0 = 1;
				break;
			default:
				break;	
		}
	
	}
}