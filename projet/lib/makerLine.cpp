#include <makerLine.h>

MakerLine::MakerLine(Register mode, Register pin, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5) 
 : _pin(pin), _pin1(pin1), _pin2(pin2), _pin3(pin3), _pin4(pin4), _pin5(pin5) {
    *mode &= ~_pin1;
    *mode &= ~_pin2;
    *mode &= ~_pin3;
    *mode &= ~_pin4;
    *mode &= ~_pin5;
}

MakerLine::Fix MakerLine::isCentered(){
    const uint8_t mask_left = _pin1 | _pin2;
    const uint8_t mask_middle = _pin3;
    const uint8_t mask_right = _pin4 | _pin5;

    if (mask_left & *_pin){
        return MakerLine::Fix::LEFT;
    }
    if (mask_right & *_pin) {
        return MakerLine::Fix::RIGHT;
    }
    if (mask_middle & *_pin) {
        return MakerLine::Fix::MID;
    }

    return MakerLine::Fix::NOTHING;
}

bool MakerLine::findCorner(){
    uint8_t linesHovered = 0;

    if (_pin1 & *_pin){
        linesHovered++;
    }
    if (_pin2 & *_pin){
        linesHovered++;
    }
    if (_pin3 & *_pin){
        linesHovered ++;
    }
    if (_pin4 & *_pin) {
        linesHovered ++;
    }
    if (_pin5 & *_pin){
        linesHovered++;
    }

    return linesHovered >= 3;
}


uint8_t MakerLine::linesHovered(){
    return *_pin & (_pin1 | _pin2 | _pin3 | _pin4 | _pin5);
}