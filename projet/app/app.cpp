

#include <findCorner.h>
#include <button.h>
#include <pathfinder.h>

//Global variables
volatile bool cornerButtonPressed = false;
volatile bool pathButtonPressed = false;
volatile uint8_t count;
volatile bool turnOff = true;
volatile uint8_t position = 0;
volatile bool startInit = false;

volatile Pathfinder::Orientations curDirection = Pathfinder::Orientations::SOUTH;

const uint16_t DELAY_FIND_POINT = 2400;
const uint8_t OFFSET_ACTIONPATH = 3;

uint8_t targetLocation = 0;
Pathfinder::Orientations lastDirection;

enum class Mode {
    FINDCORNER,
    FINDPATH
};

Mode mode;

void uint8ToString(uint8_t value, char* buffer, size_t size) {
    // Ensure buffer is not null and has enough space
    if (buffer != nullptr && size > 0) {
        // Convert each digit of the number to characters
        for (size_t i = size - 1; i > 0; --i) {
            buffer[i - 1] = '0' + value % 10;
            value /= 10;
        }
        buffer[size - 1] = '\0';  // Null-terminate the string
    }
}


//Declaration objects
Timer0 timer;
Timer1 chrono;
Timer2 timerSound;
can convertAnaNum;
DistanceSensor distanceSensor(convertAnaNum, 2);
LED led(&DDRA, &PORTA, (1 << PA0), (1 << PA1));
Motor motor(timer, (1 << PB2), (1 << PB5));
MakerLine makerLine(&DDRA, &PINA, (1 << PA3), (1 << PA4), (1 << PA5), (1 << PA6), (1 << PA7));
NavigateLines navigate(&motor, &makerLine, &chrono, &distanceSensor);
LCM disp(&LCD_DDR, &LCD_PORT);

Button buttonFindCorner(Button::Interruptions::INTER0, Button::Edges::RISING);
Button buttonSelection(Button::Interruptions::INTER1, Button::Edges::FALLING);

FindCorner find(navigate, chrono, led, &timerSound);
Pathfinder path;

ISR(TIMER1_COMPA_vect) {
    switch(mode){
        case Mode::FINDCORNER:
            if (find.checkBottomRightCorner){
                find.ouestForLast = true;
            } else {
                find.lines++;
            }
            break;
        case Mode::FINDPATH:
            if (!navigate.cornerHit){
                uint8_t curAction = navigate.currentInstruction;
                if (path.actionPath[curAction] == NavigateLines::Directions::STRAIGHT){
                    navigate.position += static_cast<int>(path.directionMap[curAction]);
                    lastDirection = path.directionMap[curAction];
                    if (navigate.position == targetLocation){
                        navigate.endAll = true;
                    }
                }
            } else {
                navigate.over = true;
            }
            break;
    }
}

ISR(TIMER2_COMPA_vect){
    switch(mode){
        case Mode::FINDCORNER:
            if (!(find.doneFlashing)){
                count++;
                if (count == 10){
                    turnOff = !turnOff;
                    count = 0;
                }

                if (turnOff){
                    led.turnOffAll();
                } else{
                    led.turnGreen();
                }
            }
            break;
        case Mode::FINDPATH:
            break;
    }
}


ISR(INT0_vect){
    buttonFindCorner.debouncer();
    cornerButtonPressed = true;
}

ISR(INT1_vect){
    buttonSelection.debouncer();
    if (!startInit){
        pathButtonPressed = true;
    } else {
        switch(path.currentAddState){
            case Pathfinder::AddState::ROW:
                if(path.buttonFlag){
                    if (path.row == 4){
                        path.row = 1;
                    } else {
                        path.row++;
                    }
                    path.rowFlag = true;
                    path.buttonFlag = false;
                }
                break;
            case Pathfinder::AddState::COL:
                if (path.buttonFlag){
                    if (path.col == 7){
                        path.col = 1;
                    } else {
                        path.col++;
                    }
                    path.colFlag = true;
                    path.buttonFlag = false;
                }
                break;
            case Pathfinder::AddState::VAL:
                if (path.buttonFlag){
                    path.isYes = !path.isYes;
                    path.valFlag = true;
                    path.buttonFlag = false;
                }
                break;
        }
    }
}

int main(){
    led.turnRed();
    while(1){
        while (!cornerButtonPressed && !pathButtonPressed){
            //DO NOTHING
        }
        
        led.turnOffAll();

        if (pathButtonPressed){
            mode = Mode::FINDPATH;
            navigate.position = 0;
            Sound sound = Sound(timerSound);
            while (1){
                disp.clear();
                startInit = true;
                targetLocation = path.init();
                disp.clear();
                disp.write("OK!!!");
                _delay_ms(2000);
                int size = path.convertToActions(navigate.position, targetLocation, curDirection);
                bool isDone = navigate.movePath(path.actionPath, size + OFFSET_ACTIONPATH);
                if (!isDone){
                    sound.play(45);
                    _delay_ms(2000);
                    sound.stop();
                    lastDirection = path.directionMap[navigate.currentInstruction];
                    int pillarPosition = navigate.position + static_cast<int>(lastDirection);
                    path.deleteNode(pillarPosition);
                    navigate.currentInstruction = 0;
                    size = path.convertToActions(navigate.position, targetLocation, lastDirection);
                    navigate.movePath(path.actionPath, size + OFFSET_ACTIONPATH);
                }
                sound.play(80);
                _delay_ms(200);
                sound.stop();
                _delay_ms(100);
                sound.play(80);
                _delay_ms(200);
                sound.stop();
                _delay_ms(100);
                sound.play(80);
                _delay_ms(200);
                sound.stop();
                _delay_ms(100);
                sound.play(80);
                _delay_ms(200);
                sound.stop();
                _delay_ms(100);
                sound.play(80);
                _delay_ms(200);
                sound.stop();
                _delay_ms(100);
                curDirection = path.directionMap[navigate.currentInstruction];
                navigate.currentInstruction = 0;
            }
        } else {
            find.findCornerSequence();
        }
        break;
    }
}