#include <common.h>
#include <avr.h>
#include <timer0.h>

#define DELAY_RIGHT_90 2000
#define DELAY_LEFT_90 1900

class Motor {
    public:
        Motor(Timer0 &timer, uint8_t pinDirectionLeft, uint8_t pinDirectionRight);
        void move(uint8_t cycle, uint8_t direction);
        void moveRight(uint8_t cycle);
        void moveLeft(uint8_t cycle);
        void turnRight90();
        void turnLeft90();
        void stop();
    private:
        Timer0 &_timer;
        uint8_t _pinDirectionRight;
        uint8_t _pinDirectionLeft;
};