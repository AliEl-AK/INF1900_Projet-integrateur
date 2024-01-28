#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

void delay_fs(int temps){
    for (int i=0;i<temps/40;i++){
    _delay_us(40);
    }
}

void del_vert(){
    PORTA = (1 << PA1);
}

void del_rouge(){
    PORTA = (1 << PA0);
}

void del_eteint(){
    PORTA = (0 <<PORTA);
}

void alumer_vert(int duree) {
    int a,c;
    for (int i = duree ;i>0;i--){
        a =i/3;
        del_vert();
        delay_fs(a);
        del_eteint();
        c =1000 -a;
        delay_fs(c);

    }
}

void alumer_rouge(int duree){
    int a,c;
    for (int i = duree ;i>0;i--){
        a =i/3;
        del_rouge();
        delay_fs(a);
        del_eteint();
        c =1000 -a;
        delay_fs(c);

    }
}
int main()
{
    DDRA |= (1 << DDA0) | (1 << DDA1);
    DDRA &= ~(1 << DDA2);

    while (true)
    {
        alumer_vert(3000);
        alumer_rouge(3000);
        }
    return 0;
}
