/*
 * File:   project_4.c
 * Author: HP
 *
 * Created on 31 October, 2026, 10:45 PM
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

volatile unsigned int timer_count = 0;
volatile unsigned char blink_event = 0;

void __interrupt() ISR(void)
{
    if (INTCONbits.TMR0IF)
    {
        TMR0 = 100;
        INTCONbits.TMR0IF = 0;

        timer_count++;

        if (timer_count >= 10)
        {
            timer_count = 0;
            blink_event = 1;
        }
    }
}

void main(void)
{
    ADCON1 = 0x06;

    TRISDbits.TRISD0 = 0;
    TRISBbits.TRISB1 = 1;

    PORTDbits.RD0 = 0;

    // Timer0: internal clock, prescaler 1:256
    OPTION_REG = 0b00000111;

    TMR0 = 100;

    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.GIE = 1;

    while (1)
    {
        // Toggle LED at each timer event
        if (blink_event)
        {
            blink_event = 0;
            PORTDbits.RD0 = !PORTDbits.RD0;
        }

        // Respond to button press
        if (PORTBbits.RB1 == 0)
        {
            __delay_ms(20);

            if (PORTBbits.RB1 == 0)
            {
                PORTDbits.RD0 = 1;

                // Wait for button release
                while (PORTBbits.RB1 == 0);

                __delay_ms(20);
            }
        }
    }
}
