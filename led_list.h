/********************************************************************************
* led_list.h: Innehåller funktionalitet för implementering av dubbellänkade
*             listor för lagring och styrning över multipla lysdioder eller
*             andra utportar, realiserat via struktar led_list och led_node
*             samt associerade funktioner.
********************************************************************************/
#ifndef LED_LIST_H_
#define LED_LIST_H_

/* Inkluderingsdirektiv: */
#include "misc.h"
#include "led.h"

/********************************************************************************
* led_node: Nod för lagring av en lysdiod i en dubbellänkad lista, med pekare
*           till föregående samt nästa nod i listan.
********************************************************************************/
struct led_node
{
   struct led_node* previous; /* Pekare till föregående nod. */
   struct led_node* next;     /* Pekare till nästa nod. */
   struct led* led;           /* Pekare till lagrad lysdiod. */
};

/********************************************************************************
* led_list: Dubbellänkad lista för lagring och styrning av lysdiod eller andra 
*           digitala utportar, implementerade via strukten led.
********************************************************************************/
struct led_list
{
   struct led_node* first; /* Pekare till första lysdioden i listan. */
   struct led_node* last;  /* Pekare till sista lysdioden i listan. */
   size_t size;            /* Listans storlek, dvs. antalet lagrade lysdioder. */
};

/********************************************************************************
* led_list_init: Initierar angiven länkad lista till tom vid start.
*
*                - self: Pekare till listan som ska initieras.
********************************************************************************/
void led_list_init(struct led_list* self);

/********************************************************************************
* led_list_clear: Tömmer och nollställer angiven länkad lista.
*
*                 - self: Pekare till listan som ska tömmas.
********************************************************************************/
void led_list_clear(struct led_list* self);

/********************************************************************************
* led_list_begin: Returnerar en pekare till den första noden i angiven lista.
*                 Ifall listan är tom returneras null.
*
*                 - self: Pekare till listan.
********************************************************************************/
struct led_node* led_list_begin(const struct led_list* self);

/********************************************************************************
* led_list_end: Returnerar en pekare till adressen direkt efter den sista
*               befintliga noden i angiven lista. Ifall listan är tom
*               returneras null.
*
*                - self: Pekare till listan.
********************************************************************************/
struct led_node* led_list_end(const struct led_list* self);

/********************************************************************************
* led_list_last: Returnerar en pekare till den sista befintliga noden i angiven
*                lista. Ifall listan är tom returneras null.
*
*                - self: Pekare till listan.
********************************************************************************/
struct led_node* led_list_last(const struct led_list* self);

/********************************************************************************
* led_list_at: Returnerar en pekare till nod på angivet index. Ifall ett index
*              utanför listans omfång passeras så returneras null.
*
*              - self : Pekare till listan.
*              - index: Index till noden vars adress ska returneras.
********************************************************************************/
struct led_node* led_list_at(const struct led_list* self, 
                        const size_t index);

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
                 struct led* led);

/********************************************************************************
* led_list_resize: Ändrar storleken på angiven lista så att den efter 
*                  omallokering rymmer angivet antal lysdioder, som kan
*                  tilldelas direkt via index i stället för en push-operation.
*                  Vid misslyckad minnesallokering returneras felkod 1.
*                  Annars om omallokeringen lyckas så returneras 0.
*
*                  - self    : Pekare till listan vars storlek ska ändras.
*                  - new_size: Listans nya storlek.
********************************************************************************/
int led_list_resize(struct led_list* self,
                    const size_t new_size);

/********************************************************************************
* led_list_push_front: Lägger till en ny lysdiod längst fram i angiven
*                      lista. Vid misslyckad minnesallokering returneras felkod 1.
*                      Annars om push-operationen lyckas returneras 0.
*
*                      - self   : Pekare till listan som ska tilldelas.
*                      - new_led: Den nya lysdiod som ska läggas till.
********************************************************************************/
int led_list_push_front(struct led_list* self,
                        struct led* new_led);

/********************************************************************************
* led_list_push_back: Lägger till en ny lysdiod  längst bak i angiven lista.
*                     Vid misslyckad minnesallokering returneras felkod 1.
*                     Annars om push-operationen lyckas returneras 0.
*
*                     - self   : Pekare till listan som ska tilldelas.
*                     - new_led: Den nya lysdiod som ska läggas till.
********************************************************************************/
int led_list_push_back(struct led_list* self,
                       struct led* new_led);

/********************************************************************************
* led_list_pop_front: Tar bort eventuellt första lysdiod i angiven lista.
*
*                     - self: Pekare till listan.
********************************************************************************/
void led_list_pop_front(struct led_list* self);

/********************************************************************************
* led_list_pop_back: Tar bort eventuellt sista lysdiod i angiven lista. 
*
*                    - self: Pekare till listan.
********************************************************************************/
void led_list_pop_back(struct led_list* self);

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
                       struct led* led);

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
                       const size_t index);

/********************************************************************************
* led_list_on: Tänder samtliga lysdioder lagrade i angiven lista.
*
*              - self: Pekare till listan vars lysdioder ska tändas.
********************************************************************************/
void led_list_on(struct led_list* self);


/********************************************************************************
* led_list_off: Släcker samtliga lysdioder lagrade i angiven lista.
*
*               - self: Pekare till listan vars lysdioder ska släckas.
********************************************************************************/
void led_list_off(struct led_list* self);

/********************************************************************************
* led_list_toggle: Togglar samtliga lysdioder lagrade i angiven lista.
*
*                  - self: Pekare till listan vars lysdioder ska togglas.
********************************************************************************/
void led_list_toggle(struct led_list* self);

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
                                const uint16_t blink_speed_ms);

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
                            const uint16_t blink_speed_ms);

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
                             const uint16_t blink_speed_ms);

#endif /* LED_LIST_H_ */