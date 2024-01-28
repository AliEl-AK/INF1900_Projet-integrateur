#include "timer1.h"

Timer1::Timer1(){
    cli();
    TCNT1 = 0;
    OCR1A = 0;
    OCR1B = 0;
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
    TIMSK1 = 0;
    _prescaler = 0;
    sei();
}

Timer1::~Timer1(){
    cli();
    TCNT1 = 0;
    OCR1A = 0;
    OCR1B = 0;
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
    TIMSK1 = 0;
    sei();
}

void Timer1::resetMode(){
    cli();
    TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B &= ~(1 << WGM12);
    TCCR1B &= ~(1 << WGM13);
    TIMSK1 &= ~(1 << OCIE1A);
    TIMSK1 &= ~(1 << OCIE1B);
    sei();
}

void Timer1::resetPrescaler(){
    cli();
    TCCR1B &= ~(1 << CS10);
    TCCR1B &= ~(1 << CS11);
    TCCR1B &= ~(1 << CS12);
    sei();
}

void Timer1::setMode(Modes mode){
    cli();
    resetMode();
    switch(mode){
        
        case Modes::NORMAL:
            break;

        case Modes::CTC_1_COMPARE:
            TCCR1B |= (1 << WGM12);
            TIMSK1 |= (1 << OCIE1A);
            break;

        case Modes::CTC_2_COMPARE:
            TCCR1B |= (1 << WGM12);
            TIMSK1 |= (1 << OCIE1A);
            TIMSK1 |= (1 << OCIE1B);
            break;
            
        case Modes::PWM:
            TCCR1A |= (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
            DDRD |= (1 << PD4) | (1 << PD5);
            break;
    }
    sei();
}

void Timer1::setPrescaler(Prescalers prescaler){
    cli();
    resetPrescaler();
    _prescaler |= static_cast<uint8_t>(prescaler);
    sei();
}

void Timer1::start(){
    TCCR1B |= _prescaler;
}

void Timer1::stop(){
    resetPrescaler();
    TCNT1 = 0;
}

void Timer1::setOCRA(uint16_t cycle){
    OCR1A = cycle;
}

void Timer1::setOCRB(uint16_t cycle){
    OCR1B = cycle;
}