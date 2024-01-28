#ifndef AFFICHAGEMEMOIRE_H
#define AFFICHAGEMEMOIRE_H

#include <avr/io.h>
#include "string.h"

class MemoryDisplay{
    public:
      void initialisationUART(void);
      void transmissionUART(uint8_t data);
      //Pour transmettre une chaîne constantes
      void log_uart(const char words[]);
};

#endif
