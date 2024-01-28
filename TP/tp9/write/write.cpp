#include <avr.h>
#include <common.h>
#include <led.h>
#include <memoire_24.h>
#include <MemoryDisplay.h>

void readByteCode()
{
    LED led(&DDRA, &PORTA, (1 << PA0), (1 << PA1));
    MemoryDisplay uart = MemoryDisplay();
    Memoire24CXXX memoire = Memoire24CXXX();

    uint16_t address = 0x00;
    uint8_t buffer1;
    uint8_t buffer2;

    led.turnGreen();
    _delay_ms(smallDELAY);

    buffer1 = uart.UARTReceive();
    memoire.ecriture(buffer1, 0);
    address++;
    buffer2 = uart.UARTReceive();
    memoire.ecriture(buffer2, 0);
    address++;

    uint16_t length = ((buffer1 << 8) | buffer2);
    uint8_t value;

    for (uint16_t i=2; i < length ; i++)
    {
        value = uart.UARTReceive();
        memoire.ecriture(address, value);
        address++;
        _delay_ms(smallDELAY);
    }
    led.turnOffAll();
}

int main()
{
    readByteCode();
    return 0;
}

// console :  serieViaUSB -e -f dance.byte -b