/*
Description du programme : Lorsqu'un utilisateur appuie sur le bouton ajouté, une minuterie est lancée pour une durée déterminée.
Pendant cette période, la DEL (diode électroluminescente) connectée à la carte alterne entre les couleurs verte et rouge à intervalles réguliers.
Identifications matérielles (Broches I/O) : Entrée : PIND / Sortie: PINB
*/
#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>

const int rouge = 0x01;
const int vert = 0x02;
const int off = 0x00;
const int enfoncer = 0x04;

volatile bool gMinuterieExpiree = false;
volatile bool gBoutonEnfonce = false;

void partirMinuterie(uint16_t duree)
{
    gMinuterieExpiree = false;

    TCNT1 = 0; // Initialisation du compteur

    OCR1A = duree; // Valeur a laquelle le timer est comparee

    TCCR1A = 0; // Activation du "compare match"       //PAS BESOIN

    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // Clock select /1024

    TCCR1C = 0;

    TIMSK1 |= (1 << OCIE1A); // Output compare A match interrupt enable
}

ISR(INT0_vect)
{
    _delay_ms(30); 
    if (static_cast<bool>(PIND & enfoncer))
    {
        gBoutonEnfonce = false;
    }
    else
    {
        gBoutonEnfonce = true;
    }
}

ISR(TIMER1_COMPA_vect)
{
    gMinuterieExpiree = true;
}

void initialisation(void)
{
    cli();       // Cli empeche les interruption
    DDRD = 0x00; // Port D en mode entree
    DDRB = 0xff; // Port B en mode sortie
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00); // Permet les inetrruptions externes aux changement de niveau du bouton-poussoir
    sei();       // Permet les interruptions
}

void arreterMinuterie()
{
    TCCR1B &= ~(1 << WGM12) | (1 << CS12) | (1 << CS10);
    TIMSK1 &= ~(1 << OCIE1A);
}

int main()
{

    const int valeurMinutrie = 781;
    const int demiSecond = 500;
    const int quartSecond = 250;


    initialisation();
    while (true)
    {
        int compteur = 0;
        while (!gBoutonEnfonce){}
        partirMinuterie(valeurMinutrie);

        while (compteur < 120 && gBoutonEnfonce) 
        {
            if (gMinuterieExpiree) 
            {
                compteur++;
                partirMinuterie(valeurMinutrie);
            }
        };

        int nbDeClignotement = (compteur / 2);
        PORTB = vert;
        _delay_ms(demiSecond);
        PORTB = off;
        _delay_ms(demiSecond);
        for (int i = 0; i < nbDeClignotement; i++)
        {
            PORTB = rouge;
            _delay_ms(quartSecond);
            PORTB = off;
            _delay_ms(quartSecond);
        }

        PORTB = vert;
        _delay_ms(1000);
        PORTB = off;
        arreterMinuterie();
    }
    return 0;
}
