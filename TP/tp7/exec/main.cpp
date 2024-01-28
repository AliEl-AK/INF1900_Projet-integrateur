#include <button.h>
#include <led.h>
#include <avr.h>
#include <motor.h>

//Simple test de button et led
Button button(Button::Interruptions::INTER1, Button::Edges::BOTH);
LED led(&DDRA, &PORTA, (1 << PA0), (1 << PA1));
volatile uint8_t buttonClicked = 0;

ISR(INT1_vect){
    button.debouncer();
    buttonClicked = !buttonClicked;
}

int main()
{
    while(1){
        if (buttonClicked){
            led.turnGreen();
        } else {
            led.turnRed();
        }
    }
}