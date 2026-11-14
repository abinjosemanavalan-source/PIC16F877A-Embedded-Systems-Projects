
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

#define RS RB0
#define EN RB2
#define BUTTON RB1

// Send LCD command
void LCD_Command(unsigned char cmd)
{
    RS = 0;

    PORTD = (PORTD & 0x0F) | (cmd & 0xF0);

    EN = 1;
    __delay_ms(2);
    EN = 0;

    PORTD = (PORTD & 0x0F) | ((cmd << 4) & 0xF0);

    EN = 1;
    __delay_ms(2);
    EN = 0;
}

// Send character to LCD
void LCD_Char(unsigned char data)
{
    RS = 1;

    PORTD = (PORTD & 0x0F) | (data & 0xF0);

    EN = 1;
    __delay_ms(2);
    EN = 0;

    PORTD = (PORTD & 0x0F) | ((data << 4) & 0xF0);

    EN = 1;
    __delay_ms(2);
    EN = 0;
}

// Display string
void LCD_String(const char *str)
{
    while(*str)
    {
        LCD_Char(*str++);
    }
}

// Initialize LCD
void LCD_Init(void)
{
    __delay_ms(20);

    LCD_Command(0x02);
    LCD_Command(0x28);
    LCD_Command(0x0C);
    LCD_Command(0x06);
    LCD_Command(0x01);

    __delay_ms(5);
}

// Display counter
void Display_Count(unsigned int count)
{
    unsigned char hundreds;
    unsigned char tens;
    unsigned char units;

    hundreds = count / 100;
    tens = (count / 10) % 10;
    units = count % 10;

    LCD_Command(0xC0);

    LCD_String("Count: ");

    LCD_Char(hundreds + '0');
    LCD_Char(tens + '0');
    LCD_Char(units + '0');
}

void main(void)
{
    unsigned int count = 0;

    // Configure pins
    ADCON1 = 0x06;

    TRISB = 0b00000010;
    TRISD = 0x0F;

    PORTB = 0x00;
    PORTD = 0x00;

    LCD_Init();

    LCD_Command(0x80);
    LCD_String("Button Counter");

    Display_Count(count);

    while(1)
    {
        if(BUTTON == 0)
        {
            __delay_ms(20);

            // Confirm button press
            if(BUTTON == 0)
            {
                count++;

                Display_Count(count);

                // Wait until button is released
                while(BUTTON == 0);

                __delay_ms(20);
            }
        }
    }
}
