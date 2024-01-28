#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>


void ajustementPWM ( float a, float b ) 
{

    // mise à un des sorties OC1A et OC1B sur comparaison
    // réussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6
    // page 177 de la description technique du ATmega324PA)
    OCR1A = 0xFF * a;
    OCR1B = 0xFF * b;

    // division d'horloge par 8 - implique une frequence de PWM fixe
    //TCCR1A = 0b111100xx;
    //TCCR1B = 0b000xx010;

    TCCR1A = 0b10100001;
    TCCR1B = 0b00000010;    //horloge / 8

    TCCR1C = 0;
}


void initialisation ( void ) 
{

    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'être interrompu alors que
    // le microcontroleur n'est pas prêt...
    cli ();
    // configurer et choisir les ports pour les entrées
    // et les sorties. DDRx... Initialisez bien vos variables
    DDRA = 0xff;
    DDRD = 0xff;
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
   
    while(true)
    {
        ajustementPWM(0, 0);
        _delay_ms(2000);
        ajustementPWM(0.25, 0.25);
        _delay_ms(2000);
        ajustementPWM(0.5, 0.5);
        _delay_ms(2000);
        ajustementPWM(0.75, 0.75);
        _delay_ms(2000);
        ajustementPWM(1, 1);
        _delay_ms(2000);
    }

    return 0; 
}