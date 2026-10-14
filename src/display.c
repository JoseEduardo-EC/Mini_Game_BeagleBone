#include "display.h"
#include "gpio.h"
#include "soc_AM335x.h"

#define LED1    2
#define LED2    3
#define LED3    4
#define LED4    5
#define LED5    22
#define LED6    23
#define LED7    24



void display_clear(void)
{
    gpio_off(SOC_GPIO_2_REGS, LED1);
    gpio_off(SOC_GPIO_2_REGS, LED2);
    gpio_off(SOC_GPIO_2_REGS, LED3);
    gpio_off(SOC_GPIO_2_REGS, LED4);
    gpio_off(SOC_GPIO_2_REGS, LED5);
    gpio_off(SOC_GPIO_2_REGS, LED6);
    gpio_off(SOC_GPIO_2_REGS, LED7);

}

void display_init(void)
{
    set_dir(SOC_GPIO_2_REGS, 0, LED1);
    set_dir(SOC_GPIO_2_REGS, 0, LED2);
    set_dir(SOC_GPIO_2_REGS, 0, LED3);
    set_dir(SOC_GPIO_2_REGS, 0, LED4);
    set_dir(SOC_GPIO_2_REGS, 0, LED5);
    set_dir(SOC_GPIO_2_REGS, 0, LED6);
    set_dir(SOC_GPIO_2_REGS, 0, LED7);

    display_clear();
}

void display_show(char digit)
{
    display_clear();

    switch(digit)
    {
        case '1':
            gpio_on(SOC_GPIO_2_REGS, LED1);
            break;

        case '2':
            gpio_on(SOC_GPIO_2_REGS, LED1);
            gpio_on(SOC_GPIO_2_REGS, LED2);
            break;

        case '3':
            gpio_on(SOC_GPIO_2_REGS, LED1);
            gpio_on(SOC_GPIO_2_REGS, LED2);
            gpio_on(SOC_GPIO_2_REGS, LED3);
            break;

        case '4':
            gpio_on(SOC_GPIO_2_REGS, LED1);
            gpio_on(SOC_GPIO_2_REGS, LED2);
            gpio_on(SOC_GPIO_2_REGS, LED3);
            gpio_on(SOC_GPIO_2_REGS, LED4);

            break;

        case '5':
            gpio_on(SOC_GPIO_2_REGS, LED1);
            gpio_on(SOC_GPIO_2_REGS, LED2);
            gpio_on(SOC_GPIO_2_REGS, LED3);
            gpio_on(SOC_GPIO_2_REGS, LED4);
            gpio_on(SOC_GPIO_2_REGS, LED5);

            break;

        case '6':
            gpio_on(SOC_GPIO_2_REGS, LED1);
            gpio_on(SOC_GPIO_2_REGS, LED2);
            gpio_on(SOC_GPIO_2_REGS, LED3);
            gpio_on(SOC_GPIO_2_REGS, LED4);
            gpio_on(SOC_GPIO_2_REGS, LED5);
            gpio_on(SOC_GPIO_2_REGS, LED6);

            break;

        case '7':
            gpio_on(SOC_GPIO_2_REGS, LED1);
            gpio_on(SOC_GPIO_2_REGS, LED2);
            gpio_on(SOC_GPIO_2_REGS, LED3);
            gpio_on(SOC_GPIO_2_REGS, LED4);
            gpio_on(SOC_GPIO_2_REGS, LED5);
            gpio_on(SOC_GPIO_2_REGS, LED6);
            gpio_on(SOC_GPIO_2_REGS, LED7);

            break;
        default:
            display_clear();
            break;
    }
}