#ifndef _SIO_H_
#define _SIO_H_

#define TRUE 1
#define FALSE 0

void SIO_Init(void);
// Post: Configures the UART in asynchronous mode

unsigned char SIO_RXAvail(void);
// Post: Returns true if there is a recieved character pending to be read, flase otherwise

unsigned char SIO_GetChar(void);
// Pre: SIO_RXAvail() returned true
// Post: performs a destructive read of the recieved character

unsigned char SIO_TXAvail(void);
// Post: returns true if its availabe to send a character, false otherwise

void SIO_PutChar (unsigned char Value);
// Pre: SIO_TXAvail() has returned true
// Post: queues a new character for transmission

#endif