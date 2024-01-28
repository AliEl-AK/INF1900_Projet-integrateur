#include "common.h"

class LED{
    public:
        LED(Register mode, Register port, uint8_t pinGreen, uint8_t pinRed);
        void turnRed();
        void turnGreen();
        void turnOffPin(uint8_t pin);
        void turnOffAll();
    private:
        Register _port;
        uint8_t _pinRed;
        uint8_t _pinGreen;
};

