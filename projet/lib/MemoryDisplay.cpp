#include <MemoryDisplay.h>

MemoryDisplay::MemoryDisplay(void)
{
    //2400 bauds.Nous vous donnons la valeur des deux
    // premiers registres pour vous éviter des complications.
    UBRR0H = 0;
    UBRR0L = 0xCF;
    // permettre la réception et la transmission par le UART0
    // UCSR0A = (1 << RXC0)| (1 << TXC0);
    UCSR0A |= (1 << MPCM0);
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    // Format des trames: 8 bits, 1 stop bits, sans parité
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void MemoryDisplay::transmissionUART(uint8_t data)
{
  /* Wait for empty transmit buffer */
  while ( !( UCSR0A & (1<<UDRE0)))  
  ;
  /* Put data into buffer, sends the data */
  UDR0 = data;
}

//Returns the data loaded into the receive buffer.
uint8_t MemoryDisplay::UARTReceive(void)
{

  /* Wait for empty transmit buffer */
  while ( !( UCSR0A & (1<<RXC0)));
  /* Get and return received data from buffer */
  return UDR0;
}


void MemoryDisplay::communication_uart(const char words[]) {
    for (uint8_t i = 0; i < strlen(words); i++)
    {

        unsigned char character = words[i];
        transmissionUART(character);
        if (character == '\0')
        {
            break;
        }
    }
}

