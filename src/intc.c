#include "soc_AM335x.h"
#include "hw_types.h"
#include "bbb_regs.h"
#include "intc.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"

/*
========================================================
       NAME: AINTC_init
 PARAMETROS: void
    RETORNO: void
DESCRIPTION: Função que reseta o INTC e desabilita o filtro de prioridade de interrupções
========================================================
*/

void AINTC_init(void){
    //Operação que reseta o INTC, para que escritas anteriores não interfiram
    HWREG(SOC_AINTC_REGS + INTC_SYSCONFIG) |= INTC_SYSCONFIG_SOFTRESET;

    //Verifica se o bit ResetDone ainda está em trasnsição
    while(!(HWREG(SOC_AINTC_REGS + INTC_SYSSTATUS) & INTC_SYSSTATUS_RESETDONE));

    //Desabilita o filtro de prioridade de interrupções
    HWREG(SOC_AINTC_REGS + INTC_THRESHOLD) |= INTC_THRESHOLD_PRIORITYTHRESHOLD;
}

/*
========================================================
       NAME: AINTC_enableIRQ
 PARAMETROS: irq -> número da interrupção (0-127)
    RETORNO: void
DESCRIPTION: Habilita uma interrupção no controlador INTC
             calculando automaticamente o grupo MIR
             correspondente e liberando o bit associado
========================================================
*/

void AINTC_enableIRQ(unsigned int irq){
    unsigned int mir = irq / 32;
    unsigned int bit = irq % 32;

    switch (mir)
    {
    case 0:
        HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR0) |= (1 << bit);
        break;
    case 1:
        HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR1) |= (1 << bit);
        break;
    case 2:
        HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR2) |= (1 << bit);
        break;
    case 3:
        HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR3) |= (1 << bit);
        break;
    default:
        break;
    }
}

/*
========================================================
       NAME: ISR_Handler
 PARAMETROS: void
    RETORNO: void
DESCRIPTION: Identifica a IRQ ativa através do INTC,
             redireciona a execução para a rotina de
             tratamento correspondente e sinaliza ao
             controlador de interrupções o término do
             atendimento da IRQ.
========================================================
*/

void ISR_Handler(void){
    //Verifica qual o número da interrupção de 0 a 127
    unsigned int irq = HWREG(SOC_AINTC_REGS + INTC_SIR_IRQ) & 0x7F;

    switch (irq)
    {
    case 98: //Gpio1
        gpio_ISR();
        break;
    case 95:    //Dmtier7
        timer_ISR();
        break;
    default:
        break;
    }

    //Sinaliza ao INTC que terminou de tartar a interrupção
    HWREG(SOC_AINTC_REGS + INTC_CONTROL) = 0x01;
}

//Falgs
volatile unsigned int flag_led_atual;
volatile unsigned int flag_acerto = 0;
volatile unsigned int reg_pontos = 0;
volatile unsigned int flag_trava = 0;
volatile unsigned int flag_start = 0;

void gpio_ISR(void){
    //Verifica quem causou a interrupção
    unsigned int bit = HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_0);
    
    if(bit & (1 << 13)){
        //Limpa a flag
        gpio_clearIRQ(SOC_GPIO_1_REGS, 13);
            if(flag_trava == 1) return;
            flag_trava = 1;

        if(flag_led_atual == 0){
            flag_acerto = 1;
            
        }
    }
    if(bit & (1 << 15)){
        gpio_clearIRQ(SOC_GPIO_1_REGS, 15);
            if(flag_trava == 1) return;
            flag_trava = 1;

        if(flag_led_atual == 1){
            flag_acerto = 1;
            
        }
    }
    
    if(bit & (1 << 16)){
        //Limpa a flag
        gpio_clearIRQ(SOC_GPIO_1_REGS, 16);
            if(flag_trava == 1) return;
            flag_trava = 1;
        
        if(flag_led_atual == 2){
            flag_acerto = 1;
            
        }
        
    }

    if(bit & (1 << 28)){
        //Limpa a flag 
        gpio_clearIRQ(SOC_GPIO_1_REGS, 28);
            if(flag_trava == 1) return;
            flag_trava = 1;

        if(flag_led_atual == 3){
            flag_acerto = 1;
            
        }
    }

    if(bit & (1 << 12)){
        gpio_clearIRQ(SOC_GPIO_1_REGS, 12);
            if(flag_trava == 1) return;
            flag_trava = 1;
        flag_start = 1;
    }

}

volatile unsigned int flag = 0;
void timer_ISR(void){
    flag_trava = 0;
    flag = 1;

    //Limpa a flag
    timer_clearIRQ_overflow(SOC_DMTIMER_7_REGS);   
}

