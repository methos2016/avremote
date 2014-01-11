#define F_CPU   8000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "usart.h"

#define BAUD_PRESCALE   (((F_CPU / (USART_BAUD * 16UL))) - 1)


/*
 *            ATTiny2313 
 *
 *              --  --
 *  USART RX  -| |  | |- Vcc
 *  USART TX  -|  --  |-
 *            -|      |-
 *            -|      |-
 *            -|      |-
 *            -|      |-
 *            -|      |-
 *            -|      |-
 *            -|      |- IR IN
 *        GND -|      |- LED + 
 *              ------
 *
 */


int main(void)
{
    /* disable interrupts */
    cli();	
    
    /* input for IR and rest, no pull up resistor */
    DDRB = 0x00;
    PORTB &= ~_BV(PORTB0);
    
    /* LED output pin, off */
    DDRD |= _BV(LED_PIN);
    LED_PORT &= ~_BV(LED_PIN);
    
    /* initialize USART  */
    usart_init(BAUD_PRESCALE);

    /* configure 16bit timer 1 for 1uS @ 8MHz using clk/8 prescaler */
    TCCR1B |= (1 << CS11);
    
    /* flash LED to confirm initialization complete */
    LED_PORT |= _BV(LED_PIN);
    _delay_ms(500);
    LED_PORT &= ~_BV(LED_PIN);

    int in_phase = 0;

	while(1) {
	
        /* if we are not in phase and the IR pin is high, we are now in phase,
         * so we reset the timer and start recording how long this phase length is
         */
  
        if(in_phase == 0 && (PINB & _BV(IR_PIN))) {
            in_phase = 1;
            TCNT1 = 0;
            continue;
        }
        
        /* if we are in phase and the IR pin goes low, we cut off the timer and measure the 
         * phase length. if it's greater than the length of our IR start bit, we assume
         * the data bits will follow
         */
        if(in_phase == 1 && !(PINB & _BV(IR_PIN))) {
            int phase_len = TCNT1;
            in_phase = 0;
            if(phase_len >= IR_BIT_START && phase_len < (IR_BIT_START * 2)) {
                
                /* LED pin high to indicate a signal */
                LED_PORT |= _BV(LED_PIN);
            
                uint32_t value = 0;
                int x;
                for(x = 0; x < IR_BITS; x++) {
                    
                    /* wait for a new phase to start. once one does, we reset our timer
                     * and wait for it to end
                     */
                    while(!(PINB & _BV(IR_PIN)));
                    TCNT1 = 0;
                    while((PINB & _BV(IR_PIN)));

                    phase_len = TCNT1;
                    
                    /* translate the phase length to bit value and shift into our final value */
                    if(phase_len >= IR_BIT_1) {
                        value |= (1 << (IR_BITS - x));
                    } else if(phase_len >= IR_BIT_0) {
                       /* nothing needed for this bit to remain 0 */
                    }
                    else {
                       /* for now if it doesn't match up with any of our phase lengths, we mark the value
                        * as an error and we will ignore it
                        */
                        value = -1;
                        break;
                    }
                }
                
                /* if we successfully received the IR data we transmit through usart*/
                if(value != -1) {
                    char v_str[50] = {0};
                    ultoa(value, v_str, 10);
                    
                    int x;
                    for(x = 0; x < strlen(v_str); x++) {
                        usart_tx(v_str[x]);
                    }
                    
                    usart_tx('\n');
                }               
                
                
                LED_PORT &= ~_BV(LED_PIN);
            }
        }
        

	}

}


