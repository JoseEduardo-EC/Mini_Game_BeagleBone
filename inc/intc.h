#ifndef INTC_H
#define INTC_H

void AINTC_init(void);

void AINTC_enableIRQ(unsigned int irq);

void ISR_Handler(void);

void gpio_ISR(void);

void timer_ISR(void);


extern volatile unsigned int flag;

extern volatile unsigned int flag_led_atual;
extern volatile unsigned int flag_acerto;
extern volatile unsigned int reg_pontos;
extern volatile unsigned int flag_start;
extern volatile unsigned int flag_trava;
#endif