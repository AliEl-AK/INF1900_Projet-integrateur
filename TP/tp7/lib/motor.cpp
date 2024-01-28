#include "motor.h"

Motor::Motor(Timer0 &timer, uint8_t pinDirectionLeft, uint8_t pinDirectionRight)
: _timer(timer) {
    DDRB |= pinDirectionLeft | pinDirectionRight;
    _pinDirectionLeft = pinDirectionLeft;
    _pinDirectionRight = pinDirectionRight;
    _timer.setMode(Timer0::Modes::PWM);
    _timer.setPrescaler(Timer0::Prescalers::CLK1);
    _timer.start();
}

void Motor::move(uint8_t percentage, uint8_t direction) {
    if (!direction) {
        PORTB &= ~( _pinDirectionLeft);
        PORTB &= ~( _pinDirectionRight);
    } 
    else {
        PORTB |= (_pinDirectionLeft) | (_pinDirectionRight);
    }

    moveLeft(percentage);
    moveRight(percentage);
}


void Motor::moveRight(uint8_t percentage) {
    uint8_t speed = _timer.PWMRatioToCycle(percentage);
    _timer.setOCRB(speed);
}

void Motor::moveLeft(uint8_t percentage) {
    uint8_t speed = _timer.PWMRatioToCycle(percentage);
    _timer.setOCRA(speed);
}

void Motor::stop() {
    _timer.stop();
}