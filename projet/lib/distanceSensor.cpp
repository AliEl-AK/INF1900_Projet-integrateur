#include <distanceSensor.h>

DistanceSensor::DistanceSensor(can &convertAnaNum, uint8_t pinSensor1) 
: _convertAnaNum(convertAnaNum) {
    _pinSensor1 = pinSensor1;
}

uint16_t DistanceSensor::findTension(){
    uint16_t tension = _convertAnaNum.lecture(_pinSensor1);
    tension = (tension >> 2);
    return tension;
}

bool DistanceSensor::isCloseToPillar(){
    return (findTension() > TOP_SENSOR_DISTANCE);
}