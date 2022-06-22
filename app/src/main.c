#include "stm8s.h"

/*------------------------------------------------------------------------------------------------ */
// Makra a proměnné tlačítka
#define PORT_BUTTONS (GPIOC) // Port tlačítek
#define BUTTON1 (PIN_7)      // TON1
#define BUTTON2 (PIN_6)      // TON2

// Makra pro reproduktor
#define PORT_REPRO (GPIOB) // Port reproduktor
#define REPRO1 (PIN_0)     // Pin reproduktor

GPIO_Pin_TypeDef Buttons[2] = {BUTTON1, BUTTON2}; // Tlačítka zvonku
#define buttonPressed(BUTTON) (GPIO_ReadInputPin(PORT_BUTTONS, BUTTON) == RESET)

// ton delka proměnné
bool state = FALSE;     // Stav časovače pro generování tonu (FALSE = neaktivní, TRUE = odpočítává)
uint16_t buzzCount = 0; // Počítání reprák
uint32_t timeUnit = 0;  // Jednotka pro počítání času (500 = 1 sekunda)
/*------------------------------------------------------------------------------------------------ */
int changeState() // Změna stavu
{
    if (state == FALSE)
    {
        state = TRUE;
        buzzCount = 0;
    }
    else
    {
        state = FALSE;
    }
    return state;
}
/*------------------------------------------------------------------------------------------------ */
void reset()
{
    state = FALSE;
    GPIO_WriteLow(PORT_REPRO, REPRO1); // Zastavení bzučení, pokud je v provozu
}

// 10ti-sekundový ton
void ton1(int timeUnit)
{
    if (buzzCount < 10)
    {
        // Chvíli ticho, chvíli bzučí
        if (timeUnit == 100)
        {
            GPIO_WriteReverse(PORT_REPRO, REPRO1);
        }
        // Po 10tisekundách se se zastaví zvonek
    }
    else if (buzzCount == 10)
    {
        GPIO_WriteLow(PORT_REPRO, REPRO1);
        state = 0;
    }
}
/*------------------------------------------------------------------------------------------------ */
// 10ti-sekundový ton
void ton2(int timeUnit)
{
    if (buzzCount < 10)
    {
        // Chvíli ticho, chvíli bzučí
        if (timeUnit == 100)
        {
            GPIO_WriteReverse(PORT_REPRO, REPRO1);
        }
        // Po 10tisekundách se se zastaví bzučení a stav odpočtu se nastaví na neaktivní
    }
    else if (buzzCount == 10)
    {
        GPIO_WriteLow(PORT_REPRO, REPRO1);
        state = 0;
    }
}
/*------------------------------------------------------------------------------------------------ */
// Přerušení
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
    if (buttonPressed(BUTTON1))
    {
        changeState();
        printf("button1\n");
    }

    else if (buttonPressed(BUTTON2))
    {

        printf("button2\n");
    }
}
/*------------------------------------------------------------------------------------------------ */
int main(void)
{
    GPIO_DeInit;                                   // Deinicializace portů
    TIM4_DeInit;                                   // Deinicializace časovače
    EXTI_DeInit;                                   // Deinicializace přerušení
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovat MCU na 16MHz
    Serial.begin(9600);                            // Inicializace Serial monitoru
    for (uint8_t i = 0; i < 2; i++)                // Inicializace tlačítek
    {
        GPIO_Init(PORT_BUTTONS, Buttons[i], GPIO_MODE_IN_PU_IT);
    }

    // Nastavení přerušení
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_ONLY);
    ITC_SetSoftwarePriority(ITC_IRQ_PORTC, ITC_PRIORITYLEVEL_0);
    enableInterrupts();
    printf("Proběhla incializace\n");
    // časovač
    // prescale for 1 ms - P = (16 * 10 '6) / (500 * 2 '8) = 125 = 128
    // top for 1ms - C = (16 * 10 '6) / (500 * 125) = 250
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 250); // Inicializace časovače
    TIM4_Cmd(ENABLE);

    while (1)
    {
        if (TIM4_GetFlagStatus(TIM4_FLAG_UPDATE) == SET)
        {
            TIM4_ClearFlag(TIM4_FLAG_UPDATE);
            timeUnit++;

            if (timeUnit == 500)
            {
                timeUnit = 0;
                buzzCount++;
            }
            buzz(timeUnit);
        }
    }
}
/*------------------------------------------------------------------------------------------------ */
