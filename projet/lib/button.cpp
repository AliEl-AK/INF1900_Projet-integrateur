#include <button.h>

Button::Button(Interruptions interruption, Edges edge){
    _interruption = interruption;
    _edge = edge;
    setOnInterruption(interruption, edge);
}


Button::~Button(){
    cli();
    switch(_interruption){
        case Interruptions::INTER0:
            EIMSK &= ~(1 << INT0);
            EICRA &= static_cast<uint8_t>(_edge);
            break;
        case Interruptions::INTER1:
            EIMSK &= ~(1 << INT1);
            EICRA &= (static_cast<uint8_t>(_edge) << 2);
            break;
    }
    sei();
}

void Button::setOnInterruption(Interruptions interruption, Edges edge)
{
    cli();
    switch(interruption){
        case Interruptions::INTER0:
            EIMSK |= (1 << INT0);
            EICRA |= static_cast<uint8_t>(edge);
            DDRD &= ~(1 << PD2);
            break;
        case Interruptions::INTER1:
            EIMSK |= (1 << INT1);
            EICRA |= (static_cast<uint8_t>(edge) << 2);
            DDRD &= ~(1 << PD3);
            break;
    }
    sei();
}


void Button::debouncer()
{
   _delay_ms(DEBOUNCE);
   EIFR |= (1 << INTF0);
}
