#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


volatile uint8_t minuterieExpiree;
volatile uint8_t boutonPoussoir;


ISR (TIMER1_COMPA_vect) {
    minuterieExpiree = 1;
}


ISR (INT0_vect) {
    // anti-rebond
    _delay_ms ( 30 );
    boutonPoussoir = 1;
    EIFR |= (1 << INTF0) ;
}

void partirMinuterie ( uint16_t duree ) {

    minuterieExpiree = 0;
    // mode CTC du timer 1 avec horloge divisée par 1024
    // interruption après la durée spécifiée
    TCNT1 = 0 ;     //compteur
    OCR1A = duree;  // set OCF1A lorsque TCNT1 ==
    TCCR1A = 0b00001000 ;
    TCCR1B = 0b00000101 ;
    TCCR1C = 0;
    TIMSK1 = 0b00000010; //man page 134 enable comparison OCF1A
}



void initialisation ( void ) {

    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'être interrompu alors que
    // le microcontroleur n'est pas prêt...
    cli ();
    // configurer et choisir les ports pour les entrées
    // et les sorties. DDRx... Initialisez bien vos variables
    DDRA = 0xff;
    DDRD = 0x00;
    // cette procédure ajuste le registre EIMSK
    // de l’ATmega324PA pour permettre les interruptions externes
    EIMSK |= (1 << INT0) ;
    // il faut sensibiliser les interruptions externes aux
    // changements de niveau du bouton-poussoir
    // en ajustant le registre EICRA
    EICRA |= (1 << ISC00);
    // sei permet de recevoir à nouveau des interruptions.
    sei ();
}


int main()
{
    initialisation();
    _delay_ms(10000);
    PORTA = (1<< PINA1);
    _delay_ms(100);
    PORTA = (0<< PINA);
    minuterieExpiree = 0;
    boutonPoussoir = 0;
    partirMinuterie(7812);  //8000000 / 1024 = 7812.5

    
    while ( minuterieExpiree == 0 && boutonPoussoir == 0 );
    // Une interruption s'est produite. Arrêter toute
    // forme d'interruption. Une seule réponse suffit.
    cli ();    
    if(minuterieExpiree){
        PORTA = (1<<PINA0);
    }
    else{
        PORTA = (1<<PINA1);
    }
    return 0;
}