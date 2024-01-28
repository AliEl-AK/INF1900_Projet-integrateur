#include <common.h>
#include <avr.h>


//Singleton: Can only have one instance of this class
class Timer0{
    public:
        enum class Prescalers {
            CLK1 = (1 << CS00),
            CLK8 = (1 << CS01),
            CLK64 = (1 << CS00) | (1 << CS01),
            CLK256 = (1 << CS02),
            CLK1024 = (1 << CS02) | (1 << CS00)
        };

        enum class Modes{NORMAL, CTC_1_COMPARE, CTC_2_COMPARE, PWM};
        Timer0();
        ~Timer0();

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