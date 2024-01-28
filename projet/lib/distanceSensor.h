//TODO: Classe qui s'occupe des capteurs de distance
#include <common.h>
#include <avr.h>
#include <can.h>
#define TOP_SENSOR_DISTANCE 100

class DistanceSensor{
    public:
        DistanceSensor(can &convertAnaNum, uint8_t pinSensor1);
        uint16_t findTension();
        bool isCloseToPillar();
    private:
        can &_convertAnaNum;
        uint8_t _pinSensor1;
};