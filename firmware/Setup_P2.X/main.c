/*
 * File:   main.c
 * Author: teoA1
 *
 * Created on February 20, 2025, 4:15 PM
 */

#include "TAD_SIO.h"
#include "TRFID.h"
#include "TAD_Menu.h"
#include "TAD_TIMER.h"
#include "TAD_KEYPAD.h"
#include "LCTLCD.h"
#include "TAD_Time.h"

#include "TAD_LED1.h"
#include "TAD_LED2.h"
#include "TAD_LED3.h"
#include "TAD_LED4.h"
#include "TAD_LED5.h"
#include "TAD_LED6.h"

#include <xc.h>

#pragma config OSC = HSPLL
#pragma config WDT = OFF
#pragma config PBADEN = DIG
#pragma config MCLRE = ON
#pragma config LVP = OFF

static void __interrupt (high_priority) MyRSI (void);
void main(void);

static void __interrupt (high_priority) MyRSI (void) {
    if (TMR0IF==1) {      
        RSI_Timer0();
    }
}


void main(void) {
    
    TI_Init();
    SIO_Init();
    initRFID();
    initTime();
    
    initLED1();
    initLED2();
    initLED3();
    initLED4();
    initLED5();
    initLED6();
    
    initKeypad();
    
    LcInit(2, 16);
    
    while (1) {
        //LATAbits.LATA0 = 1;
        RFID_Motor();
        
        keypad_Motor();
        LCD_motor();
        Menu_Motor();
      
        LED1_Motor();
        LED2_Motor();
        LED3_Motor();
        LED4_Motor();
        LED5_Motor();
        LED6_Motor(); 
        
        Time_Motor();
        //LATAbits.LATA0 = 0;
    }
    return;
}
