#include <timer0.h>

Timer0::Timer0(){
    cli();
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    OCR0A = 0;
    OCR0B = 0;
    TIMSK0 = 0;
    _prescaler = 0;
    sei();
}

Timer0::~Timer0(){
    cli();
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    OCR0A = 0;
    OCR0B = 0;
    TIMSK0 = 0;
    sei();
}

void Timer0::resetMode(){
    TCCR0A &= ~(1 << WGM00) & ~(1 << WGM01);
    TCCR0B &= ~(1 << WGM02);
    TIMSK0 &= ~(1 << OCIE0A) & ~(1 << OCIE0B);
}

void Timer0::resetPrescaler(){
    TCCR0B &= ~(1 << CS00);
    TCCR0B &= ~(1 << CS01);
    TCCR0B &= ~(1 << CS02);
}

void Timer0::setMode(Modes mode){
    cli();
    resetMode();
    switch(mode){
        case Modes::NORMAL:
            break;
        case Modes::CTC_1_COMPARE:
            TCCR0A |= (1 << WGM01);
            TIMSK0 |= (1 << OCIE0A);
            break;
        case Modes::CTC_2_COMPARE:
            TCCR0A |= (1 << WGM01);
            TIMSK0 |= (1 << OCIE0A);
            TIMSK0 |= (1 << OCIE0B);
            break;
        case Modes::PWM:
            TCCR0A |= (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
            DDRB |= (1 << PB3) | (1 << PB4);
            break;
    }
    sei();
}

void Timer0::setPrescaler(Prescalers prescaler){
    cli();
    resetPrescaler();
    _prescaler |= static_cast<uint8_t>(prescaler);
    sei(); 
}

void Timer0::start(){
    TCCR0B |= _prescaler;
}

void Timer0::stop(){
    resetPrescaler();
    TCNT0 = 0;
}

void Timer0::setOCRA(uint8_t cycle){
    OCR0A = cycle;
}

void Timer0::setOCRB(uint8_t cycle){
    OCR0B = cycle;
}

//Params: PWMRation: (0-100); Pourcentage
uint8_t Timer0::PWMRatioToCycle(uint8_t PWMRatio){
    return static_cast<uint8_t>((TOP * PWMRatio/PERCENT));
}



