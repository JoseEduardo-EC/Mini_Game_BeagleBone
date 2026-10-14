#include "hw_types.h"
#include "soc_AM335x.h"
#include "bbb_regs.h"
#include "gpio.h"
#include "uart.h"
#include "display.h"


#define GPIO1_2     0x808 //led 1
#define GPIO1_3     0x80C //led 2
#define GPIO1_6     0x818 //Led 3
#define GPIO1_7     0x81C //led 4

#define GPIO1_13    0x834 //botão 1
#define GPIO1_15    0x83C //botão 2
#define GPIO1_16    0x840 //botão 3
#define GPIO1_28    0x878 //botão 4
#define GPIO1_12    0x830 //Botão start

#define GPIO2_2     0x890 //Led_display 1
#define GPIO2_3     0x894 //Led_display 2
#define GPIO2_4     0x898 //Led_display 3
#define GPIO2_5     0x89C //Led_display 4
#define GPIO2_22    0x8E8 //Led_display 5
#define GPIO2_23    0x8EC //Led_display 6
#define GPIO2_24    0x8F0 //Led_display 7



/*
========================================================
       NAME: set_PINMUX
 PARAMETROS: offset do pino e a ativação para a sua função
    RETORNO: void
DESCRIPTION: Função que ativa a multiplexação do pino para o modulo desejado.
========================================================
*/
void set_PINMUX(unsigned int offset, unsigned int mode){
    HWREG(SOC_CONTROL_REGS + offset) |= mode;
}

/*
========================================================
       NAME: set_dir
 PARAMETROS: Endereço base do GPIO a direção e o bit 
    RETORNO: void
DESCRIPTION: Função que ativa a direção do pino para trabalhar como saída (dir = 0) ou entrada (dir = 1)
========================================================
*/
void set_dir(unsigned int gpio_base, unsigned int dir, unsigned int bit){
    switch (dir)
    {
        //Saída
    case 0:
        HWREG(gpio_base + GPIO_OE) &= ~(1 << bit);
        break;
        //Entrada
    case 1:
        HWREG(gpio_base + GPIO_OE) |= (1 << bit);
        break;       
    default:
        break;
    }
}

/*
========================================================
       NAME: gpio_on
 PARAMETROS: Endereço base do GPIO, bit 
    RETORNO: void
DESCRIPTION: Função que coloca o gpio em nível alto
========================================================
*/

void gpio_on(unsigned int gpio_base, unsigned int bit){
    HWREG(gpio_base + GPIO_SETDATAOUT) |= (1 << bit);
}


/*
========================================================
       NAME: gpio_off
 PARAMETROS: Endereço base do GPIO, bit 
    RETORNO: void
DESCRIPTION: Função que coloca o gpio em nível baixo
========================================================
*/

void gpio_off(unsigned int gpio_base, unsigned int bit){
    HWREG(gpio_base + GPIO_CLEARDATAOUT) |= (1 << bit);
}

/*
========================================================
       NAME: gpio_setIRQ
 PARAMETROS: Endereço base do GPIO, bit 
    RETORNO: void
DESCRIPTION: Função que habilita a irq no pino do GPIO 
             Seta como ocorre a irq, (borda de subida 0->1)
========================================================
*/


void gpio_setIRQ(unsigned int gpio_base, unsigned int bit){
    //Seta interrupção
    HWREG(gpio_base + GPIO_IRQSTATUS_SET_0) |= (1 << bit);

    //Seta como ocorre
    HWREG(gpio_base + GPIO_RISINGDETECT) |= (1 << bit);
}

/*
========================================================
       NAME: gpio_clearIRQ
 PARAMETROS: Endereço base do GPIO, bit 
    RETORNO: void
DESCRIPTION: Função que limpa a flag de interrupção, sem ela o GPIO fica disparando infinitamente
========================================================
*/

void gpio_clearIRQ(unsigned int gpio_base, unsigned int  bit){
    HWREG(gpio_base + GPIO_IRQSTATUS_0) = (1 << bit);
}


void init_gpio_minigame(void){
    
    //leds
    set_PINMUX(GPIO1_2, PINMUX_GPIO_OU);
    set_PINMUX(GPIO1_3, PINMUX_GPIO_OU);
    set_PINMUX(GPIO1_6, PINMUX_GPIO_OU);
    set_PINMUX(GPIO1_7, PINMUX_GPIO_OU);

    //botões
    set_PINMUX(GPIO1_13, PINMUX_GPIO_IN);
    set_PINMUX(GPIO1_15, PINMUX_GPIO_IN);
    set_PINMUX(GPIO1_16, PINMUX_GPIO_IN);
    set_PINMUX(GPIO1_28, PINMUX_GPIO_IN);
    
    // Pinmux do display 
    set_PINMUX(GPIO2_2,  PINMUX_GPIO_OU);
    set_PINMUX(GPIO2_3,  PINMUX_GPIO_OU);
    set_PINMUX(GPIO2_4,  PINMUX_GPIO_OU);
    set_PINMUX(GPIO2_5,  PINMUX_GPIO_OU);
    set_PINMUX(GPIO2_22, PINMUX_GPIO_OU);
    set_PINMUX(GPIO2_23, PINMUX_GPIO_OU);
    set_PINMUX(GPIO2_24, PINMUX_GPIO_OU);
    
    //saida
    set_dir(SOC_GPIO_1_REGS, 0, 2); //led 1
    set_dir(SOC_GPIO_1_REGS, 0, 3); //led 2
    set_dir(SOC_GPIO_1_REGS, 0, 6); //led 3
    set_dir(SOC_GPIO_1_REGS, 0, 7); //led 4

    display_init();
    
    //entrada
    set_dir(SOC_GPIO_1_REGS, 1, 13); //botão 1
    set_dir(SOC_GPIO_1_REGS, 1, 15); //botão 2
    set_dir(SOC_GPIO_1_REGS, 1, 16); //botão 3
    set_dir(SOC_GPIO_1_REGS, 1, 28); //botão 4
    set_dir(SOC_GPIO_1_REGS, 1, 12); //botaõ start
}