#include "timer2.h"

Timer2::Timer2(){
    cli();
    
    TCNT2 = 0;
    OCR2A = 0;
    OCR2B = 0;
    TCCR2A = 0;
    TCCR2B = 0;
    TIMSK2 = 0;
    _prescaler = 0;
    
    sei();  
}

Timer2::~Timer2(){
    cli();
    
    TCNT2 = 0;
    OCR2A = 0;
    OCR2B = 0;
    TCCR2A = 0;
    TCCR2B = 0;
    TIMSK2 = 0;
    
    sei();
}

void Timer2::resetMode(){
    TCCR2A &= ~(1 << WGM20) & ~(1 << WGM21);
    TCCR2B &= ~(1 << WGM22);
    TIMSK2 &= ~(1 << OCIE2A) & ~(1 << OCIE2B);
}

void Timer2::resetPrescaler(){
    TCCR2B &= ~(1 << CS20) & ~(1 << CS21) & ~(1 << CS22);
}

void Timer2::setMode(Modes mode){
    cli();
    resetMode();
    
    switch(mode){

        case Modes::NORMAL:
            break;

        case Modes::CTC_1_COMPARE:
            TCCR2A |= (1 << WGM21);
            TIMSK2 |= (1 << OCIE2A);
            break;

        case Modes::CTC_2_COMPARE:
            TCCR2A |= (1 << WGM21);
            TIMSK2 |= (1 << OCIE2A);
            TIMSK2 |= (1 << OCIE2B);
            break;

        case Modes::PWM:
            TCCR2A |= (1 << WGM20) | (1 << COM2A1) | (1 << COM2B1);
            DDRD |= (1 << PD6) | (1 << PD7);
            break;
    }
    sei();
}

void Timer2::setPrescaler(Prescalers prescaler){
    cli();
    resetPrescaler();
    _prescaler = static_cast<uint8_t>(prescaler);
    sei(); 
}

void Timer2::start(){
    TCCR2B |= _prescaler;
}

void Timer2::stop(){
    resetPrescaler();
    TCNT2 = 0; 
}

void Timer2::setOCRA(uint8_t cycle){
    OCR2A = cycle;
}

void Timer2::setOCRB(uint8_t cycle){
    OCR2B = cycle;
}

uint8_t Timer2::PWMRatioToCycle(uint8_t PWMRatio){
    return static_cast<uint8_t>((TOP * PWMRatio/PERCENT)); 
}

