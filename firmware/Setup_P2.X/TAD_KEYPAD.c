#include <xc.h>
#include "TAD_TIMER.h"
#include "TAD_Menu.h"   // for getMenuTxt(...)
//#include "TAD_SIO.h"
#include "TAD_LED1.h"
#include "TAD_LED2.h"
#include "TAD_LED3.h"
#include "TAD_LED4.h"
#include "TAD_LED5.h"
#include "TAD_LED6.h"
#include "TRFID.h"
#include "LCTLCD.h"

#define FALSE 0
#define TRUE  1

static unsigned char state = 14;     
static unsigned char row = 0;       
static unsigned char debTimer;      
static unsigned char longTimer;     
static unsigned char meltdownTimer; 
static unsigned char digits[2];
static unsigned char count = 0;     
static unsigned char meltdownIndex = 0;   

static unsigned char decodeKey(unsigned char row, unsigned char col)
{
    static unsigned char map[4][3] = {
       {'1',  '2',  '3'},
       {'4',  '5',  '6'},
       {'7',  '8',  '9'},
       {'A', '0', 11}
    };
    return map[row][col];
}  

static  unsigned char meltdownX[6] = {10,14, 2, 6,10,14};
static  unsigned char meltdownY[6] = { 0,  0, 1, 1, 1, 1};

static void driveRow(unsigned char r)
{
    LATDbits.LATD7 = (r == 0);
    LATDbits.LATD4 = (r == 1);
    LATDbits.LATD5 = (r == 2);
    LATDbits.LATD6 = (r == 3);
}

static unsigned char whichColPressed(void)
{
    if (PORTEbits.RE1) return 0;
    if (PORTEbits.RE0) return 1;
    if (PORTCbits.RC5) return 2;
    return 3; 
}

static unsigned char anyKeyPressed(void)
{
    return (whichColPressed() != 3) ? TRUE : FALSE;
}

void enableSystem(unsigned char en) {
    if (en) {
        state = 0;
    }
    else {
        state = 14;
    }
    
}

void initKeypad()
{
    ADCON1 = 0x0F;

    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD7 = 0;
    LATDbits.LATD4 = 0;
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 0;
    LATDbits.LATD7 = 0;

    TRISEbits.TRISE0 = 1;
    TRISEbits.TRISE1 = 1;
    TRISCbits.TRISC5 = 1;

    TI_NewTimer(&debTimer);
    TI_NewTimer(&longTimer);
    TI_NewTimer(&meltdownTimer);
}

void keypad_Motor()
{
    switch (state)
    {

    case 0:
        driveRow(row);
        if (!anyKeyPressed()) {
            row++;
            if (row >= 4) row = 0;
        } else {
            TI_ResetTics(debTimer);
            state = 1;
        }
        break;

    case 1: 
        if (TI_GetTics(debTimer) >= 8) {
            unsigned char c = whichColPressed();
            
            unsigned char d = decodeKey(row, c);
            if (d == 11) { 
                TI_ResetTics(longTimer);
                meltdownIndex = 0;
                state = 3; 
            }
            else {
                
                digits[count] = d;
                count++;
                state = 11; 
            }
            
        }
        break;

    case 2: 
        if (!anyKeyPressed()) {
            TI_ResetTics(meltdownTimer);
            state = 13; 
        }
        break;

    case 3: 
        
        if (!anyKeyPressed()) { 
            state = 10; 
        } 
        else {
           
            state = 12;
        }
        break;

    case 4: 
    
        unsigned char d0 = digits[0];
        unsigned char d1 = digits[1];
        
        switch (d0 - '0')
        {
            case 0:
                setIntensity1(d1);
                setIntensityRFID(d1, 0, 5);
                break;
            case 1:
                setIntensity2(d1);
                setIntensityRFID(d1, 1, 13);
                break;
            case 2:
                setIntensity3(d1);
                setIntensityRFID(d1, 2, 21);
                break;
            case 3:
                setIntensity4(d1);
                setIntensityRFID(d1, 3, 29);
                break;
            case 4:
                setIntensity5(d1);
                setIntensityRFID(d1, 4, 37);
                break;
            case 5:
                setIntensity6(d1);
                setIntensityRFID(d1, 5, 45);
                break;
        }
        enable();
        count = 0;
        state = 2; 
        break;
    

    case 7: 
    
        deleteConfig();
        setIntensity1('0');
        setIntensity2('0');
        setIntensity3('0');
        setIntensity4('0');
        setIntensity5('0');
        setIntensity6('0');
        enable();
        meltdownIndex = 0;
        TI_ResetTics(meltdownTimer);
        state = 9; 
        break;
    

    case 9:
        if (!anyKeyPressed()) {
            TI_ResetTics(meltdownTimer);
            state = 13; 
        }
        break;

 
    case 10: 
    
        printMenuHash();
        TI_ResetTics(meltdownTimer);
        state = 13; 
        
        break;
    

    case 11:
        if (count >= 2) {
            state = 4; 
        } else {
            state = 2; 
        }
        break;

    case 12:
        if (!anyKeyPressed()) {
            meltdownIndex = 0;
            state = 10; 
        } 
        else {
            
            if (TI_GetTics(longTimer) >= 1500) {
                meltdownIndex = 0;     
                state = 7;            
            }
        }
        break;

    case 13:
            if (TI_GetTics(meltdownTimer) >= 8) {
                state = 0;
                TI_ResetTics(meltdownTimer);
            }
            break;
    
    }
}
