/*
 * File:   project_6.c
 * Author: HP
 *
 * Created on 3 November, 2026, 2:48 PM
 */


#include <xc.h>
#include <stdio.h>

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 4000000

// LCD pin definitions
#define RS RD0
#define EN RD1
#define LCD_PORT PORTD

void LCD_Command(unsigned char cmd) {
    RS = 0;
    LCD_PORT = (LCD_PORT & 0x0F) | (cmd & 0xF0);
    EN = 1; __delay_ms(2); EN = 0;
    LCD_PORT = (LCD_PORT & 0x0F) | ((cmd << 4) & 0xF0);
    EN = 1; __delay_ms(2); EN = 0;
}

void LCD_Char(unsigned char data) {
    RS = 1;
    LCD_PORT = (LCD_PORT & 0x0F) | (data & 0xF0);
    EN = 1; __delay_ms(2); EN = 0;
    LCD_PORT = (LCD_PORT & 0x0F) | ((data << 4) & 0xF0);
    EN = 1; __delay_ms(2); EN = 0;
}

void LCD_Init(void) {
    LCD_Command(0x02);
    LCD_Command(0x28);
    LCD_Command(0x0C);
    LCD_Command(0x06);
    LCD_Command(0x01);
    __delay_ms(5);
}

void LCD_String(const char *str) {
    while (*str) LCD_Char(*str++);
}

void ADC_Init(void) {
    ADCON0 = 0x01;   // Enable ADC, select AN0
    ADCON1 = 0x0E;   // AN0 analog, others digital, right-justified
}

unsigned int ADC_Read(void) {
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO);
    return ((ADRESH << 8) | ADRESL);
}

void main(void) {
    TRISA = 0x01;    // RA0 input
    TRISD = 0x00;    // RD output

    ADC_Init();
    LCD_Init();

    char buffer[6];
    unsigned int adc_val;

    while (1) {
        adc_val = ADC_Read();
        LCD_Command(0x80);
        sprintf(buffer, "%4u", adc_val);
        LCD_String(buffer);
        __delay_ms(200);
    }
}
