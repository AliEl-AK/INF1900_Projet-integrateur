/*
+-----------------+--------+----------------+----+----+---------+
| Etat present    | Entree | Etat suivant   | A0 | A1 | Couleur |
+-----------------+--------+----------------+----+----+---------+
| INIT            | 0      | INIT           | 1  | 0  | Rouge   |
+-----------------+--------+----------------+----+----+---------+
| INIT            | 1      | FIRST_PRESS    | 1  | 1  | Ambre   |
+-----------------+--------+----------------+----+----+---------+
| FIRST_PRESS     | 0      | FIRST_RELEASE  | 0  | 1  | Vert    |
+-----------------+--------+----------------+----+----+---------+
| FIRST_PRESS     | 1      | FIRST_PRESS    | 1  | 1  | Ambre   |
+-----------------+--------+----------------+----+----+---------+
| FIRST_RELEASE   | 0      | FIRST_RElEASE  | 0  | 1  | Vert    |
+-----------------+--------+----------------+----+----+---------+
| FIRST_RELEASE   | 1      | SECOND_PRESS   | 1  | 0  | Rouge   |
+-----------------+--------+----------------+----+----+---------+
| SECOND_PRESS    | 0      | SECOND_RELEASE | 0  | 0  | OFF     |
+-----------------+--------+----------------+----+----+---------+
| SECOND_PRESS    | 1      | SECOND_PRESS   | 1  | 0  | Rouge   |
+-----------------+--------+----------------+----+----+---------+
| SECOND_REALEASE | 0      | SECOND_RELEASE | 0  | 0  | OFF     |
+-----------------+--------+----------------+----+----+---------+
| SECOND_RELEASE  | 1      | THIRD_PRESS    | 0  | 1  | Vert    |
+-----------------+--------+----------------+----+----+---------+
| THIRD_PRESS     | 0      | INIT           | 1  | 0  | Rouge   |
+-----------------+--------+----------------+----+----+---------+
| THIRD_PRESS     | 1      | THIRD_PRESS    | 0  | 1  | Vert    |
+-----------------+--------+----------------+----+----+---------+
*/


#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

enum class State
{
    INIT,
    FIRST_PRESS,
    FIRST_RELEASE,
    SECOND_PRESS,
    SECOND_RELEASE,
    THIRD_PRESS
};

const uint8_t MASK_INTERRUPT = (1 << PD2);
const uint8_t DEBOUNCE = 10;

bool isButtonPushed()
{
    uint8_t lecture1 = PIND & MASK_INTERRUPT;
    _delay_ms(DEBOUNCE);
    uint8_t lecture2 = PIND & MASK_INTERRUPT;
    return (lecture1 && lecture2);
}

void set_rouge()
{
    PORTA = (1 << PINA0);
    PORTA &= ~(1 << PINA1); // Turn off A1
}

void set_vert()
{
    PORTA = (1 << PINA1);
    PORTA &= ~(1 << PINA0); // Turn off A0
}

void set_off()
{
    PORTA &= ~((1 << PINA0) | (1 << PINA1)); // Turn off both A0 and A1
}

int main()
{
    DDRA |= (1 << DDA0) | (1 << DDA1); // Set A0 and A1 as outputs
    DDRD &= ~(1 << DDD2); // Set PD2 as input
    _delay_ms(10);

    State currentState = State::INIT;
    while (true)
    {
        switch (currentState)
        {
        case State::INIT:
            set_rouge();
            if (isButtonPushed())
            {
                currentState = State::FIRST_PRESS;
            }
            break;

        case State::FIRST_PRESS:
            while (isButtonPushed())
            {
                set_rouge();
                _delay_ms(DEBOUNCE);
                set_vert();
                _delay_ms(DEBOUNCE);
            }
            currentState = State::FIRST_RELEASE;
            break;

        case State::FIRST_RELEASE:
            set_vert();
            if (isButtonPushed())
            {
                currentState = State::SECOND_PRESS;
            }
            break;

        case State::SECOND_PRESS:
            while (isButtonPushed())
            {
                set_rouge();
            }
            currentState = State::SECOND_RELEASE;
            break;

        case State::SECOND_RELEASE:
            set_off();
            if (isButtonPushed())
            {
                currentState = State::THIRD_PRESS;
            }
            break;

        case State::THIRD_PRESS:
            while (isButtonPushed())
            {
                set_vert();
            }
            currentState = State::INIT;
            break;
        }
    }
}
