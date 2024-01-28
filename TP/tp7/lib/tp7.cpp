#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "timer1.h"
#include "led.h"
#include "AffichageMemoire.h"



volatile uint8_t timerExpired = 0;
LED led(&DDRA, &PORTA, (1 << PA0), (1 << PA1));
Timer1 timer;

ISR(TIMER1_COMPA_vect){
    timerExpired = 1;
}


int main(){
    
    timer.changeMode(Timer1::Modes::CTC_1_COMPARE);
    timer.changePrescalar(Timer1::Prescalars::CLK1024);
    timer.setOCRA(63000);
    timer.start();
    while(1) {
        if (timerExpired){
            led.turnGreen();
        }
    }

    /*initialisationUART();
    char mots[] = "test1: Le robot en INF1900\n";

    log_uart(mots);*/


}
