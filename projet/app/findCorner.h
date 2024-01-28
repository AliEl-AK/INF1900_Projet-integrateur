#include <navigateLines.h>
#include <timer1.h>
#include <led.h>
#include <sound.h>
#include <avr.h>
#include <stdlib.h>
#include <common.h>
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"

#define TIME_BETWEEN_LINES_CYCLE 24000
#define DELAY_BETWEEN_INSTRUCTION 600
#define DELAY_FINAL_AND_START 1000

class FindCorner {
    public:
        uint8_t lines;
        bool orientationFound = true;
        bool doneFlashing = false;
        uint8_t ouestForLast = false;
        bool checkBottomRightCorner = false;
        enum class Corners {
            N_E = (1 << 3) | (1 << 2),
            N_O = (1 << 3) | (1 << 1),
            S_O = (1 << 2) | (1 << 1),
            S_E = (1 << 3)
        };
        FindCorner(NavigateLines &navigate, Timer1 &chrono, LED &led, Timer2* timer);
        void initChrono();
        void addLinesToCorner();
        void findCornerSequence();
    private:
        NavigateLines& navigate;
        Timer1& chrono;
        LED& led;
        Timer2* timer;
        uint8_t corner;
        uint8_t startDirection;

};