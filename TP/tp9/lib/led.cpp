#include <led.h>


LED::LED(Register mode, Register port, uint8_t pinGreen, uint8_t pinRed) {
    _port = port;
    _pinGreen = pinGreen;
    _pinRed = pinRed;
    *mode |= pinGreen | pinRed;
}

void LED::turnRed(){
    turnOffPin(_pinGreen);
    *_port |= _pinRed;   
}

void LED::turnGreen(){
    turnOffPin(_pinRed);
    *_port |= _pinGreen;
}
void LED::turnYellow(){
    turnRed();
    _delay_ms(DELAY);
    turnGreen();
    _delay_ms(DELAY);
}

void LED::turnOffPin(uint8_t pin){
    *_port &= ~pin;
}

void LED::turnOffAll(){
    turnOffPin(_pinGreen);
    turnOffPin(_pinRed);
}
