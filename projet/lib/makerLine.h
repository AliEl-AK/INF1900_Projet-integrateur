
#include <common.h>
#include <avr.h>

class MakerLine{
    public:
        enum class Fix {
            LEFT,
            RIGHT,
            MID,
            NOTHING
        };
        MakerLine(Register mode, Register pin, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5);
        Fix isCentered();
        bool findCorner();
        uint8_t linesHovered();
    private:
        Register _pin;
        uint8_t _pin1;
        uint8_t _pin2;
        uint8_t _pin3;
        uint8_t _pin4;
        uint8_t _pin5;
};