#include <motor.h>

Motor::Motor(Timer0 &timer, uint8_t pinDirectionLeft, uint8_t pinDirectionRight)
: _timer(timer) {
    DDRB |= pinDirectionLeft | pinDirectionRight;
    PORTB &= ~( _pinDirectionLeft);
    PORTB &= ~( _pinDirectionRight);
    _pinDirectionLeft = pinDirectionLeft;
    _pinDirectionRight = pinDirectionRight;
    _timer.setMode(Timer0::Modes::PWM);
    _timer.setPrescaler(Timer0::Prescalers::CLK64);
    _timer.start();
}

void Motor::move(uint8_t cycle, uint8_t direction) {
    if (!direction) {
        PORTB &= ~( _pinDirectionLeft);
        PORTB &= ~( _pinDirectionRight);
    } 
    else {
        PORTB |= (_pinDirectionLeft) | (_pinDirectionRight);
    }

    moveLeft(cycle);
    moveRight(cycle);
}


void Motor::moveRight(uint8_t cycle) {
    _timer.setOCRB(cycle);
}

void Motor::moveLeft(uint8_t cycle) {
    _timer.setOCRA(cycle);
}

void Motor::changeDirection(uint8_t direction, Motor::Wheel wheel){
    uint8_t pinDirection = wheel == Motor::Wheel::RIGHT ? _pinDirectionRight : _pinDirectionLeft;

    if (!direction){
        PORTB &= ~pinDirection;
    } else {
        PORTB |= pinDirection;
    }
}

void Motor::resetDirection(){
    PORTB &= ~( _pinDirectionLeft);
    PORTB &= ~( _pinDirectionRight);
}

void Motor::turnRight90(){
    moveLeft(50);
    _delay_ms(DELAY_RIGHT_90);
    moveLeft(0);
}

void Motor::turnLeft90() {
    moveRight(50);
    _delay_ms(DELAY_LEFT_90);
    moveRight(0);
}

void Motor::stop() {
    move(0,0);
}