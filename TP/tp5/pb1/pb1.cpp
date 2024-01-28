#include <avr/io.h>
#define F_CPU 8000000
#include <avr/interrupt.h>
#include <util/delay.h>
#include "memoire_24.h"

int main()
{
    DDRA = 0xff; // sortie
    DDRD = 0x00; // entree
    Memoire24CXXX memory;
    bool estPareil = true;
    const uint16_t adress = 0x0000;
    unsigned char messageEnvoyer[46] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
    const uint8_t sizeEnvoyer = sizeof(messageEnvoyer);
    while (true)
    {
    unsigned char messageRecu[sizeEnvoyer];
        

        memory.ecriture(adress,messageEnvoyer, sizeEnvoyer);
        _delay_ms(5);
        memory.lecture(adress, messageRecu, sizeEnvoyer);

        for (int i = 0; i < 46 && estPareil; i++)
        {
            if (messageRecu[i] != messageEnvoyer[i])
                PORTA = (1<< PORTA0); //Rouge
            else
                PORTA = (1<< PORTA1); // Vert 
        }
    }
    return 0;
}
