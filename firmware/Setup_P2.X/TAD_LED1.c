/*
 * File:   TAD_LED1.c
 * Author: teoA1
 *
 * Created on February 25, 2025, 5:13 PM
 */


#include <xc.h>
#include "TAD_TIMER.h"
#include "TAD_LED1.h"
#include "TAD_SIO.h"

static unsigned char state1 = 0;
static unsigned char intensity1 = 0;
static unsigned char Timer1;

void initLED1() {
    TRISA = 0xC0;
    ADCON1 = 0x0F;
    LATAbits.LATA0 = 0;
    LATAbits.LATA1 = 0;
    LATAbits.LATA2 = 0;
    LATAbits.LATA3 = 0;
    LATAbits.LATA4 = 0;
    LATAbits.LATA5 = 0;
    TI_NewTimer(&Timer1);
}

void setIntensity1(unsigned char value) {
    if (value - '0' == 17) {
        intensity1 = 10;
    }
    else {
        intensity1 = value - '0';
    }
   
}

void LED1_Motor () {
   
    switch (state1) {
        
        case 0:
            if (TI_GetTics(Timer1) >= intensity1) {
                if (intensity1 != 10) {
                    LATAbits.LATA0 = 0;
                    state1 = 1;
                }
            }
            break;
        case 1:
            if (TI_GetTics(Timer1) >= 10) {
                    if (intensity1 != 0) {
                    TI_ResetTics(Timer1);
                    LATAbits.LATA0 = 1;
                    state1 = 0;
                }
            }
            break;
    }
    
}
