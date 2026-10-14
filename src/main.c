#include "soc_AM335x.h"
#include "hw_types.h"
#include "bbb_regs.h"
#include "clock.h"
#include "gpio.h"
#include "wdt.h"
#include "timer.h"
#include "uart.h"
#include "intc.h"
#include "display.h"


void _main(){
    //Seta o clock dos periféricos
    set_clock_minigame();
    
    //seta o modo POSTED dos timers
    set_POSTED(SOC_DMTIMER_7_REGS);
    set_POSTED(SOC_DMTIMER_2_REGS);

    //Liga o TCRR do segundo timer que irá ser utilizaado para decidir o led
    timer_run(SOC_DMTIMER_2_REGS);

    //Desabilita o whatchdog
    disable_watchdog();

    //Ativar o INTC
    AINTC_init();

    //configuração inicial dos GPIOS para Led e botão
    init_gpio_minigame();

    //Ativando a interrupção em INTC
    AINTC_enableIRQ(98);    //GPIO1
    AINTC_enableIRQ(95);    //DMTIMER7

    //Habilitando o periférico a mandar o sinal de interrupção
    gpio_setIRQ(SOC_GPIO_1_REGS, 13); //botão 1
    gpio_setIRQ(SOC_GPIO_1_REGS, 15); //botão 2
    gpio_setIRQ(SOC_GPIO_1_REGS, 16); //botão 3
    gpio_setIRQ(SOC_GPIO_1_REGS, 28); //botão 4

    gpio_setIRQ(SOC_GPIO_1_REGS, 12); //botão start

    //menu
    prints(SOC_UART_0_REGS, "\033[H\033[J");
    
    while (1){
        prints(SOC_UART_0_REGS, "press start buton\n\r");

        flag_start = 0;
        while(!flag_start);
        prints(SOC_UART_0_REGS, "\033[H\033[J");
        display_clear();
        prints(SOC_UART_0_REGS, "Start game\n\r");

        volatile unsigned int reg_pontos = 0;
        
        for(volatile unsigned int i = 0; i < 7; i++){
            flag_acerto = 0;
            gpio_off(SOC_GPIO_1_REGS, 2);
            gpio_off(SOC_GPIO_1_REGS, 3);
            gpio_off(SOC_GPIO_1_REGS, 6);
            gpio_off(SOC_GPIO_1_REGS, 7);
            timer_setupOverflow(SOC_DMTIMER_7_REGS, 500);

            flag_led_atual = HWREG(SOC_DMTIMER_2_REGS + DMTIMER_TCRR) % 4;

            switch (flag_led_atual)
            {
            case 0:
                gpio_on(SOC_GPIO_1_REGS, 2);
                timer_setupOverflow(SOC_DMTIMER_7_REGS, 750);
                gpio_off(SOC_GPIO_1_REGS, 2);
                break;
            
            case 1:
                gpio_on(SOC_GPIO_1_REGS, 3);
                timer_setupOverflow(SOC_DMTIMER_7_REGS, 750);
                gpio_off(SOC_GPIO_1_REGS, 3);
                break;

            case 2:
                gpio_on(SOC_GPIO_1_REGS, 6);
                timer_setupOverflow(SOC_DMTIMER_7_REGS, 750);
                gpio_off(SOC_GPIO_1_REGS, 6);
                break;

            case 3:
                gpio_on(SOC_GPIO_1_REGS, 7);
                timer_setupOverflow(SOC_DMTIMER_7_REGS, 750);
                gpio_off(SOC_GPIO_1_REGS, 7);
                break;

            default:
                break;
            }

            if(flag_acerto == 1) reg_pontos++;
        }
        prints(SOC_UART_0_REGS, "Fim de jogo.... Pontos: ");
        putch(SOC_UART_0_REGS, '0' + reg_pontos);
        prints(SOC_UART_0_REGS, "/7\n\r");

        display_show('0' + reg_pontos);
    }
}

