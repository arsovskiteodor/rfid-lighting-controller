/*
 * File:   TAD_SIO.c
 * Author: teoA1
 *
 * Created on February 19, 2025, 1:03 PM
 */


#include <xc.h>
#include "TAD_SIO.h"
#include "pic18f4321.h"

void SIO_Init() {

    TRISCbits.TRISC7 = 1; 
    TRISCbits.TRISC6 = 1; 
    
    TXSTA = 0x20;
    
    BAUDCON = 0;
    BAUDCONbits.BRG16 = 1;
    
    RCSTA = 0x90;
    
    SPBRG = 21;
    SPBRGH = 0;
    
}

unsigned char SIO_RXAvail() {
    if (PIR1bits.RCIF == 1) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

unsigned char SIO_GetChar() {
    return RCREG;
}

unsigned char SIO_TXAvail() {
    if (TXSTAbits.TRMT == 1) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

void SIO_PutChar(unsigned char Value) {
    TXREG = Value;
}
