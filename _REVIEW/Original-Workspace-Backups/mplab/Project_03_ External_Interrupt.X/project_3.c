/*
 * File:   project_3.c
 * Author: HP
 *
 * Created on 31 October, 2026, 1:05 PM
 */

#include <xc.h>

#define _XTAL_FREQ 20000000UL

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

volatile unsigned char button_event = 0;

// Interrupt Service Routine
void __interrupt() ISR(void)
{
    if (INTCONbits.INTF == 1)
    {
        button_event = 1;       // Record button press
        INTCONbits.INTF = 0;    // Clear interrupt flag
    }
}

void main(void)
{
    ADCON1 = 0x06;              // Configure analogue pins as digital

    TRISDbits.TRISD0 = 0;       // RD0: LED output
    TRISBbits.TRISB0 = 1;       // RB0: Button input

    PORTDbits.RD0 = 0;

    OPTION_REGbits.INTEDG = 0;  // Interrupt on falling edge

    INTCONbits.INTF = 0;        // Clear interrupt flag
    INTCONbits.INTE = 1;        // Enable external interrupt
    INTCONbits.GIE = 1;         // Enable global interrupts

   while (1)
{
    if (button_event == 1)
    {
        button_event = 0;

        PORTDbits.RD0 = 1;  // LED ON
        __delay_ms(1000);   // Stay ON for 1 second
        PORTDbits.RD0 = 0;  // LED OFF
    }
    else
    {
        PORTDbits.RD0 = 1;
        __delay_ms(300);

        PORTDbits.RD0 = 0;
        __delay_ms(300);
    }
}
}