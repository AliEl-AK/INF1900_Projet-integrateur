#include <navigateLines.h>
#include <timer1.h>
#include <led.h>
#include <sound.h>

Timer0 timer;
Timer1 chrono;
Timer2 timerSound;
Sound sound(timerSound);
LED led(&DDRA, &PORTA, (1 << PA0), (1 << PA1));
Motor motor(timer, (1 << PB2), (1 << PB5));
MakerLine makerLine(&DDRA, &PINA, (1 << PA3), (1 << PA4), (1 << PA5), (1 << PA6), (1 << PA7));
NavigateLines navigate(&motor, &makerLine);
volatile uint8_t lines = 0;
uint8_t corner;
uint8_t startDirection;
const uint16_t DELAY_LINE = 15625;
const uint16_t DELAY_BETWEEN_INSTRUCTION = 1000;

enum class Corners {
    N_E = (1 << 3) | (1 << 2),
    N_O = (1 << 3) | (1 << 1),
    S_O = (1 << 2) | (1 << 1),
    S_E = (1 << 3)
};

ISR(TIMER1_COMPA_vect) {
    lines++;
}

void initChrono(){
    chrono.setMode(Timer1::Modes::CTC_1_COMPARE);
    chrono.setPrescaler(Timer1::Prescalers::CLK1024);
}

void addLinesToCorner(){
    chrono.stop();
    corner |= (1 << lines);
    lines = 0;
}

int main(){
    initChrono();
    chrono.setOCRA(DELAY_LINE);
    _delay_ms(2000);
    chrono.start();
    navigate.moveStraight(true);
    startDirection = lines;
    addLinesToCorner();
    _delay_ms(DELAY_BETWEEN_INSTRUCTION);
    navigate.turn180();
    _delay_ms(DELAY_BETWEEN_INSTRUCTION);
    navigate.moveStraight(true);
    _delay_ms(DELAY_BETWEEN_INSTRUCTION);
    navigate.turn180();
    _delay_ms(DELAY_BETWEEN_INSTRUCTION);
    navigate.turnRight();
    _delay_ms(DELAY_BETWEEN_INSTRUCTION);
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
    navigate.turnRight();

    Corners finalCorner = static_cast<Corners>(corner);
    switch(finalCorner){
        case Corners::N_O:
            led.turnGreen();
            break;
        case Corners::N_E:
            led.turnRed();
            break;
        case Corners::S_O:
            led.turnOffAll();
            break;
        case Corners::S_E:
            sound.play(60);
            break;
        default:
            led.turnGreen();
            _delay_ms(500);
            led.turnRed();
            _delay_ms(500);
            led.turnGreen();
            _delay_ms(500);
            led.turnRed();
            _delay_ms(500);
            led.turnGreen();
            _delay_ms(500);
            led.turnRed();
            _delay_ms(500);
            break;
    };
    _delay_ms(DELAY_BETWEEN_INSTRUCTION);
    sound.stop();
    return 0;
}