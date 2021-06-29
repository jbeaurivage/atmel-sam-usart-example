#include "sam.h"

#define F_CORE 1000000U

#define usart SERCOM0->USART
#define port0 PORT->Group[0]
const uint32_t BAUD = 9600;

void main() {
    // Setup PA10 and PA11 as alternate function C
    port0.PMUX[6].reg = PORT_PMUX_PMUXE_C | PORT_PMUX_PMUXO_C;

    // Configure PA05 as OUTPUT LOW
    port0.DIRSET.reg = (1 << 5);
    port0.OUTCLR.reg = (1 << 5);

    // Software reset SERCOM
    usart.CTRLA.reg |= SERCOM_USART_CTRLA_SWRST;
    while(usart.SYNCBUSY.bit.SWRST) {}

    // Setup Pads
    usart.CTRLA.bit.RXPO = 0x3; // RX = Pad 3
    usart.CTRLA.bit.TXPO = 0x1; // TX = Pad 2

    // Internal clock mode
    usart.CTRLA.bit.MODE = 0x0;
    
    // 16x oversampling with fractional baud mode
    usart.CTRLA.bit.SAMPR = 0x1;

    uint32_t baud_times_8 = (F_CORE * 8) / (16 * BAUD);
    usart.BAUD.FRAC.FP = baud_times_8 % 8;
    usart.BAUD.FRAC.BAUD = baud_times_8 / 8;

    usart.CTRLA.bit.DORD = 1; // LSB-first

    // Enable receiver, transmitter, and peripheral
    usart.CTRLB.reg |= SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN;
    usart.CTRLA.bit.ENABLE = 1;

    // Wait for 10 ms so we get a buffer overflow
    for(uint16_t i = 0; i < 10000; i++){ asm("nop"); }

    asm("bkpt");

    // Disabling the receiver is supposed to clear the RX buffer
    // and clear the FERR, PERR and BUFOVF status flags
    usart.CTRLB.bit.RXEN = 0;
    while(usart.SYNCBUSY.bit.CTRLB) {}

    usart.CTRLB.bit.RXEN = 1;
    while(usart.SYNCBUSY.bit.CTRLB) {}

    // Read a couple bytes to make BUFOVF bubble up
    for(uint8_t i = 0; i < 3; i++){
        volatile uint16_t data = usart.DATA.reg;
    }

    // At this breakpoint we can see that BUFOVF isn't cleared
    asm("bkpt");



}