#include <avr/io.h>
#include "string.h"

class MemoryDisplay{
    public:
      MemoryDisplay(void);
      
      void transmissionUART(uint8_t data);
      
      void communication_uart(const char words[]);
};

