
#include <xc.h>

#define _XTAL_FREQ 20000000UL

// Configuration bits
#pragma config FOSC = HS
#pragma config WDTE = ON
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void main(void)
{
    // Configure RD0 and RD1 as outputs
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;

    // Initially turn OFF both LEDs
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;

    // Configure Watchdog Timer prescaler
    OPTION_REGbits.PSA = 1;

    // WDT prescaler = 1:128
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;

    while(1)
    {
        // STAGE 1: Normal operation
        // Red LED blinks 3 times

        for(int i = 0; i < 7; i++)
        {
            PORTDbits.RD0 = 1;  // Red ON
            PORTDbits.RD1 = 0;  // Green OFF

            __delay_ms(300);

            PORTDbits.RD0 = 0;  // Red OFF

            __delay_ms(300);

            CLRWDT();  // Clear Watchdog Timer
        }

        // STAGE 2: Firmware failure
        PORTDbits.RD0 = 0;  // Red OFF
        PORTDbits.RD1 = 1;  // Green ON

        // STAGE 3: Intentional infinite loop
        while(1)
        {
            // Do not clear Watchdog Timer
            // PIC will automatically reset
        }
    }
}
