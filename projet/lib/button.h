#include <common.h>
#include <avr.h>

class Button {
    public:
        enum class Interruptions{
            INTER0,
            INTER1
        };
        enum class Edges{
            BOTH = 1,
            FALLING = 2,
            RISING = 3
        };
        Button(Interruptions interruption, Edges edge);
        ~Button();
        void setOnInterruption(Interruptions interruption, Edges edge);
        void debouncer();
    private:
        Interruptions _interruption;
        Edges _edge;
};