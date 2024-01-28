#include <distanceSensor.h>

distanceSensor::distanceSensor(can &convertAnaNum, uint8_t pinSensor1) 
: _convertAnaNum(convertAnaNum) {
    _pinSensor1 = pinSensor1;
}

uint16_t distanceSensor::findTension(){
    uint16_t tension = _convertAnaNum.lecture(_pinSensor1);
    tension = (tension >> 2);
    return tension;
}

bool distanceSensor::isCloseToPillar(){
    return (findTension() > 50);
}