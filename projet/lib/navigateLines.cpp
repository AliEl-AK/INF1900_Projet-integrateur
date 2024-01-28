#include <navigateLines.h>

//WHEN FULLY CHARGED, DELAY BETWEEN LINES AROUND 2.5/3s


NavigateLines::NavigateLines(Motor* motor, MakerLine* makerLine, Timer1* chrono, DistanceSensor* distanceSensor)
: _motor(motor), _makerLine(makerLine), chrono(chrono), _distanceSensor(distanceSensor){
    chrono->setMode(Timer1::Modes::CTC_1_COMPARE);
    chrono->setPrescaler(Timer1::Prescalers::CLK1024);
    chrono->setOCRA(TIME_BETWEEN_LINES_IN_CYCLE_PATHFINDING);
}

void NavigateLines::moveStraight(bool ignoreCorners){
    while(1){
        MakerLine::Fix fixWheel;
        fixWheel = _makerLine->isCentered();
        if (_makerLine->findCorner()){
            fixWheel = MakerLine::Fix::MID;
        }

        _motor->moveRight(_speedRight);
        _motor->moveLeft(_speedLeft);

        if (fixWheel == MakerLine::Fix::RIGHT){
            _speedLeft = SLOWED_SPEED;
            _speedRight = MAX_SPEED;
        } else if (fixWheel == MakerLine::Fix::LEFT){
            _speedRight = SLOWED_SPEED;
            _speedLeft = MAX_SPEED;
        } else if (fixWheel == MakerLine::Fix::MID){
            _speedLeft = MAX_SPEED;
            _speedRight = MAX_SPEED;
        }

        if (over || endAll) {
            _motor->stop();
            break;
        }

        if (_distanceSensor->isCloseToPillar() && (!cornerHit)) {
            _motor->stop();
            chrono->stop();
            endTravel = true;
            break;
        }

        if (!ignoreCorners){
            if (_makerLine->findCorner()){
                chrono->stop();
                chrono->setOCRA(DELAY_CORNER_CYCLE);
                chrono->start();
                cornerHit = true;
            } else if (_makerLine->linesHovered() == 0) {
                chrono->stop();
                _motor->move(MAX_SPEED, 0);
                _delay_ms(DELAY_CORNER_MS);
                _motor->stop();
                break;
            }
        } else {
            if (_makerLine->linesHovered() == 0){
                _motor->move(MAX_SPEED, 0);
                _delay_ms(DELAY_CORNER_MS);
                _motor->stop();
                break;
            }
        }
    }
}

void NavigateLines::turnLeft(){
    _motor->moveRight(TOP);
    _motor->changeDirection(1, Motor::Wheel::LEFT);
    _motor->moveLeft(TOP);
    _delay_ms(100);
    _motor->moveRight(TURNING_SPEED_RIGHT);
    _motor->moveLeft(TURNING_SPEED_LEFT);
    _delay_ms(OFFSET_DELAY);
    while (_makerLine->linesHovered() != _rightDetection){

    }
    _motor->resetDirection();
    _motor->stop();
}

void NavigateLines::turnRight(){
    _motor->moveLeft(TOP);
    _motor->changeDirection(1, Motor::Wheel::RIGHT);
    _motor->moveRight(TOP);
    _delay_ms(100);
    _motor->moveLeft(TURNING_SPEED_LEFT);
    _motor->moveRight(TURNING_SPEED_RIGHT);
    _delay_ms(OFFSET_DELAY);
    while(_makerLine->linesHovered() != _leftDetection){

    }
    _motor->resetDirection();
    _motor->stop();
}

void NavigateLines::turn180(){
    _motor->moveLeft(TOP);
    _motor->changeDirection(1, Motor::Wheel::RIGHT);
    _motor->moveRight(TOP);
    _delay_ms(100);
    _motor->moveLeft(TURNING_SPEED_LEFT);
    _motor->changeDirection(1, Motor::Wheel::RIGHT);
    _motor->moveRight(TURNING_SPEED_RIGHT);
    _delay_ms(OFFSET_180);
    while (_makerLine->linesHovered() != _leftDetection && _makerLine->linesHovered() != _rightDetection){

    }
    _motor->resetDirection();
    _motor->stop();
}

bool NavigateLines::movePath(Directions directions[], uint8_t size){
    uint8_t oldPosition = 0;
    flagEnd = true;
    LCM disp(&LCD_DDR, &LCD_PORT);
    for (uint8_t i=0; i < size; i++){
        chrono->start();
        oldPosition = position;
        Directions currentDirection = directions[i];
        switch(currentDirection){
            case NavigateLines::Directions::STRAIGHT:
                moveStraight(false);
                if (oldPosition == position && (!endTravel)) {
                    cornerHit = false;
                    flagEnd = false;
                    chrono->setOCRA(3900);
                    chrono->start();
                    _delay_ms(700);
                    chrono->setOCRA(TIME_BETWEEN_LINES_IN_CYCLE_PATHFINDING);
                }
                break;
            case NavigateLines::Directions::RIGHT:
                turnLeft();
                break;
            case NavigateLines::Directions::LEFT:
                turnRight();
                break;
            case NavigateLines::Directions::TURN_180:
                turn180();
                break;
            default:
                break;
        }
        if (endTravel) {
            endTravel = false;
            return false;
        }
        if (endAll) {
            chrono->stop();
            endAll = false;
            if (flagEnd){
                _motor->move(MAX_SPEED, 0);
                _delay_ms(1800);
                _motor->stop();
            }
            return true;
        }
        currentInstruction++;
        chrono->stop();
        chrono->setOCRA(TIME_BETWEEN_LINES_IN_CYCLE_PATHFINDING);
        cornerHit = false;
        over = false;
        forceInt = false;
        _delay_ms(INSTRUCTION_DELAY);
        flagEnd = true;
    }
    return true;
}
