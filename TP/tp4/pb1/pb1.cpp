#define F_CPU 8000000
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
volatile uint8_t state = 0;
ISR(INT0_vect)
{
    _delay_ms(30);

    if (state == 5)
    {
        state = 0;
    }
    else
    {
        state++;
    }
    EIFR |= (1 << INTF0);
}

void initialisation(void)
{
    cli();
    DDRA = 0xff;
    DDRD = 0x00;

    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00);

    sei();
}

void delRouge()
{
    PORTA = (1 << PINA0);
}
void delVert()
{
    PORTA = (1 << PINA1);
}
void delAmbre()
{

    delRouge();
    _delay_ms(10);
    delVert();
    _delay_ms(10);
}
void delOff()
{
    PORTA = (0 << PINA0);
}

int main()
{
    initialisation();
    while (true)
    {
        switch (state)
        {
        case 0:
            delRouge();
            break;
        case 1:
            delAmbre();
            break;
        case 2:
            delVert();
            break;
        case 3:
            delRouge();

            break;
        case 4:
            delOff();

            break;
        case 5:
            delVert();

            break;
        }
    }
}
