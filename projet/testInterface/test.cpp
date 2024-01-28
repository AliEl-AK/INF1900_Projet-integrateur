#include <avr.h>
#include <stdlib.h>
#include <common.h>
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"

LCM disp(&LCD_DDR, &LCD_PORT);

int main(){
    disp.write("test");
    disp.write("hi",LCM_FW_HALF_CH);
    while(1){

    }
}