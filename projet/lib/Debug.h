#include <avr/io.h>
#include <string.h>

#ifdef DEBUG
#define DEBUG_PRINT(x) communication_uart(x)
#define DEBUG_TRANSMIT(str) transmissionUART(x)

#else
#define DEBUG_PRINT(x) do {} while (0) 
#define DEBUG_TRANSMIT(x) do{} while(0)
#endif

void transmissionUART(uint8_t data)
{
  /* Wait for empty transmit buffer */
  while ( !( UCSR0A & (1<<UDRE0)))  
  ;
  /* Put data into buffer, sends the data */
  UDR0 = data;
}

void communication_uart(const char words[]) {
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
