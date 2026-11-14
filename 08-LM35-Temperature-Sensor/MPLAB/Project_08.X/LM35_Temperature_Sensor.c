
#include <xc.h>
#define _XTAL_FREQ 20000000

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define RS RB0
#define EN RB1

void lcd_command(unsigned char cmd)
{
    PORTD = (PORTD & 0x0F) | (cmd & 0xF0);
    RS = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;

    PORTD = (PORTD & 0x0F) | ((cmd << 4) & 0xF0);
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

void lcd_data(unsigned char data)
{
    PORTD = (PORTD & 0x0F) | (data & 0xF0);
    RS = 1;
    EN = 1;
    __delay_ms(2);
    EN = 0;

    PORTD = (PORTD & 0x0F) | ((data << 4) & 0xF0);
    EN = 1;
    __delay_ms(2);
    EN = 0;

    RS = 0;
}

void lcd_string(const char *str)
{
    while(*str)
        lcd_data(*str++);
}

void lcd_init(void)
{
    __delay_ms(20);
    lcd_command(0x02);
    lcd_command(0x28);
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_command(0x01);
    __delay_ms(5);
}

void ADC_Init(void)
{
    ADCON1 = 0x8E;
    ADCON0 = 0x81;
}

unsigned int ADC_Read(void)
{
    __delay_us(30);
    GO_nDONE = 1;

    while(GO_nDONE);

    return ((ADRESH << 8) | ADRESL);
}

void main(void)
{
    unsigned int adc_value;
    unsigned long temp;

    TRISA = 0x01;
    TRISB = 0x00;
    TRISD = 0x00;

    PORTA = 0;
    PORTB = 0;
    PORTD = 0;

    ADC_Init();
    lcd_init();

    lcd_command(0x80);
    lcd_string("LM35 SENSOR");
    __delay_ms(1500);

    while(1)
    {
        adc_value = ADC_Read();

        // Temperature x 10
        temp = ((unsigned long)adc_value * 5000UL + 511UL) / 1023UL;

        lcd_command(0x80);
        lcd_string("TEMPERATURE     ");

        lcd_command(0xC0);

        lcd_string("TEMP: ");

        // Hundreds digit
        lcd_data((temp / 1000) + '0');

        // Tens digit
        lcd_data(((temp / 100) % 10) + '0');

        // Units digit
        lcd_data(((temp / 10) % 10) + '0');

        lcd_data('.');

        // Decimal digit
        lcd_data((temp % 10) + '0');

        lcd_string(" C   ");

        __delay_ms(500);
    }
}
