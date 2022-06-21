/**
 * @author [Petr Oblouk]
 * @github [https://github.com/peoblouk]
 * @create date 24-05-2022 - 21:54:54
 * @modify date 24-05-2022 - 21:54:54
 * @desc [Segment display]
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __4SEGMENT_H
#define __4SEGMENT_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/

/** @addtogroup HELPERS_Exported_Types
 * @{
 */
typedef struct
{
  void (*init)(void);
  void (*write)(uint32_t);
  void (*num)(uint32_t, uint32_t, uint32_t, uint32_t);

} Segment_module;
/**
 * @}
 */

/* Exported constants --------------------------------------------------------*/

/** @addtogroup HELPERS_Exported_Constants
 * @{
 */

/**
 * @}
 */

/* Exported macros -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/** @addtogroup HELPERS_Private_Macros
 * @{
 */

/**
 * @}
 */

/* Exported functions ------------------------------------------------------- */

/** @addtogroup HELPERS_Exported_Functions
 * @{
 */
void display_init(void);
void display_sep(uint32_t display);
void display_num(uint32_t cislo2, uint32_t cislo1, uint32_t sec1, uint32_t milisec1);
extern const Segment_module display;

/**
 * @}
 */

#endif /* __STM8S_4SEGMENT_H */
