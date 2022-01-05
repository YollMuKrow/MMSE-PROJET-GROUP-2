//
// Created by Alexis Lheritier on 02/01/2022.
//

#ifndef USART_COM_H
#define USART_COM_H

#include <avr/interrupt.h>
// HORLOGE

extern uint8_t phase_shift;

extern uint8_t seconds;
extern uint8_t minutes;
extern uint8_t hours;

volatile int i = 0;
volatile char ReceivedByte = 0;
volatile int mode = 0;
unsigned char buffer[32];

// SETTER TIME
void set_seconds(unsigned int sec1, unsigned int sec0)
{
    seconds = 10 * sec1 + sec0;
}

void set_minutes(unsigned int min1, unsigned int min0)
{
    minutes = 10 * min1 + min0;
}

void set_hours(unsigned int hrs1, unsigned int hrs0)
{
    hours = 10 * hrs1 + hrs0;
}

// USART COMMUNICATION
void USART_Init(unsigned int ubrr)
{
    UCSR0A &= ~(1 << U2X0);
    /*Set baud rate */
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    /*Enable receiver and transmitter and each interrupt*/
    //UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<UDRIE0);          //(1<<TXCIE0)|(1<<RXCIE0);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    /* Set frame format: 8data, 1 stop bit */
    UCSR0C = (1 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);
}
void Interrupt_Init()
{
    // Enables buffer-available interruption
    UCSR0B |= _BV(UDRIE0);
}

// DATA ANALYSIS
unsigned int uchar_to_int(unsigned char n)
{
    return (unsigned int)n - '0';
}

void data_handler(unsigned char data_bytes)
{
    //format heure = h12:13:45:
    if (data_bytes == 'h')
    {
        i = 0;
    }
    buffer[i] == data_bytes;

    if (data_bytes == ':')
    {
        if (i > 2)
        {
            set_hours(uchar_to_int(buffer[1]), uchar_to_int(buffer[2]));
        }
        if (i > 5)
        {
            set_minutes(uchar_to_int(buffer[4]), uchar_to_int(buffer[5]));
        }
        if (i > 8)
        {
            set_seconds(uchar_to_int(buffer[7]), uchar_to_int(buffer[8]));
        }
    }

    if (data_bytes == '\r')
    {
        i = 0;
    }

    if(data_bytes == 'p'){
        phase_shift = uchar_to_int(buffer[i-3])*100 + uchar_to_int(buffer[i-2])*10 + uchar_to_int(buffer[i-1]);
        i = 0;
    }
}

unsigned char USART_Receive(void)
{
    /* Wait for data to be received */
    while (!(UCSR0A & (1 << RXC0)))
        ;
    /* Get and return received data from buffer */
    return UDR0;
}

void USART_Transmit(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

void USART_Transmit_str(unsigned char *data)
{
    int i = 0;
    while (data[i] != '\0')
    {
        USART_Transmit(data[i]);
        i++;
    }
}

// USART INTERRUPT
ISR(USART_RX_vect)
{
    ReceivedByte = UDR0;
    data_handler(ReceivedByte);
    i++;
    UDR0 = ReceivedByte;
    /* The receive buffer (UDR0) must be read during the reception ISR, or
         * the ISR will just
        *  execute immediately upon exiting. */
}

ISR(USART_UDRE_vect)
{
    //cnt++;
    // Disable this interrupt
    UCSR0B &= ~(1 << UDRIE0);
}

#endif //USART_COM_H
