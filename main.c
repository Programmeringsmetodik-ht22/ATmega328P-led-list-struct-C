/********************************************************************************
* main.c: Demonstration av dubbellänkad lista för lagring och styrning av 
*         multipla lysdioder.
********************************************************************************/
#include "led.h"
#include "button.h"
#include "led_list.h"

/********************************************************************************
* num_buttons_pressed: Returnerar antalet nedtryckta tryckknappar.
*
*                      - button1: Pekare till den första tryckknappen.
*                      - button2: Pekare till den andra tryckknappen.
*                      - button3: Pekare till den tredje tryckknappen.
*                      - button4: Pekare till den fjärde tryckknappen.
********************************************************************************/
static uint8_t num_buttons_pressed(struct button* button1,
                                   struct button* button2,
                                   struct button* button3,
                                   struct button* button4)
{
   uint8_t num = 0;
   if (button_is_pressed(button1)) num++;
   if (button_is_pressed(button2)) num++;
   if (button_is_pressed(button3)) num++;
   if (button_is_pressed(button4)) num++;
   return num;
}

/********************************************************************************
* main: Ansluter fem lysdioder till pin 6 - 10 samt fyra tryckknappar till pin
*       11 - 13 samt pin 2. Lysdioderna lagras i en dynamisk array.
*       Beroende på antalet tryckknappar som trycks ned så blinkar lysdioderna
*       antingen framåt, bakåt eller synkroniserat, eller så hålls de tända
*       eller släckta.
********************************************************************************/
int main(void)
{ 
   struct led l1, l2, l3, l4, l5;
   struct button b1, b2, b3, b4;
   struct led_list leds;

   led_init(&l1, 6);
   led_init(&l2, 7);
   led_init(&l3, 8);
   led_init(&l4, 9);
   led_init(&l5, 10);

   button_init(&b1, 11);
   button_init(&b2, 12);
   button_init(&b3, 13);
   button_init(&b4, 2);

   led_list_init(&leds);

   led_list_push_back(&leds, &l1);
   led_list_push_back(&leds, &l2);
   led_list_push_back(&leds, &l3);
   led_list_push_back(&leds, &l4);
   led_list_push_back(&leds, &l5);

   while (1)
   {
      const uint8_t buttons_pressed = num_buttons_pressed(&b1, &b2, &b3, &b4);

      if (buttons_pressed == 0)
      {
         led_list_off(&leds);
      }
      else if (buttons_pressed == 1)
      {
         led_list_blink_colletively(&leds, 100);
      }
      else if (buttons_pressed == 2)
      {
         led_list_blink_forward(&leds, 100);
      }
      else if (buttons_pressed == 3)
      {
         led_list_blink_backward(&leds, 100);
      }
      else if (buttons_pressed == 4)
      {
         led_list_on(&leds);
      }
      else
      {
         led_list_off(&leds);
      }
   }
  
   return 0;
}

