/**
 * @author [Petr Oblouk]
 * @github [https://github.com/peoblouk]
 * @create date 15-04-2022 - 16:46:28
 * @modify date 19-04-2022 - 19:24:41
 * @desc [description]
 */
/* -------------------------------------------------------------------------------------------*/

#include "stm8s.h"
#include "stm8s_milis.h"
#include "stm8s_4segment.h"

uint32_t mtime_key = 0;
bool status = FALSE;

// tlačítko č. 1
#define port_tlac1 GPIOF
#define pin_tlac1 PIN_7

// tlačítko č. 2
#define port_tlac2 GPIOF
#define pin_tlac2 PIN_6

// tlačítko č. 3
#define port_tlac3 GPIOF
#define pin_tlac3 PIN_5

// tlačítko č. 4
#define port_tlac4 GPIOF
#define pin_tlac4 PIN_6

uint16_t milisec = 0;
uint16_t sec2 = 0;
uint16_t sec1 = 0;
uint16_t min1 = 0;
uint16_t min2 = 0;

int main(void)
{
    GPIO_DeInit;
    TIM4_DeInit;
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovat MCU na 16MHz
    Serial.begin(9600);                            // Inicializace Serial monitoru
    milis.init();                                  // Initializace milis
    display.init();
    display.write(0000);
    // inicializace tlačítek
    GPIO_Init(port_tlac1, pin_tlac1, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(port_tlac2, pin_tlac2, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(port_tlac3, pin_tlac3, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(port_tlac4, pin_tlac4, GPIO_MODE_IN_PU_NO_IT);

    // prescale for 1 ms - P = (16 * 10 '6) / (1000 * 2 '8) = 62,5
    // top for 1ms - C = (16 * 10 '6) / (1000 * 62,5) = 256

    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 250);
    uint32_t timeUnit = 0;
    TIM4_Cmd(ENABLE);

    printf("Proběhla incializace\n");
    while (1)
    {
        // GPIO_Write(GPIOD, 0b00000000);

        if (milis.get() - mtime_key > 25) // počkej 25 ms a zkontroluj tlačítka
        {
            mtime_key = milis.get();                             // milis now
            if (GPIO_ReadInputPin(port_tlac1, pin_tlac1) == SET) // tlačítko č. 1 (SET)
            {
                TIM4_Cmd(ENABLE);
                printf("tlač. SET\n");
            }
            else if (GPIO_ReadInputPin(port_tlac2, pin_tlac2) == RESET) // tlačítko č. 2 (SAVE TIME TO MEMORY)
            {
                char mezicas[] = {min1};
                printf("tlač. Mezičas\n");
            }
            else if (GPIO_ReadInputPin(port_tlac3, pin_tlac3) == RESET) // tlačítko č. 3 (RESET)
            {
                TIM4_ClearFlag(TIM4_FLAG_UPDATE);
                printf("tlač. RESET\n");
            }

            else if (GPIO_ReadInputPin(port_tlac4, pin_tlac4) == RESET) // tlačítko č. 4 (BROWSE TIME IN MEMORY)
            {
                printf("tlač. Prohlížení časů\n");
                delay.ms(1000);
            }

            if (TIM4_GetFlagStatus(TIM4_FLAG_UPDATE) == SET)
            {
                TIM4_ClearFlag(TIM4_FLAG_UPDATE);
                timeUnit++;
            }
            if (timeUnit == 500)
            {
                timeUnit = 0;
                sec1 += 1;
            }

            // Pokud uběhla minuta, přičti jednu minut k času
            if (sec1 > 10)
            {
                sec1 = 0;

                sec2 += 1;
                if (sec2 == 10)
                {
                    sec2 = 0;

                    min1 += 1;
                    if (min1 = 10)
                    {
                        min1 = 0;

                        min2 += 1;
                        if (min2 == 10)
                        {
                            min2 = 0;
                        }
                    }
                }

                //

                //
            }
            status = TRUE;
            printf("%d\n", sec1);
        }
        display.num(min2, min1, sec2, sec1);

        if (min2 >= 100)
        {
            printf("autoreset\n");
            status = FALSE;
            TIM4_ClearFlag(TIM4_FLAG_UPDATE);
        }
    }
}

/*
        for (uint16_t i = 0; i < 9999; i++)
        {
            milisec += 1;
            // display.write(milisec);
            display.num(min, sec, milisec);
            delay.ms(100);
        }

        if (milisec >= 9)
        {
            milisec = 0;
            sec += 1;
            if (sec == 60)
            {
                min = 1;
            }
        }
*/