#include <sound.h>

const uint8_t OCRA_FREQUENCY[] = {141, 133, 126, 118, 112, 105, 99, 94, 88, 83, 79, 74, 70, 66, 62, 59, 55, 52, 49, 46, 44, 41, 39, 37, 35, 
33, 31, 29, 27, 26, 24, 23, 21, 20, 19, 18, 17};

Sound::Sound(Timer2 &timer): _timer(timer){
    _timer.setMode(Timer2::Modes::CTC_SOUND);
    _timer.setPrescaler(Timer2::Prescalers::CLK256);
    _timer.setOCRA(0);
}

void Sound::play(uint8_t note){
    uint8_t index = note - 45;
    uint8_t OCR = OCRA_FREQUENCY[index];
    _timer.setOCRA(OCR);
    _timer.start();
}

void Sound::stop(){
    _timer.stop();
}