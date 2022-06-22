#include "stm8s_4segment.h"

/* -------------------------------------------------------------------------------------------*/
//   0    1    2    3    4    5    6    7    8    9    -
// {192, 249, 164, 176, 153, 146, 130, 248, 128, 144, 255}; // numbers
char numbers[12] =
    {
        0b11000000, // 0
        0b11111001, // 1
        0b10100100, // 2
        0b10110000, // 3
        0b10011001, // 4
        0b10010010, // 5
        0b10000010, // 6
        0b11111000, // 7
        0b10000000, // 8
        0b10010000, // 9
        0b10111111, // -
        0b11111111  // NOTHING
};

char numbers_with_dot[12] =
    {
        0b01000000, // 0
        0b01111001, // 1
        0b00100100, // 2
        0b00110000, // 3
        0b00011001, // 4
        0b00010010, // 5
        0b00000010, // 6
        0b01111000, // 7
        0b00000000, // 8
        0b00010000, // 9
        0b00111111, // -
        0b01111111  // NOTHING
};

#define dgt_1_port GPIOB
#define dgt_2_port GPIOD
#define dgt_3_port GPIOE
#define dgt_4_port GPIOG

void display_init(void)
{
  GPIO_Init(dgt_1_port, PIN_ALL, GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_Init(dgt_2_port, PIN_ALL, GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_Init(dgt_3_port, PIN_ALL, GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_Init(dgt_4_port, PIN_ALL, GPIO_MODE_OUT_PP_HIGH_SLOW);
}

/* -------------------------------------------------------------------------------------------*/
void display_sep(uint32_t display)
{
  uint32_t temp1 = (display / 1000);       // thousands
  uint32_t temp2 = ((display / 100) % 10); // hundreds
  uint32_t temp3 = ((display / 10) % 10);  // tens
  uint32_t temp4 = (display % 10);         // unit

  char displayed_numbers[4] = {temp1, temp2, temp3, temp4}; // save variables to array

  // první idigt
  GPIO_Write(dgt_1_port, numbers[displayed_numbers[0]]);

  // druhý digit
  GPIO_Write(dgt_2_port, numbers_with_dot[displayed_numbers[1]]);

  // třetí digit
  GPIO_Write(dgt_3_port, numbers[displayed_numbers[2]]);

  // čtvrtý digit
  GPIO_Write(dgt_4_port, numbers[displayed_numbers[3]]);
}

/* -------------------------------------------------------------------------------------------*/
void display_num(uint32_t cislo2, uint32_t cislo1, uint32_t sec1, uint32_t milisec1)
{
  // první digit
  GPIO_Write(dgt_1_port, numbers[cislo2]);

  // druhý digit
  GPIO_Write(dgt_2_port, numbers_with_dot[cislo1]);

  // třetí digit
  GPIO_Write(dgt_3_port, numbers[sec1]);

  // čtvrtý digit
  GPIO_Write(dgt_4_port, numbers[milisec1]);
}
/* -------------------------------------------------------------------------------------------*/
void display_min_sec(uint32_t minuty, uint32_t sekundy)
{
  // Převod zadaných čísla na desítky a jednotky
  uint32_t temp1 = ((sekundy / 10) % 10); // tens
  uint32_t temp2 = (sekundy % 10);        // unit

  // první idigt
  GPIO_Write(dgt_1_port, numbers[temp1]);

  // druhý digit
  GPIO_Write(dgt_2_port, numbers_with_dot[temp2]);

  // Převod zadaných čísla na desítky a jednotky
  uint32_t temp3 = ((sekundy / 10) % 10); // tens
  uint32_t temp4 = (sekundy % 10);        // unit
  // třetí digit
  GPIO_Write(dgt_3_port, numbers[temp3]);

  // čtvrtý digit
  GPIO_Write(dgt_4_port, numbers[temp4]);
}
/* -------------------------------------------------------------------------------------------*/
// Vytvoření konstat aby bylo možné zadat display.init() a nemuselo se psát display_init()
const Segment_module display = {
    .init = display_init,
    .write = display_sep,
    .num = display_num,
    .clock = display_min_sec,
};

/*****************************END OF FILE****/
