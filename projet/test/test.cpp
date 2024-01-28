
#include <led.h>
#include <avr.h>
#include <timer1.h>
#include <sound.h>
#include <navigateLines.h>

Timer0 timer;
Timer1 chrono;
Timer2 timer2;
Sound sound(timer2);
MakerLine makerLine(&DDRA, &PINA, (1 << PA3), (1 << PA4), (1 << PA5), (1 << PA6), (1 << PA7));
LED led(&DDRA, &PORTA, (1 << PA0), (1 << PA1));
Motor motor(timer, (1 << PB2), (1 << PB5));
NavigateLines navigate(&motor, &makerLine);


NavigateLines::Directions path[13] = {
    NavigateLines::Directions::STRAIGHT,
    NavigateLines::Directions::RIGHT,
    NavigateLines::Directions::STRAIGHT,
    NavigateLines::Directions::STRAIGHT,
    NavigateLines::Directions::LEFT,
    NavigateLines::Directions::STRAIGHT,
    NavigateLines::Directions::STRAIGHT,
    NavigateLines::Directions::LEFT,
    NavigateLines::Directions::STRAIGHT,
    NavigateLines::Directions::RIGHT,
    NavigateLines::Directions::STRAIGHT,
    NavigateLines::Directions::LEFT,
    NavigateLines::Directions::STRAIGHT
};

int main(){
    navigate.movePath(path, 13);
}

