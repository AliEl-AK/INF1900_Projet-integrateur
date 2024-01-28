#include <common.h>
#include <avr.h>
#include <can.h>

class distanceSensor{
    public:
        distanceSensor(can &convertAnaNum, uint8_t pinSensor1);
        uint16_t findTension();
        bool isCloseToPillar();
    private:
        can &_convertAnaNum;
        uint8_t _pinSensor1;
};