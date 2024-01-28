#include <distanceSensor.h>
#include <stdlib.h>
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"

int main(){
    LCM disp(&LCD_DDR, &LCD_PORT);
    can analog;
    DistanceSensor distanceSensor(analog, PA2);
    while(1){
        if (distanceSensor.isCloseToPillar()){
            disp.write("FOUND");
        } else {
            disp.clear();
        }
    }
}