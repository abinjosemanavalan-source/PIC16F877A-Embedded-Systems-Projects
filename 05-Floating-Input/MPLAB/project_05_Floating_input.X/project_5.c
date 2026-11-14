/*
 * File:   project_5.c
 * Author: HP
 *
 * Created on 1 November, 2026, 10:11 AM
 */


#include <xc.h>

// Configuration bits
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 4000000UL

void main(void)
{
    // Configure pins
    TRISCbits.TRISC0 = 1;  // RC0 input
    TRISDbits.TRISD0 = 0;  // RD0 output

    // Initialize LED
    PORTDbits.RD0 = 0;

    while (1)
    {
        if (PORTCbits.RC0 == 0)
        {
            PORTDbits.RD0 = 1;  // Button pressed: LED ON
        }
        else
        {
            PORTDbits.RD0 = 0;  // Button released: LED OFF
        }
    }
}