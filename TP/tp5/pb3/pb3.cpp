#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include <memoire_24.h>

const int MODE_SORTIE = 0xff;

void initialisationUART ( void ) {

    // 2400 bauds. Nous vous donnons la valeur des deux

    // premier registres pour vous éviter des complications

    UBRR0H = 0;

    UBRR0L = 0xCF;

    // permettre la reception et la transmission par le UART0

    UCSR0A = 0;

    UCSR0B = (1 << RXEN0) | (1 << TXEN0) ; // RXEN0 = Reciever enable, TXEN0 = Transmission enable

    // Format des trames: 8 bits, 1 stop bits, none parity

    UCSR0C = (1 << UCSZ11) | (1 << UCSZ10); // UCSZ11 & UCSZ10 : Character size a 8-bit

}

void transmissionUART ( uint8_t donnee ) {
    while (!((UCSR0A) & (1 << UDRE0))){        
    }
    UDR0 = donnee; 
}

/*void ecrireMessageMemoire(Memoire24CXXX* memoire) {
    char* tableEcrite = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
    const int longueur = 45;
    
    memoire->ecriture(0x00,(uint8_t*) &tableEcrite, sizeof(tableEcrite));
    _delay_ms(5);
}

*/
int main(){
    DDRD = MODE_SORTIE;

    initialisationUART();

    Memoire24CXXX memoire = Memoire24CXXX();
    uint8_t tampon;
    uint16_t i = 0x0000;
    //ecrireMessageMemoire(&memoire);

    while(tampon!=0xFF){
        memoire.lecture(i++, &tampon);
        if(tampon!=0xFF) {
            transmissionUART(tampon);
        }
    }
   
        
    return 0;
}
 
