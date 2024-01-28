#include <avr.h>
#include <common.h>
#include <led.h>
#include <memoire_24.h>
#include <motor.h>
#include <MemoryDisplay.h>
#include <sound.h>

struct Robot{

    Timer0 timer0;
    Timer2 timer2;
    LED led = LED(&DDRA, &PORTA, (1 << PA0), (1 << PA1));
    Motor motor = Motor(timer0, (1 << PB2), (1 << PB5));
    Sound sound = Sound(timer2);

    uint8_t nbLoop = 0;
    uint16_t programmeCounter = 0;
};

Robot robot;
uint8_t speed;
const uint8_t START_BYTE = 2;
uint8_t address = 0x00;

enum class Commandes{
    INIT = 0,
    DBT = 0x01,
    ATT = 0x02,
    DAL = 0x44,
    DET = 0x45,
    SGO = 0x48,
    SAR = 0x09,
    MAR = 0x60,
    MAR1 = 0x61,
    MAV = 0x62,
    MRE = 0x63,
    TRD = 0x64,
    TRG = 0x65,
    DBC = 0xC0,
    FBC = 0xC1,
    FIN = 0xFF,
};


void execution(Commandes action, uint8_t operand)
{
    switch (action)
    {
    case Commandes::ATT:
        for (uint8_t i = 0; i < operand; i++) {
            _delay_ms(operandDelay);
        };
        break;

    case Commandes::DAL:
        if (operand == 1) {
            robot.led.turnGreen();
        }
        else if (operand == 2) {
            robot.led.turnRed();
        };
        break;

    case Commandes::DET:
        robot.led.turnOffAll();
        break;

    case Commandes::SGO:
        robot.sound.play(operand);
        break;

    case Commandes::SAR:
        robot.sound.stop();
        break;

    case Commandes::MAR:
    case Commandes::MAR1:
        robot.motor.stop();
        break;
    case Commandes::MAV:
        robot.motor.move(operand, 0);
        break;
    case Commandes::MRE:
        robot.motor.stop();
        break;
    case Commandes::MAV:
        robot.motor.move(operand, 0);
        break;
    case Commandes::MRE:
        robot.motor.move(operand, 1);
        break;
    case Commandes::TRD:
        robot.motor.turnRight90();
        break;

    case Commandes::TRG:
        robot.motor.turnLeft90();
        break;

    case Commandes::DBC:
        break;

    case Commandes::TRG:
        robot.motor.turnLeft90();
        break;

    case Commandes::DBC:
        robot.nbLoop = operand;
        robot.programmeCounter = (
            address
        );
        break;

    case Commandes::FBC:
        if (--robot.nbLoop != 0) {
            address = robot.programmeCounter;
        };
        break;

    case Commandes::FIN:
        robot.motor.stop();
        robot.sound.stop();
        robot.led.turnOffAll();
        break;
    default:
        break;
    }
}

int main()
{
    Memoire24CXXX memory;
    MemoryDisplay uart;
    Commandes action = Commandes::INIT;

    uint8_t operand;
    uint8_t opcode;

    address = address + START_BYTE;

    while (action != Commandes::DBT) {
        memory.lecture(address, &opcode);
        address++;
        memory.lecture(address, &operand);
        address++;

        action = static_cast<Commandes>(opcode);
    }

    while (action != Commandes::FIN) {
        memory.lecture(address, &opcode);
        address++;
        memory.lecture(address, &operand);
        address++;

        action = static_cast<Commandes>(opcode);
        execution(action, operand);
    }
    
    return 0; 
}