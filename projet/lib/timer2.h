#include <common.h>
#include <avr.h>

class Timer2 {
    public:
        enum class Prescalers {
            CLK1 = (1 << CS20),
            CLK8 = (1 << CS21),
            CLK32 = (1 << CS20) | (1 << CS21),
            CLK64 = (1 << CS22),
            CLK128 = (1 << CS22) | (1 <<CS20),
            CLK256 = (1 << CS22) | (1 << CS21),
            CLK1024 = (1 << CS22) | (1 << CS21) | (1 << CS20),
        };
        
        enum class Modes {NORMAL, CTC_1_COMPARE, CTC_2_COMPARE, PWM, CTC_SOUND};

        Timer2();
        ~Timer2();
        
        void resetMode();
        void resetPrescaler();
        void setMode(Modes mode);
        void setPrescaler(Prescalers prescaler);
        void start();
        void stop();
        void setOCRA(uint8_t cycle);
        void setOCRB(uint8_t cycle);
        uint8_t PWMRatioToCycle(uint8_t PWMRatio);
    private:
        uint8_t _prescaler;
};
