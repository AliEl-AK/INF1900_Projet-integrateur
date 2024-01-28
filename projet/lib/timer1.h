#pragma once
#include <common.h>
#include <avr.h>

class Timer1 {
    public:
        enum class Prescalers {
            CLK1 = (1 << CS10),
            CLK8 = (1 << CS11),
            CLK64 = (1 << CS10) | (1 << CS11),
            CLK256 = (1 << CS12),
            CLK1024 = (1 << CS12) | (1 << CS10)
        };

        enum class Modes {
            NORMAL,
            CTC_1_COMPARE,
            CTC_2_COMPARE,
            PWM
        };

        Timer1();
        ~Timer1();
        void resetMode();
        void resetPrescaler();
        void setMode(Modes mode);
        void setPrescaler(Prescalers prescaler);
        void start();
        void stop();
        void setOCRA(uint16_t cycle);
        void setOCRB(uint16_t cycle);
    private:
        uint8_t _prescaler;
};
