/*

+---------------+--------+---------------+---------+

| Etat present  | Entree | Etat suivant  | Sortie  |

+---------------+--------+---------------+---------+

| Init          | 0      | Init          | 0       |

+---------------+--------+---------------+---------+

| Init          | 1      | First_press   | 0       |

+---------------+--------+---------------+---------+

| First_press   | 0      | First_press   | 0       |

+---------------+--------+---------------+---------+

| First_press   | 1      | Second_press  | 0       |

+---------------+--------+---------------+---------+

| Second_press  | 0      | Second_press  | 0       |

+---------------+--------+---------------+---------+

| Second_press  | 1      | Third_press   | 0       |

+---------------+--------+---------------+---------+

| Third_press   | 0      | Init          | 1       |

+---------------+--------+---------------+---------+

| Third_press   | 1      | Init          | 1       |

+---------------+--------+---------------+---------+

*/
#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

enum class State
{
    OFF,
    WAITING,
    GREEN
};

const uint8_t MASK_INTERRUPT = (1 << PD2);
const uint8_t DEBOUNCE = 10;
const uint16_t TIME_WAITING = 2000;

bool isButtonPushed()
{
    uint8_t lecture1 = PIND & MASK_INTERRUPT;
    _delay_ms(DEBOUNCE);
    uint8_t lecture2 = PIND & MASK_INTERRUPT;
    return (lecture1 && lecture2);
}

int main()
{
    DDRA |= (1 << DDA0) | (1 << DDA1);
    DDRD &= ~(1 << DDD2);
    PORTA = 0;
    State currentState = State::OFF;
    uint8_t count = 0;

    while (true)
    {
        switch (currentState)
        {
        case State::OFF:
            PORTA &= ~(1 << PA1);

            if (isButtonPushed())
            {
                count++;

                if (count == 3)
                {
                    currentState = State::GREEN;
                }
                else
                {
                    currentState = State::WAITING;
                }
            }
            break;

        case State::WAITING:
            if (!isButtonPushed())
            {
                currentState = State::OFF;
            }
            break;

        case State::GREEN:
            if (!isButtonPushed())
            {
                PORTA = (1 << PA1);
                _delay_ms(2000);
                currentState = State::OFF;
                count = 0;
            }
            break;
        }
    }
    return 0;
}

