#include <common.h>
#include <avr.h>
#include <led.h>
#include <button.h>

LED led = LED(&DDRA, &PORTA, (1 << PA0), (1 << PA1));
Button boutonSelection(Button::Interruptions::INTER0, Button::Edges::FALLING);

ISR(INT0_vect){
    led.turnGreen();
}

int main(){
    
    while(true){
    }
    return 0;
}

