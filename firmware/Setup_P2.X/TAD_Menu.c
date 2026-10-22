
#include <xc.h>
#include "TAD_SIO.h"
#include "TAD_Menu.h"
#include "TRFID.h"
#include "LCTLCD.h"
#include "TAD_Time.h"

char menuText[] = "\r\n---------------\r\nMain Menu\r\n---------------\r\nChoose an option:\n\r\n\t1. Who is in the room?\r\n\t2. Show settings\r\n\t3. Change system time\n\r\nOption: ";
static unsigned char state = 0;
static unsigned char i = 0;
static unsigned char j = 0;
static unsigned char val;
static unsigned char temp;
static unsigned char initial = 1;




void printMenuHash() {
    state = 1;;
    i = 0;
}

void Menu_Motor() {
    
    switch (state) {
        case 0:
            i = 0;
            j = 0;
            if (SIO_RXAvail() || initial) { 
                val = SIO_GetChar();
                if (val == 27 || initial) {
                    state = 1;
                    initial = 0;
                }
                if (val == 49) {
                    
                    state = 5;
                    SIO_PutChar('\r');
                }
                if (val == 50) {
                    state = 6;
                    j = 0;
                    i = 0;
                    SIO_PutChar('\r');
                }
                if (val == 51) {
                    resetHrs();
                    resetMins();
                    state = 12;
                   
                }
            }
            break;
        case 1:
            if(menuText[i] != '\0') {
                if (SIO_TXAvail()) {
                    SIO_PutChar(menuText[i]);
                    i++;
                }
            }
            else {
                state = 0;
                i = 0;
            }
            break;
        case 2:
            
            if (i < 5) {
                if (SIO_TXAvail()) {
                    SIO_PutChar(getUID(i, 1));
                    state = 3;
                }
            }
            else {
                i = 0;
                state = 0;
            }
            break;
        case 3:
            if (SIO_TXAvail()) {
                SIO_PutChar(getUID(i, 0));
                state = 4;
                i++;
            }
            break;
        case 4:
            if (i < 5) {
                if (SIO_TXAvail()) {
                SIO_PutChar('-');
                state = 2;
                }
            }
            else{
                state = 2;
            }
           
            break;   
        case 5:
            if (SIO_TXAvail()) {
                SIO_PutChar('\n');
                state = 2;
            }
            break;
        case 6:
            
            if (SIO_TXAvail()) {
                SIO_PutChar('\n');
                state = 7;
            }
            break;
        case 7:
            if (j < 8) {
                state = 8;
                i = 0;
            }
            else {
                state = 0;
            }
            break;
        case 8:
            if (i < 6) {
                if (SIO_TXAvail()) {
                    SIO_PutChar(getConfigMenu(j, i));
                    i++;
                    state = 9;
                }
            }
            else {
                state = 10;
            }
            break;
        case 9:
            if (i == 6) {
                state = 8;
                break;
            }
            if (SIO_TXAvail()) {
                SIO_PutChar('-');
                state = 8;
            }
            break;
        case 10:
            if (SIO_TXAvail()) {
                SIO_PutChar('\r');
                j++;
                state = 6;
            }
            break;
        case 12:
            if (i < 5) {
                if (SIO_RXAvail()) {
                    state = 13;
                    i++;
                } 
            }
            else {
                state = 0;
            }
            break;
        case 13:
            if (i == 3) {
                i++;
            }
            LcGotoXY(i + 1, 0);
            state = 14;
            break;
        case 14:
            temp = SIO_GetChar();
            LcPutChar(temp);
            temp = temp - '0';
            state = 15;
            break;
        case 15:
            
            switch (i) {
                case 1:
                    setHrs(temp * 10);
                    break;

                case 2:
                    setHrs(temp);
                    break;

                case 4:
                    setMins(temp * 10);
                    break;

                default:
                    setMins(temp);
                break;
            }
            state = 12;
            break;   
    }
}