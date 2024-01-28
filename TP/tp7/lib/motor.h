#include "common.h"
#include "timer0.h"

class Motor {
    public:
        Motor(Timer0 &timer, uint8_t pinDirectionLeft, uint8_t pinDirectionRight);
        void move(uint8_t percentage, uint8_t direction);
        void moveRight(uint8_t percentage);
        void moveLeft(uint8_t percentage);
        void stop();
        
    private:
        Timer0 &_timer;
        uint8_t _pinDirectionRight;
        uint8_t _pinDirectionLeft;
};