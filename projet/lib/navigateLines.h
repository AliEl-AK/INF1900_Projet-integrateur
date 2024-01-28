#pragma once
#include <common.h>
#include <avr.h>
#include <makerLine.h>
#include <distanceSensor.h>
#include <motor.h>
#include <timer1.h>
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"

#define DELAY_CORNER_MS 1150
#define DELAY_CORNER_CYCLE 13000
#define MAX_SPEED 100
#define SLOWED_SPEED 70
#define TURNING_SPEED_LEFT 80 //rightwheel
#define TURNING_SPEED_RIGHT 85 //leftwheel
#define OFFSET_DELAY 700
#define OFFSET_180 3000
#define INSTRUCTION_DELAY 500
#define TIME_BETWEEN_LINES_IN_CYCLE_PATHFINDING 20000

class NavigateLines {
    public:
        bool forceInt = false; //debug
        bool endAll = false;
        bool over = false;
        bool cornerHit = false;
        uint8_t position = 0;
        uint8_t currentInstruction = 0;
        enum class Directions {
            RIGHT,
            LEFT,
            STRAIGHT,
            TURN_180
        };
        NavigateLines(Motor* motor, MakerLine* makerLine, Timer1* chrono, DistanceSensor* distanceSensor);
        bool movePath(Directions directions[], uint8_t size);
        void moveStraight(bool ignoreCorners);
        void turnLeft();
        void turnRight();
        void turn180();
    private:
        Motor* _motor;
        MakerLine* _makerLine;
        Timer1* chrono;
        DistanceSensor* _distanceSensor;
        bool flagEnd = true;
        bool endTravel = false;
        uint8_t _speedLeft = MAX_SPEED;
        uint8_t _speedRight = MAX_SPEED;
        uint8_t _rightDetection = (1 << PA5) | (1 << PA6);
        uint8_t _leftDetection = (1 << PA4) | (1 << PA5);

};