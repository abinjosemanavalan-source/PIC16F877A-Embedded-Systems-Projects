/*
 * File:   Temperature_Monitoring_using_LM35.c
 * Author: HP
 *
 * Created on 13 November, 2026, 12:27 PM
 */



#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 20000000

// CONFIGURATION BITS
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

// LCD CONNECTIONS
#define RS RB0
#define EN RB1

// LCD COMMAND
void LCD_Command(unsigned char cmd)
{
    RS = 0;

    PORTB = (PORTB & 0x0F) | (cmd & 0xF0);
    EN = 1;
    __delay_ms(2);
    EN = 0;

    PORTB = (PORTB & 0x0F) | ((cmd << 4) & 0xF0);
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

// LCD DATA
void LCD_Char(unsigned char data)
{
    RS = 1;

    PORTB = (PORTB & 0x0F) | (data & 0xF0);
    EN = 1;
    __delay_ms(2);
    EN = 0;

    PORTB = (PORTB & 0x0F) | ((data << 4) & 0xF0);
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

// LCD INITIALIZATION
void LCD_Init()
{
    __delay_ms(30);

    LCD_Command(0x02);
    LCD_Command(0x28);
    LCD_Command(0x0C);
    LCD_Command(0x06);
    LCD_Command(0x01);

    __delay_ms(5);
}

// LCD STRING
void LCD_String(const char *str)
{
    while(*str)
    {
        LCD_Char(*str++);
    }
}

// LCD CURSOR
void LCD_Set_Cursor(unsigned char row,
                    unsigned char column)
{
    if(row == 1)
        LCD_Command(0x80 + column - 1);
    else
        LCD_Command(0xC0 + column - 1);
}

// ADC INITIALIZATION
void ADC_Init()
{
    ADCON0 = 0x41;
    ADCON1 = 0x8E;
}

// ADC READ
unsigned int ADC_Read()
{
    __delay_us(25);

    GO_nDONE = 1;

    while(GO_nDONE);

    return ((ADRESH << 8) | ADRESL);
}

// MAIN PROGRAM
void main()
{
    unsigned int adc_value;
    unsigned long temperature;
    char buffer[16];

    TRISA = 0x01;
    TRISB = 0x00;

    PORTB = 0;

    ADC_Init();
    LCD_Init();

    while(1)
    {
        adc_value = ADC_Read();

        // LM35: 10mV per degree C
        // ADC reference = 5V
        // Temperature = ADC * 500 / 1023

        temperature =
        ((unsigned long)adc_value * 5000UL) / 1023UL;

        LCD_Set_Cursor(1,1);
        LCD_String("TEMPERATURE:");

        LCD_Set_Cursor(2,1);

        sprintf(buffer,"TEMP: %lu.%01lu C",
                temperature / 10,
                temperature % 10);

        LCD_String(buffer);

        __delay_ms(500);
    }
}
