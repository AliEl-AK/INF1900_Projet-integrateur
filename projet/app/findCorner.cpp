#include <findCorner.h>

FindCorner::FindCorner(NavigateLines &navigate, Timer1 &chrono, LED &led, Timer2* timer) : navigate(navigate), chrono(chrono), led(led), timer(timer) {
    corner = 0;
    lines = 0;
    startDirection = 0;
}

void FindCorner::initChrono(){
    chrono.setMode(Timer1::Modes::CTC_1_COMPARE);
    chrono.setPrescaler(Timer1::Prescalers::CLK1024);
}

void FindCorner::addLinesToCorner(){
    chrono.stop();
    if (lines == 0){
        lines = 1;
    } else if(lines > 3){
        lines = 3;
    }
    if (orientationFound){
        startDirection = lines;
        orientationFound = false;
    }

    corner |= (1 << lines);
    lines = 0;
}

void FindCorner::findCornerSequence(){
    LCM disp(&LCD_DDR, &LCD_PORT);
    initChrono();
    timer->setMode(Timer2::Modes::CTC_1_COMPARE);
    timer->setPrescaler(Timer2::Prescalers::CLK1024);
    timer->setOCRA(255);
    timer->start();
    chrono.setOCRA(TIME_BETWEEN_LINES_CYCLE);
    _delay_ms(DELAY_FINAL_AND_START);
    chrono.start();
    navigate.moveStraight(true);
    addLinesToCorner();
    _delay_ms(DELAY_BETWEEN_INSTRUCTION);
    navigate.turn180();
    _delay_ms(DELAY_BETWEEN_INSTRUCTION);
    navigate.moveStraight(true);
    _delay_ms(DELAY_BETWEEN_INSTRUCTION);
    navigate.turn180();
    _delay_ms(DELAY_BETWEEN_INSTRUCTION);
    chrono.setOCRA(25906);
    checkBottomRightCorner = true;
    chrono.start();
    navigate.turnRight();
    chrono.stop();
    checkBottomRightCorner = false;
    _delay_ms(DELAY_BETWEEN_INSTRUCTION);
    chrono.setOCRA(TIME_BETWEEN_LINES_CYCLE);
    chrono.start();
    navigate.moveStraight(true);
    addLinesToCorner();
    FindCorner::Corners finalCorner = static_cast<FindCorner::Corners>(corner);
    switch(finalCorner){
        disp.clear();
        case FindCorner::Corners::N_O:
            disp.write("(1, 1)");
            if (startDirection == 1){
                disp.write("SUD", 16);
            } else {
                disp.write("EST", 16);
            }
            break;
        case FindCorner::Corners::N_E:
            disp.write("1, 7");
            if (startDirection == 2) {
                disp.write("OUEST", 16);
            } else {
                disp.write("SUD", 16);
            }
            break;
        case FindCorner::Corners::S_O:
            disp.write("4, 1");
            if (startDirection == 1) {
                disp.write("NORD", 16);
            } else {
                disp.write("EST", 16);
            }
            break;
        case FindCorner::Corners::S_E:
            disp.write("4, 7");
            if (ouestForLast){
                disp.write("OUEST", 16);
            } else {
                disp.write("NORD", 16);
            }
            break;
        default:
            disp.write("ERROR");
            break;
    };
    doneFlashing = true;
    led.turnGreen();
    Sound sound(*timer);
    sound.play(80);
    _delay_ms(DELAY_FINAL_AND_START);
    sound.stop();
    navigate.turn180();
    _delay_ms(DELAY_BETWEEN_INSTRUCTION);
    navigate.moveStraight(true);
    _delay_ms(DELAY_BETWEEN_INSTRUCTION);
    navigate.turn180();
    _delay_ms(DELAY_BETWEEN_INSTRUCTION);
    navigate.turnRight();
}