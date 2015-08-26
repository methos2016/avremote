avremote
========

Deprecated in favor of: https://github.com/nikharris0/usbremote

AVR firmware for decoding inrared remote signals for controlling your computer. Designed for the ATTiny2313 but compatible with any AVR MCU with USART support.

Compilation and Flashing
--------

Compiling and flashing fuses and firmware are all done using make.

    make 
    make fuses
    make flash
    



Pin Configuration
--------

Below is a basic diagram showing PIN configurations for the indicator LED, USART RX/TX, and the infrared receiver.

<pre>
            ATTiny2313 

              --  --
  USART RX  -| |  | |- Vcc
  USART TX  -|  --  |-
            -|      |-
            -|      |-
            -|      |-
            -|      |-
            -|      |-
            -|      |-
            -|      |- IR IN
        GND -|      |- LED + 
              ------
</pre>
