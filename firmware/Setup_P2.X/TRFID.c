/*
 * File:   TRFID.c
 * Author: jnavarro & ester.vidana
 *
 * 
 * Inspired by: https://simplesoftmx.blogspot.com/2014/11/libreria-para-usar-lector-rfid-rc522.html
 */


 #include <xc.h>
 #include "TRFID.h"
 #include "TAD_SIO.h"
 #include "TAD_LED1.h"
 #include "TAD_LED2.h"
 #include "TAD_LED3.h"
 #include "TAD_LED4.h"
 #include "TAD_LED5.h"
 #include "TAD_LED6.h"
 #include "LCTLCD.h"
 #include "TAD_KEYPAD.h"
 
 #define UID_LENGTH 5
 #define NUM_ACCEPTED_UIDS 8
 #define LED_COUNT 6
 
 static unsigned char state = 0;
 static unsigned char stateNext = 0;

 static char UID[5]; 
 static unsigned char msgIndex = 0;
 static unsigned char uidIndex = 0;
 static unsigned char i = 0;
 static unsigned char j = 0;
 static unsigned char k = 0;

  int ucAddr1;
  char ucAddr2;
  unsigned char myAddress;
  unsigned char myValue;

  static unsigned char myTemp;
 
  char myirqEn = 0x00;
  char mywaitIRq = 0x00;               
  char mylastBits;
  char n1;

 static unsigned char times = 0;

 static unsigned char mysendLen = 0;

 static char ims = 0;

static unsigned mybackLen;

 unsigned char myucResult;

char myserNumCheck = 0;

char myStatus;

unsigned in;

 static unsigned char currentIndx = NUM_ACCEPTED_UIDS;
 
 char  msg[] = "\r\nCard detected!\r\n\tUID: ";  
 char lights[] = "\tL0: 0 - L1: 0 - L2: 0 - L3: 0 - L4: 0 - L5: 0"; 
 char const hexDigits[] = "0123456789ABCDEF";  
 
 static unsigned char acceptedUIDs[NUM_ACCEPTED_UIDS][UID_LENGTH] = {
     {0xBF, 0xC7, 0x4A, 0x1F, 0x2D},//
     {0xEF, 0x7E, 0x68, 0x1E, 0xE7},//
     {0xFE, 0xD6, 0x78, 0x00, 0x50},//
     {0xE5, 0x85, 0x38, 0xA4, 0xFC},//
     {0x88, 0x05, 0x6F, 0xD0, 0x32},//
     {0x88, 0x05, 0x6E, 0xA6, 0x45},//
     {0x88, 0x05, 0x68, 0xA6, 0x43},//
     {0x33, 0xE0, 0x78, 0x00, 0xAB}//
 };
 
 static unsigned char userLEDs[NUM_ACCEPTED_UIDS][LED_COUNT] = {
     {'0', '0', '0', '0', '0', '0'},  
     {'0', '0', '0', '0', '0', '0'},  
     {'0', '0', '0', '0', '0', '0'},   
     {'0', '0', '0', '0', '0', '0'},   
     {'0', '0', '0', '0', '0', '0'},   
     {'0', '0', '0', '0', '0', '0'},   
     {'0', '0', '0', '0', '0', '0'},   
     {'0', '0', '0', '0', '0', '0'}   
 };
 
 
 
 //-------------- Private functions: --------------
 void InitPortDirections(){
 DIR_MFRC522_SO  = 1; 
 DIR_MFRC522_SI  = 0; 
 DIR_MFRC522_SCK = 0; 
 DIR_MFRC522_CS  = 0; 
 DIR_MFRC522_RST = 0;
 }
 
 void delay_us(char howMany){
 #define NUM_US 16  //Number of instructions in 1 uS  
     char x;
     for (x=0; x<howMany*NUM_US;x ++) Nop();
 }
 
 unsigned char MFRC522_Rd(unsigned char Address) 
 {
    unsigned int i, ucAddr;
    unsigned int ucResult = 0;
    MFRC522_SCK = 0;
    MFRC522_CS = 0;
    ucAddr = ( (Address<<1)&0x7E)|0x80;
 
    for (i = 8; i > 0; i--)
    {
       MFRC522_SI= ((ucAddr&0x80) == 0x80);
       MFRC522_SCK = 1;
       //delay_us(5);
 
       ucAddr <<= 1;
       MFRC522_SCK = 0;
       //delay_us(5);
 
    }
 
    for (i = 8; i > 0; i--)
    {
       MFRC522_SCK = 1;
       //delay_us(5);
       ucResult <<= 1;   
       ucResult|= MFRC522_SO;
       MFRC522_SCK = 0;
       //delay_us(5);
    }
 
   
    MFRC522_CS = 1;
    MFRC522_SCK = 1;
    return ucResult;
 }
 
 
 void MFRC522_Wr(unsigned char Address, unsigned char value)
 {
   
    unsigned char i, ucAddr;
    MFRC522_SCK = 0;
    MFRC522_CS = 0;
    ucAddr = ( (Address<<1)&0x7E);
    for (i = 8; i > 0; i--)
    {
       MFRC522_SI= ( (ucAddr&0x80) == 0x80);
       MFRC522_SCK = 1;
       ucAddr <<= 1;
       delay_us(5);
       MFRC522_SCK = 0;
       delay_us(5);
    }
 
    for (i = 8; i > 0; i--)
    {
       MFRC522_SI = ( (value&0x80) == 0x80);
       MFRC522_SCK = 1;
       value <<= 1;
       delay_us(5);
       MFRC522_SCK = 0;
       delay_us(5);
 
    }
 
    MFRC522_CS = 1;
    MFRC522_SCK = 1;
 }
 
 
 
 void MFRC522_Clear_Bit( char addr, char mask )
 {     unsigned char  tmp =0x0;
       tmp=MFRC522_Rd( addr ) ;
      MFRC522_Wr( addr,  tmp&~mask );   
 }
 void MFRC522_Set_Bit( char addr, char mask )
 {    unsigned char  tmp =0x0;
       tmp=MFRC522_Rd( addr ) ; 
      MFRC522_Wr( addr, tmp|mask );
 }
 void MFRC522_Reset()
 { 
    MFRC522_RST = 1;
    delay_us (1);
    MFRC522_RST = 0;
    delay_us (1);
    MFRC522_RST = 1;
    delay_us (1);
    //MFRC522_Wr( COMMANDREG, PCD_RESETPHASE );
    delay_us (1);
 }
 
 void MFRC522_AntennaOn()
 {                                              
     //unsigned char stt;
     //stt= MFRC522_Rd( TXCONTROLREG ) ;
     MFRC522_Set_Bit( TXCONTROLREG, 0x03 );
 }
 void MFRC522_AntennaOff()
 {
 MFRC522_Clear_Bit( TXCONTROLREG, 0x03 );                                          
 }
 void MFRC522_Init()     
 {                 
     MFRC522_CS = 1; 
     MFRC522_RST = 1;
    
      MFRC522_Reset();       
      MFRC522_Wr( TMODEREG, 0x8D );      //Tauto=1; f(Timer) = 6.78MHz/TPreScaler
      //MFRC522_Wr( TPRESCALERREG, 0x3E ); //TModeReg[3..0] + TPrescalerReg
      MFRC522_Wr( TRELOADREGL, 30 );
      //MFRC522_Wr( TRELOADREGH, 0 ); 
      MFRC522_Wr( TXAUTOREG, 0x40 );    //100%ASK
      //MFRC522_Wr( MODEREG, 0x3D );      // CRC valor inicial de 0x6363
        
   
      //MFRC522_AntennaOff() ;            
      MFRC522_AntennaOn();
 }
 
 //-------------- Public functions: --------------
 void initRFID(){
     InitPortDirections();
     MFRC522_Init(); 
 }
 
 void setIntensityRFID(unsigned char v, unsigned char which, unsigned char l) {
     userLEDs[currentIndx][which] = v;
     lights[l] = v;
 }
 
 unsigned char getUID(unsigned char pos, unsigned char which) {
     if (currentIndx != NUM_ACCEPTED_UIDS) {
        if (which == 1) {
            return hexDigits[acceptedUIDs[currentIndx][pos] >> 4];
        }
        else {
            return hexDigits[acceptedUIDs[currentIndx][pos] & 0x0F];
        }
     }
     else {
         return 'X';
     }
 }
 
 void deleteConfig() {
     userLEDs[currentIndx][0] = '0';
     userLEDs[currentIndx][1] = '0';
     userLEDs[currentIndx][2] = '0';
     userLEDs[currentIndx][3] = '0';
     userLEDs[currentIndx][4] = '0';
     userLEDs[currentIndx][5] = '0';
    lights[5] = '0' ;
    lights[13] = '0';
    lights[21] = '0' ;
    lights[29] = '0' ;
    lights[37] = '0' ;
    lights[45] = '0';     
     
 }
 
 unsigned char getConfig(unsigned char which) {
     if (currentIndx == NUM_ACCEPTED_UIDS) {
         return '0';
     }
     else {
         return userLEDs[currentIndx][which];
     } 
 }
 unsigned char getConfigMenu(unsigned char which, unsigned char l) {
     
     return userLEDs[which][l];
 }
 
void RFID_Motor(){   
    
    switch (state) {
        
        case 0:
            myserNumCheck = 0;
            times = 0;
            mysendLen = 1;
            myAddress = BITFRAMINGREG;
            myValue = 0x07;
            state = 1;
            stateNext = 7;
            msgIndex = 0;
            uidIndex = 0;
            break;
        case 1:
            MFRC522_SCK = 0;
            MFRC522_CS = 0;
            ucAddr2 = ((myAddress<<1)&0x7E);
            state = 2;
            j = 0;
            break;
        case 2:
            if (j < 8) {
                MFRC522_SI= ( (ucAddr2&0x80) == 0x80);
                MFRC522_SCK = 1;
                ucAddr2 <<= 1;
                delay_us(5);
                MFRC522_SCK = 0;
                delay_us(5);
                j++;
            }
            else {
                j = 0;
                state = 3;
            }
            break;
        case 3:
            if (j < 8) {
                MFRC522_SI = ( (myValue&0x80) == 0x80);
                MFRC522_SCK = 1;
                myValue <<= 1;
                delay_us(5);
                MFRC522_SCK = 0;
                delay_us(5);
                j++;
            }
            else {
                MFRC522_CS = 1;
                MFRC522_SCK = 1;
                state = stateNext;
            }
            break;
        case 4:
            MFRC522_SCK = 0;
            MFRC522_CS = 0;
            ucAddr1 = ( (myAddress<<1)&0x7E)|0x80;
            state = 5;
            j = 0;
            break;
        case 5:
            if (j < 8) {
                MFRC522_SI= ((ucAddr1&0x80) == 0x80);
                MFRC522_SCK = 1;
                delay_us(5);
                ucAddr1 <<= 1;
                MFRC522_SCK = 0;
                delay_us(5);
                j++;
            }
            else {
                myucResult = 0;
                state = 6;
                j = 0;
            }
            break;
        case 6:
            if (j < 8) {
                MFRC522_SCK = 1;
                delay_us(5);
                myucResult <<= 1;   
                myucResult|= MFRC522_SO;
                MFRC522_SCK = 0;
                delay_us(5);
                j++;
            }
            else {
                myTemp = myucResult;
                MFRC522_CS = 1;
                MFRC522_SCK = 1;
                state = stateNext;
            }
            break;
        case 7:
            UID[0] = PICC_REQIDL;
            myirqEn = 0x77;
            mywaitIRq = 0x30;
            myAddress = COMMIENREG;
            myValue = myirqEn | 0x80;
            state = 1;
            stateNext = 8;
            break;
        case 8:
            myAddress = COMMIRQREG;
            state = 4;
            stateNext = 9;
            break;
        case 9:
            myValue = myTemp&~0x80;
            state = 1;
            stateNext = 10;
            break;
        case 10:
            myAddress = FIFOLEVELREG;
            state = 4;
            stateNext = 11;
            break;
        case 11:
            myValue = myTemp|0x80;
            state = 1;
            stateNext = 12;
            break;
        case 12:
            myAddress = COMMANDREG;
            myValue = PCD_IDLE;
            state = 1;
            stateNext = 13;
            k = 0;
            break;
        case 13:
            myAddress = FIFODATAREG;
            myValue = UID[k];
            k++;
            state = 1;
            stateNext = 14;
            break;
        case 14:
            if (k < mysendLen) {
                myValue = UID[k];
                state = 1;
                k++;
            }
            else {
                myAddress = COMMANDREG;
                myValue = PCD_TRANSCEIVE;
                state = 1;
                stateNext = 15;
            }
            break;
        case 15:
            myAddress = BITFRAMINGREG;
            state = 4;
            stateNext = 16;
            break;
        case 16:
            myValue = myTemp|0x80;
            state = 1;
            stateNext = 17;
            break;
        case 17:
            in = 0xFFFF;
            myAddress = COMMIRQREG;
            state = 4;
            stateNext = 18;
            in--;
            break;
        case 18:
            n1 = myTemp;
            if (in && !(n1 & 0x01) && !(n1 & mywaitIRq)) {
                state = 4;
                in--;
            }
            else {
                myAddress = BITFRAMINGREG;
                state = 4;
                stateNext = 19;
            }
            break;
        case 19:
            myValue = myTemp&~0x80;
            state = 1;
            stateNext = 20;
            break;
        case 20:
            if (in != 0) {
                myAddress = ERRORREG;
                state = 4;
                stateNext = 21;
            }
            else {
                myStatus = MI_ERR;
                state = 26;
            }
            break;
        case 21:
            if (!(myTemp & 0x1B)) {
                myStatus = MI_OK;
                if (n1 & myirqEn & 0x01) myStatus = MI_NOTAGERR;
                myAddress = FIFOLEVELREG;
                state = 4;
                stateNext = 22;
            }
            else {
                myStatus = MI_ERR;
                state = 26;
            }
            break;
        case 22:
            n1 = myTemp;
            myAddress = CONTROLREG;
            state = 4;
            stateNext = 23;
            break;
        case 23:
            mylastBits = myTemp & 0x07;
            if (mylastBits) {
                mybackLen = (n1-1) * 8 + mylastBits;
            }
            else {
                mybackLen = n1 * 8;
            }
            if (n1 == 0) {
                n1 = 1;
            }
            else if (n1 > 16) {
                n1 = 16;
            }
            myAddress = FIFODATAREG;
            state = 4;
            stateNext = 24;
            ims = 0;
            break;
        case 24:
            UID[ims] = myTemp;
            ims++;
            if (ims < n1) {
                state = 4;
            }
            else {
                UID[ims] = 0;
                state = 25;
            }
            break;
        case 25:
            if (times == 1) {
                state = 31;
            }
            else {
                if ((myStatus != MI_OK) || (mybackLen != 0x10)) {
                myStatus = MI_ERR;
                 
                }
                state = 26;
            }
            
            break;
        case 26:
            
            if (myStatus == MI_OK) {
                
                times++;
                if (times == 2) {
                    state = 31;
                }
                else if (times == 1) {
                    state = 27;
                }   
            }
            else {
                state = 0;
            }
            break;
        case 27:
            UID[0] = PICC_ANTICOLL;
            UID[1] = 0x20;
            myAddress = BITFRAMINGREG;
            myValue = 0x00;
            state = 1;
            stateNext = 28;
            break;
        case 28:
            myAddress = STATUS2REG;
            state = 4;
            stateNext = 29;
            break;
        case 29:
            myValue = myTemp&~0x08;
            state = 1;
            stateNext = 30;
            break;
        case 30:
            mysendLen  = 2;
            myirqEn = 0x77;
            mywaitIRq = 0x30;
            state = 1;
            stateNext = 8;
            myAddress = COMMIENREG;
            myValue = myirqEn|0x80;
            break;
        case 31:
            if (myStatus == MI_OK) {
                state = 32;
                k = 0;
            }
            else {
                state = 0;
            }
            break;
        case 32:
            if (k < 4) {
                myserNumCheck ^= UID[(int)k];
                k++;
            }
            else {
                if (myserNumCheck != UID[4]) {
                    myStatus = MI_ERR;  
                }
                state = 33;
            }
            break;
        case 33:
            UID[5] = 0;
            if (myStatus == MI_OK) {
                state = 34;
                msgIndex = 0;
                j = 0;
            }
            else {
                state = 0;
            }
            break;
        case 34:
            if (j < NUM_ACCEPTED_UIDS) {
                state = 35;
                i = 0;
            }
            else {
                state = 46; // 
            }
            break;
        case 35:
            if (i < UID_LENGTH) {
                if (UID[i] == acceptedUIDs[j][i]) {
                    i++;
                }
                else {
                    j++;
                    state = 34;
                }
            }
            else {
                state = 36;
            }
            break;
        case 36:
            if (j == currentIndx) {
                currentIndx = NUM_ACCEPTED_UIDS; // not here
                setIntensity1('0');
                setIntensity2('0');
                setIntensity3('0');
                setIntensity4('0');
                setIntensity5('0');
                setIntensity6('0');
                state = 53; 
                enableSystem(0);
            }
            else {
                currentIndx = j;
                state = 37;   
            }
            
            break;
        case 37:
            msgIndex = 0;
            uidIndex = 0;
            
            setIntensity1(userLEDs[currentIndx][0]);
            setIntensity2(userLEDs[currentIndx][1]);
            setIntensity3(userLEDs[currentIndx][2]);
            setIntensity4(userLEDs[currentIndx][3]);
            setIntensity5(userLEDs[currentIndx][4]);
            setIntensity6(userLEDs[currentIndx][5]);
            enableSystem(1);
            state = 38;
            break;
        case 38:
            lights[5] = userLEDs[currentIndx][0] ;
            lights[13] = userLEDs[currentIndx][1];
            lights[21] = userLEDs[currentIndx][2] ;
            lights[29] = userLEDs[currentIndx][3] ;
            lights[37] = userLEDs[currentIndx][4] ;
            lights[45] = userLEDs[currentIndx][5];
            state = 39;
            break;
        case 39:
            setId(hexDigits[UID[0] >> 4]);
            enable();
            state = 46;
            break;
        case 46:
            if (msg[msgIndex] != '\0') {
                if (SIO_TXAvail()) {
                    SIO_PutChar(msg[msgIndex]);
                    msgIndex++;
                }
            } else {
                state = 47; 
            }
            break;
        case 47: 
            if (uidIndex < 5) {
                if (SIO_TXAvail()) {
                    SIO_PutChar(hexDigits[UID[uidIndex] >> 4]);  
                    state = 48; 
                }
            } else {
                state = 49;  
            }
            break;
        case 48: 
            if (SIO_TXAvail()) {
                SIO_PutChar(hexDigits[UID[uidIndex] & 0x0F]);  
                uidIndex++;
                state = 49; 
            }
            break;
        case 49:
            if (uidIndex < 5) {
                if (SIO_TXAvail()) {
                    SIO_PutChar('-');
                    state = 47; 
                }
            } else {
                state = 50; 
            }
            break;
        case 50:
             if (SIO_TXAvail()) {
                SIO_PutChar('\r');
                state = 51;
            }
            break;
        case 51:
            if (SIO_TXAvail()) {
                SIO_PutChar('\n');
                msgIndex = 0;
                state = 52;  
            }
            break;
        case 52:
            if (lights[msgIndex] != '\0') {
                if (SIO_TXAvail()) {
                    SIO_PutChar(lights[msgIndex]);
                    msgIndex++;
                }
            }
            else {
                state = 0;
            }
            break;
        case 53:
            setId('-');
            enable();
            state = 46;
            break;
            
    }
}