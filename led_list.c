#include "led_list.h"

/* Statiska funktioner: */
struct led_node* led_node_new(struct led* led);
void led_node_delete(struct led_node** self);

/********************************************************************************
* led_list_init: Initierar angiven länkad lista till tom vid start.
*
*                - self: Pekare till listan som ska initieras.
********************************************************************************/
void led_list_init(struct led_list* self)
{
   self->first = 0;
   self->last = 0;
   self->size = 0;
   return;
}

/********************************************************************************
* led_list_clear: Tömmer och nollställer angiven länkad lista.
*
*                 - self: Pekare till listan som ska tömmas.
********************************************************************************/
void led_list_clear(struct led_list* self)
{
   for (struct led_node* i = self->first; ; i = i->next)
   {
      struct led_node* next = i->next;
      led_node_delete(&i);
      i = next;
   }

   self->first = 0;
   self->last = 0;
   self->size = 0;
   return;
}

/********************************************************************************
* led_list_begin: Returnerar en pekare till den första noden i angiven lista.
*                 Ifall listan är tom returneras null.
*
*                 - self: Pekare till listan.
********************************************************************************/
struct led_node* led_list_begin(const struct led_list* self)
{
   return self->first;
}

/********************************************************************************
* led_list_end: Returnerar en pekare till adressen direkt efter den sista
*               befintliga noden i angiven lista. Ifall listan är tom
*               returneras null.
*
*                - self: Pekare till listan.
********************************************************************************/
struct led_node* led_list_end(const struct led_list* self)
{
   return self->size > 0 ? self->last->next : 0;
}

/********************************************************************************
* led_list_last: Returnerar en pekare till den sista befintliga noden i angiven
*                lista. Ifall listan är tom returneras null.
*
*                - self: Pekare till listan.
********************************************************************************/
struct led_node* led_list_last(const struct led_list* self)
{
   return self->last;
}

/********************************************************************************
* led_list_at: Returnerar en pekare till nod på angivet index. Ifall ett index
*              utanför listans omfång passeras så returneras null.
*
*              - self : Pekare till listan.
*              - index: Index till noden vars adress ska returneras.
********************************************************************************/
struct led_node* led_list_at(const struct led_list* self,
                        const size_t index)
{
   if (index < self->size)
   {
      struct led_node* n = self->first;

      for (size_t i = 0; i < index; ++i)
      {
         n = n->next;
      }

      return n;
   }
   else
   {
      return 0;
   }
}

/********************************************************************************
* led_list_set: Lagrar adressen till angiven lysdiode på angivet index.
*               Ifall ett index utanför listans befintliga omfång passeras så
*               returneras felkod 1, annars returneras 0.
*
*              - self : Pekare till listan.
*              - index: Index där adressen till angiven lysdiod ska lagras.
*              - led  : Pekare (adressen) till lysdioden som ska läggas till.
********************************************************************************/
int led_list_set(struct led_list* self,
                 const size_t index,
                 struct led* led)
{
   if (index < self->size)
   {
      struct led_node* n = led_list_at(self, index);
      n->led = led;
      return 0;
   }
   else
   {
      return 1;
   }
}

/********************************************************************************
* led_list_resize: Ändrar storleken på angiven lista så att den efter
*                  omallokering rymmer angivet antal lysdioder, som kan
*                  tilldelas direkt via index i stället för en push-operation.
*                  Vid misslyckad minnesallokering returneras felkod 1.
*                  Annars om omallokeringen lyckas så returneras 0.
*
*                  - self    : Pekare till listan vars storlek ska ändras.
*                  - new_size: Vektorns nya storlek.
********************************************************************************/
int led_list_resize(struct led_list* self,
                    const size_t new_size)
{
   if (!new_size == 0)
   {
      led_list_clear(self);
      return 0;
   }
   else
   {
      while (self->size > new_size)
      {
         led_list_pop_back(self);
      }
      while (self->size < new_size)
      {
         led_list_push_back(self, 0);
      }

      if (self->size == new_size) return 0;
      else return 1;
   }
}

/********************************************************************************
* led_list_push_front: Lägger till en ny lysdiod längst fram i angiven
*                      lista. Vid misslyckad minnesallokering returneras felkod 1.
*                      Annars om push-operationen lyckas returneras 0.
*
*                      - self   : Pekare till listan som ska tilldelas.
*                      - new_led: Den nya lysdiod som ska läggas till.
********************************************************************************/
int led_list_push_front(struct led_list* self,
                        struct led* new_led)
{
   struct led_node* n1 = led_node_new(new_led);
   struct led_node* n2 = self->first;
   if (!n1) return 1;

   if (self->size++ == 0)
   {
      self->first = n1;
      self->last = n1;
   }
   else
   {
      n1->next = n2;
      n2->previous = n1;
      self->first = n1;
   }

   return 0;
}

/********************************************************************************
* led_list_push_back: Lägger till en ny lysdiod längst bak i angiven lista.
*                     Vid misslyckad minnesallokering returneras felkod 1.
*                     Annars om push-operationen lyckas returneras 0.
*
*                     - self   : Pekare till listan som ska tilldelas.
*                     - new_led: Den nya lysdiod som ska läggas till.
********************************************************************************/
int led_list_push_back(struct led_list* self,
                       struct led* new_led)
{
   struct led_node* n1 = self->last;
   struct led_node* n2 = led_node_new(new_led);
   if (!n2) return 1;

   if (self->size++ == 0)
   {
      self->first = n2;
      self->last = n2;
   }
   else
   {
      n1->next = n2;
      n2->previous = n1;
      self->last = n2;
   }

   return 0;
}

/********************************************************************************
* led_list_pop_front: Tar bort eventuell första lysdiod i angiven lista.
*
*                    - self: Pekare till listan.
********************************************************************************/
void led_list_pop_front(struct led_list* self)
{
   if (self->size <= 1)
   {
      led_list_clear(self);
   }
   else
   {
      struct led_node* n1 = self->first;
      struct led_node* n2 = n1->next;

      n1->next = 0;
      n2->previous = 0;

      led_node_delete(&n1);
      self->first = n2;
      self->size--;
   }

   return;
}

/********************************************************************************
* led_list_pop_back: Tar bort eventuellt sista lysdiod i angiven lista.
*
*                    - self: Pekare till listan.
********************************************************************************/
void led_list_pop_back(struct led_list* self)
{
   if (self->size <= 1)
   {
      led_list_clear(self);
   }
   else
   {
      struct led_node* n2 = self->last;
      struct led_node* n1 = n2->previous;

      n1->next = 0;
      n2->previous = 0;

      led_node_delete(&n2);
      self->last = n1;
      self->size--;
   }

   return;
}

/********************************************************************************
* led_list_insert_at: Lägger in en ny lysdiod på angivet index i en länkad lista
*                     och flyttar bak efterföljande lysdioder ett steg.
*
*                     Ifall angivet index inte ligger inom listans omgång eller
*                     om minnesallokeringen misslyckas så returneras felkod 1,
*                     annars returneras 0.
*
*                     - self : Pekare till listan.
*                     - index: Index där den nya lysdioden ska lagras.
*                     - led  : Pekare till lysdioden som ska lagras.
********************************************************************************/
int led_list_insert_at(struct led_list* self,
                       const size_t index,
                       struct led* led)
{
   if (index < self->size)
   {
      if (index == 0)
      {
         return led_list_push_front(self, led);
      }
      else if (index == self->size - 1)
      {
         return led_list_push_back(self, led);
      }
      else
      {
         struct led_node* n3 = led_list_at(self, index);
         struct led_node* n1 = n3->previous;
         struct led_node* n2 = led_node_new(led);

         if (!n2) return 1;

         n2->next = n3;
         n2->previous = n1;
         n1->next = n2;
         n3->previous = n2;

         self->size++;
         return 0;
      }
   }
   else
   {
      return 1;
   }
}

/********************************************************************************
* led_list_remove_at: Tar bort lysdiod på angivet index i en länkad lista oh
*                     flyttar fram efterföljande lysdioder ett steg.
*
*                     Ifall angivet index inte ligger inom listans omgång så
*                     returneras felkod 1, annars returneras 0.
*
*                     - self : Pekare till listan.
*                     - index: Index till lysdioden som ska tas bort ur listan.
********************************************************************************/
int led_list_remove_at(struct led_list* self,
                       const size_t index)
{
   if (index < self->size)
   {
      if (index == 0)
      {
         led_list_pop_front(self);
         return 0;
      }
      else if (index == self->size - 1)
      {
         led_list_pop_back(self);
         return 0;
      }
      else
      {
         struct led_node* n2 = led_list_at(self, index);
         struct led_node* n1 = n2->previous;
         struct led_node* n3 = n2->next;

         n2->previous = 0;
         n2->next = 0;

         n1->next = n3;
         n3->previous = n1;

         led_node_delete(&n2);
         self->size--;
         return 0;
      }
   }
   else
   {
      return 1;
   }
   
}

/********************************************************************************
* led_list_on: Tänder samtliga lysdioder lagrade i angiven lista.
*
*              - self: Pekare till listan vars lysdioder ska tändas.
********************************************************************************/
void led_list_on(struct led_list* self)
{
   for (struct led_node* i = self->first; i; i = i->next)
   {
      led_on(i->led);
   }
   return;
}

/********************************************************************************
* led_list_off: Släcker samtliga lysdioder lagrade i angiven lista.
*
*               - self: Pekare till listan vars lysdioder ska släckas.
********************************************************************************/
void led_list_off(struct led_list* self)
{
   for (struct led_node* i = self->first; i; i = i->next)
   {
      led_off(i->led);
   }
   return;
}

/********************************************************************************
* led_list_toggle: Togglar samtliga lysdioder lagrade i angiven lista.
*
*                  - self: Pekare till listan vars lysdioder ska togglas.
********************************************************************************/
void led_list_toggle(struct led_list* self)
{
   for (struct led_node* i = self->first; i; i = i->next)
   {
      led_toggle(i->led);
   }
   return;
}

/********************************************************************************
* led_list_blink_collectively: Genomför kollektiv (synkroniserad) blinkning
*                              av samtliga lysdioder lagrade i angiven lista.
*
*                              - self          : Pekare till listan vars
*                                                lysdioder ska blinkas.
*                              - blink_speed_ms: Lysdiodernas blinkhastighet
*                                                mätt i millisekunder.
********************************************************************************/
void led_list_blink_colletively(struct led_list* self,
                                const uint16_t blink_speed_ms)
{
   led_list_on(self);
   delay_ms(blink_speed_ms);
   led_list_off(self);
   delay_ms(blink_speed_ms);
   return;
}

/********************************************************************************
* led_list_blink_forward: Genomför sekventiell blinkning framåt av samtliga
*                         lysdioder lagrade i angiven lista. Därmed blinkar
*                         lysdioderna framåt i en sekvens en efter en.
*
*                         - self          : Pekare till listan vars lysdioder
*                                           ska blinkas.
*                         - blink_speed_ms: Lysdiodernas blinkhastighet
*                                           mätt i millisekunder.
********************************************************************************/
void led_list_blink_forward(struct led_list* self,
                            const uint16_t blink_speed_ms)
{
   led_list_off(self);

   for (struct led_node* i = self->first; i; i = i->next)
   {
      led_on(i->led);
      delay_ms(blink_speed_ms);
      led_off(i->led);
   }

   return;
}

/********************************************************************************
* led_list_blink_backward: Genomför sekventiell blinkning bakåt av samtliga
*                          lysdioder lagrade i angiven lista. Därmed blinkar
*                          lysdioderna bakåt i en sekvens en efter en.
*
*                          - self          : Pekare till listan vars lysdioder
*                                            ska blinkas.
*                          - blink_speed_ms: Lysdiodernas blinkhastighet
*                                            mätt i millisekunder.
********************************************************************************/
void led_list_blink_backward(struct led_list* self,
                             const uint16_t blink_speed_ms)
{
   led_list_off(self);

   for (struct led_node* i = self->last; i; i = i->previous)
   {
      led_on(i->led);
      delay_ms(blink_speed_ms);
      led_off(i->led);
   }

   return;
}


/********************************************************************************
* led_node_new: Allokerar minne för en ny nod, lagrar en referens till angiven
*               lysdiod och returnerar en pekare till noden. Vid misslyckad
*               minnesallokering returneras null.
********************************************************************************/
struct led_node* led_node_new(struct led* led)
{
   struct led_node* self = (struct led_node*)malloc(sizeof(struct led_node));
   if (!self) return 0;
   self->previous = 0;
   self->next = 0;
   self->led = led;
   return self;
}

/********************************************************************************
* led_node_delete: Raderar angiven nod genom att frigöra minnet för denna.
*                  Nodpekaren sätts också till null efter deallokeringen via 
*                  passering av dess adress.
*
*                  - led_node_delete: Adressen till nodpekaren.
********************************************************************************/
void led_node_delete(struct led_node** self)
{
   free(*self);
   *self = 0;
   return;
}