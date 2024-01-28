#include <avr.h>
#include <stdlib.h>
#include <common.h>
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"


void static inline w(void) {
	cp_wait_ms(2000);
}

int main(void) {	
	// Cr�ation de l'objet (l'afficheur est connect� sur le port DEMO_PORT)
	LCM disp(&LCD_DDR, &LCD_PORT);
	
	// �criture d'un simple caract�re au d�but de la premi�re ligne
	disp.write("(4, 5)", 0, true);
    disp.write ("OUEST",LCM_FW_HALF_CH, false);
	w();
    disp.clear();
	// Ajout d'autres caract�res

	
    // Ajout d'autres caract�res
	disp.write("\x7e yeah! \x7f", LCM_FW_HALF_CH, true);
	w();

    disp.write("\x04",0,true);
    w();

	// Nettoyage de l'affichage
	disp.clear();
	w();

    w();
    uint8_t cc2 [] = {
		0x02, //    #
		0x04, //   #
		0x0e, //  ##
		0x11, // #   #
		0x1f, // #####
		0x10, // #
		0x0e, //  ###
		0x00 //
	};
	disp.build_cc(4, cc2);
	char end [] = "Fin de la d\x04mo !";
	disp = end;
	disp << end;
	
	return 0;

}