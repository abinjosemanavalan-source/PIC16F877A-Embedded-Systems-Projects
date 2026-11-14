
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
    ADCON0 = 0x41;  // ADC ON, AN0
    ADCON1 = 0x8E;  // AN0 analog, right justified
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
    unsigned int ldr;
    char buffer[16];

    // PORT SETTINGS
    ADC_Init();

    TRISA = 0x01;
    TRISB = 0x00;
    TRISC = 0x00;

    PORTB = 0x00;
    PORTC = 0x00;

    // INITIALIZE LCD
    LCD_Init();

    while(1)
    {
        // READ LDR
        ldr = ADC_Read();

        // DISPLAY ADC VALUE
        LCD_Set_Cursor(1,1);
        LCD_String("LDR VALUE:");

        sprintf(buffer, "%4u   ", ldr);

        LCD_Set_Cursor(1,12);
        LCD_String(buffer);

        // AUTOMATIC LIGHT CONTROL
        LCD_Set_Cursor(2,1);

        if(ldr < 400)
        {
            RC0 = 1;

            LCD_String("DARK: LIGHT ON ");
        }
        else if(ldr > 600)
        {
            RC0 = 0;

            LCD_String("BRIGHT: LIGHT OFF");
        }

        __delay_ms(200);
    }
}
