#ifndef __4SEGMENT_H
#define __4SEGMENT_H

#include "stm8s.h"

typedef struct // Nový struct
{
  void (*init)(void);
  void (*write)(uint32_t);
  void (*num)(uint32_t, uint32_t, uint32_t, uint32_t);
  void (*clock)(uint32_t, uint32_t);
} Segment_module;

// Inicializace funkcí jak mají vypadat
void display_init(void);
void display_sep(uint32_t display);
void display_num(uint32_t cislo2, uint32_t cislo1, uint32_t sec1, uint32_t milisec1);
void display_min_sec(uint32_t minuty, uint32_t sekundy);
extern const Segment_module display;

#endif /* __STM8S_4SEGMENT_H */
