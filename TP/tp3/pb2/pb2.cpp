#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <utility>
void roueTourne()
{
    PORTB = (1<< PINB1);
}
void roueArret()
{
    PORTB = (0 << PINB1);
}
void delayFs(int temps)
{
    for (int i=0;i<temps;i++)
    {
    _delay_us(1);
    }
}
int f(int PMW, int b )
{
    int a = PMW * b;
    int c = b-a;
}
int main()
{   DDRB = 0xff;
    while(true){
    for (int i = 0; i<2000 ; i++)
    {
        std::pair<int,int>f(0,1/60);
        roueTourne();
        delayFs(a);
         roueArret();
        delayFs(c);
    }
    for (int i =0 ; i<2000; i++)
    {
        std::pair<int,int>f(0.25,1/60);
        roueTourne();
        _delay_us(250);
         roueArret();
        _delay_us(750);
    }
    for (int i =0 ; i<2000; i++)
    {
        std::pair<int,int>f(0.50,1/60);
         roueTourne();
        _delay_us(500);
         roueArret();
        _delay_us(500);
    }
    for (int i =0 ; i<2000; i++)
    {
        std::pair<int,int>f(0.75,1/60);
        roueTourne();
        _delay_us(750);
         roueArret();
        _delay_us(250);
    }

    for (int i =0 ; i<2000; i++)
    {
        std::pair<int,int>f(1,1/60);
        roueTourne();
        _delay_us(1000);
         roueArret();
        _delay_us(0);
    }
    }
}