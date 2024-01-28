#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include "can.h"

const int lumierBasse = 0xA0; // 160
const int lumierForte = 0xE6; // 230

const int rouge= 0x01; 
const int vert= 0x02;
const int off= 0x00;

int main()
{
   DDRA = 0x00;
   DDRB = 0xff;
   uint16_t lecture;
   can convert;
   while (true)
   {
      lecture = convert.lecture(PORTA0);
      lecture = lecture >> 2; 
      if (lecture < lumierBasse)
         PORTB = vert;
      else
      {
         if (lecture < lumierForte && lecture > lumierBasse)
         {
            PORTB = vert;
            _delay_ms (10);
            PORTB = rouge;
            _delay_ms(10);
         }
         else 
         PORTB = rouge;
      }
   }
   return 0;
}