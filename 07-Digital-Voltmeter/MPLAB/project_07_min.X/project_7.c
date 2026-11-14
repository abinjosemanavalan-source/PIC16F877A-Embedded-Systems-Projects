/*
 * File:   project_7.c
 * Author: HP
 *
 * Created on 11 November, 2026, 8:42 AM
 */



#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 20000000UL

#pragma config FOSC = EC
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define RS RD0
#define EN RD1

void lcd_pulse(void)
{
    EN = 1;
    __delay_us(500);
    EN = 0;
    __delay_us(500);
}

void lcd_nibble(unsigned char value)
{
    PORTD = (PORTD & 0x0F) | (value & 0xF0);
    lcd_pulse();
}

void lcd_send(unsigned char value)
{
    lcd_nibble(value);
    lcd_nibble(value << 4);
}

void lcd_command(unsigned char cmd)
{
    RS = 0;
    lcd_send(cmd);
    __delay_ms(2);
}

void lcd_data(unsigned char data)
{
    RS = 1;
    lcd_send(data);
}

void lcd_init(void)
{
    __delay_ms(20);

    RS = 0;
    lcd_nibble(0x30);
    __delay_ms(5);
    lcd_nibble(0x30);
    __delay_us(200);
    lcd_nibble(0x30);
    lcd_nibble(0x20);

    lcd_command(0x28);
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_command(0x01);
}

void lcd_string(const char *str)
{
    while(*str)
        lcd_data(*str++);
}

unsigned int adc_read(void)
{
    __delay_us(25);

    GO_nDONE = 1;

    while(GO_nDONE);

    return ((unsigned int)ADRESH << 8) | ADRESL;
}

void main(void)
{
    unsigned int adc;
    float voltage;
    char display[17];

    TRISA = 0x01;
    TRISD = 0x00;

    PORTD = 0;

    // AN0 analog, VDD/VSS reference
    ADCON1 = 0x8E;

    // ADC ON, AN0, Fosc/32
    ADCON0 = 0x81;

    lcd_init();

    while(1)
    {
        adc = adc_read();

        // Two equal 10k resistors
        voltage = (adc * 5.0 / 1023.0) * 2.0;

        sprintf(display, "Voltage: %5.2fV", voltage);

        lcd_command(0x80);
        lcd_string("DIGITAL VOLTMETER");

        lcd_command(0xC0);
        lcd_string("                ");

        lcd_command(0xC0);
        lcd_string(display);

        __delay_ms(200);
    }
}
