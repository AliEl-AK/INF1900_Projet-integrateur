#include <avr.h>
#include <common.h>
#include <timer2.h>

class Sound{
    public:
        Sound(Timer2 &timer);
        void play(uint8_t note);
        void stop();
    private:
        Timer2 &_timer;
};



