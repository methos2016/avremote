#ifndef _CONFIG_H_
#define _CONFIG_H_


/* IR bit definitions */
#define IR_BITS         32
#define IR_BIT_START    4000
#define IR_BIT_1        1500
#define IR_BIT_0        450
#define IR_PORT         PORTB
#define IR_PIN          PINB0


/* USART settings */
#define USART_BAUD      9600
#define USART_PORT      PORTD
#define USART_TX_PIN    PIND1
#define USART_RX_PIN    PIND0


/* indicator LED definitions */
#define LED_PORT        PORTD
#define LED_PIN         PIND6

#endif

