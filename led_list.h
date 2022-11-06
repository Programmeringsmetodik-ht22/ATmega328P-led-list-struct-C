/********************************************************************************
* led_list.h: Inneh�ller funktionalitet f�r implementering av dubbell�nkade
*             listor f�r lagring och styrning �ver multipla lysdioder eller
*             andra utportar, realiserat via struktar led_list och led_node
*             samt associerade funktioner.
********************************************************************************/
#ifndef LED_LIST_H_
#define LED_LIST_H_

/* Inkluderingsdirektiv: */
#include "misc.h"
#include "led.h"

/********************************************************************************
* led_node: Nod f�r lagring av en lysdiod i en dubbell�nkad lista, med pekare
*           till f�reg�ende samt n�sta nod i listan.
********************************************************************************/
struct led_node
{
   struct led_node* previous; /* Pekare till f�reg�ende nod. */
   struct led_node* next;     /* Pekare till n�sta nod. */
   struct led* led;           /* Pekare till lagrad lysdiod. */
};

/********************************************************************************
* led_list: Dubbell�nkad lista f�r lagring och styrning av lysdiod eller andra 
*           digitala utportar, implementerade via strukten led.
********************************************************************************/
struct led_list
{
   struct led_node* first; /* Pekare till f�rsta lysdioden i listan. */
   struct led_node* last;  /* Pekare till sista lysdioden i listan. */
   size_t size;            /* Listans storlek, dvs. antalet lagrade lysdioder. */
};

/********************************************************************************
* led_list_init: Initierar angiven l�nkad lista till tom vid start.
*
*                - self: Pekare till listan som ska initieras.
********************************************************************************/
void led_list_init(struct led_list* self);

/********************************************************************************
* led_list_clear: T�mmer och nollst�ller angiven l�nkad lista.
*
*                 - self: Pekare till listan som ska t�mmas.
********************************************************************************/
void led_list_clear(struct led_list* self);

/********************************************************************************
* led_list_begin: Returnerar en pekare till den f�rsta noden i angiven lista.
*                 Ifall listan �r tom returneras null.
*
*                 - self: Pekare till listan.
********************************************************************************/
struct led_node* led_list_begin(const struct led_list* self);

/********************************************************************************
* led_list_end: Returnerar en pekare till adressen direkt efter den sista
*               befintliga noden i angiven lista. Ifall listan �r tom
*               returneras null.
*
*                - self: Pekare till listan.
********************************************************************************/
struct led_node* led_list_end(const struct led_list* self);

/********************************************************************************
* led_list_last: Returnerar en pekare till den sista befintliga noden i angiven
*                lista. Ifall listan �r tom returneras null.
*
*                - self: Pekare till listan.
********************************************************************************/
struct led_node* led_list_last(const struct led_list* self);

/********************************************************************************
* led_list_at: Returnerar en pekare till nod p� angivet index. Ifall ett index
*              utanf�r listans omf�ng passeras s� returneras null.
*
*              - self : Pekare till listan.
*              - index: Index till noden vars adress ska returneras.
********************************************************************************/
struct led_node* led_list_at(const struct led_list* self, 
                        const size_t index);

/********************************************************************************
* led_list_set: Lagrar adressen till angiven lysdiode p� angivet index.
*               Ifall ett index utanf�r listans befintliga omf�ng passeras s�
*               returneras felkod 1, annars returneras 0.
*
*              - self : Pekare till listan.
*              - index: Index d�r adressen till angiven lysdiod ska lagras.
*              - led  : Pekare (adressen) till lysdioden som ska l�ggas till.
********************************************************************************/
int led_list_set(struct led_list* self,
                 const size_t index,
                 struct led* led);

/********************************************************************************
* led_list_resize: �ndrar storleken p� angiven lista s� att den efter 
*                  omallokering rymmer angivet antal lysdioder, som kan
*                  tilldelas direkt via index i st�llet f�r en push-operation.
*                  Vid misslyckad minnesallokering returneras felkod 1.
*                  Annars om omallokeringen lyckas s� returneras 0.
*
*                  - self    : Pekare till listan vars storlek ska �ndras.
*                  - new_size: Listans nya storlek.
********************************************************************************/
int led_list_resize(struct led_list* self,
                    const size_t new_size);

/********************************************************************************
* led_list_push_front: L�gger till en ny lysdiod l�ngst fram i angiven
*                      lista. Vid misslyckad minnesallokering returneras felkod 1.
*                      Annars om push-operationen lyckas returneras 0.
*
*                      - self   : Pekare till listan som ska tilldelas.
*                      - new_led: Den nya lysdiod som ska l�ggas till.
********************************************************************************/
int led_list_push_front(struct led_list* self,
                        struct led* new_led);

/********************************************************************************
* led_list_push_back: L�gger till en ny lysdiod  l�ngst bak i angiven lista.
*                     Vid misslyckad minnesallokering returneras felkod 1.
*                     Annars om push-operationen lyckas returneras 0.
*
*                     - self   : Pekare till listan som ska tilldelas.
*                     - new_led: Den nya lysdiod som ska l�ggas till.
********************************************************************************/
int led_list_push_back(struct led_list* self,
                       struct led* new_led);

/********************************************************************************
* led_list_pop_front: Tar bort eventuellt f�rsta lysdiod i angiven lista.
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
* led_list_insert_at: L�gger in en ny lysdiod p� angivet index i en l�nkad lista
*                     och flyttar bak efterf�ljande lysdioder ett steg. 
*
*                     Ifall angivet index inte ligger inom listans omg�ng eller 
*                     om minnesallokeringen misslyckas s� returneras felkod 1, 
*                     annars returneras 0.
*
*                     - self : Pekare till listan.
*                     - index: Index d�r den nya lysdioden ska lagras.
*                     - led  : Pekare till lysdioden som ska lagras.
********************************************************************************/
int led_list_insert_at(struct led_list* self,
                       const size_t index,
                       struct led* led);

/********************************************************************************
* led_list_remove_at: Tar bort lysdiod p� angivet index i en l�nkad lista oh
*                     flyttar fram efterf�ljande lysdioder ett steg.
*
*                     Ifall angivet index inte ligger inom listans omg�ng s� 
*                     returneras felkod 1, annars returneras 0.
*
*                     - self : Pekare till listan.
*                     - index: Index till lysdioden som ska tas bort ur listan.
********************************************************************************/
int led_list_remove_at(struct led_list* self,
                       const size_t index);

/********************************************************************************
* led_list_on: T�nder samtliga lysdioder lagrade i angiven lista.
*
*              - self: Pekare till listan vars lysdioder ska t�ndas.
********************************************************************************/
void led_list_on(struct led_list* self);


/********************************************************************************
* led_list_off: Sl�cker samtliga lysdioder lagrade i angiven lista.
*
*               - self: Pekare till listan vars lysdioder ska sl�ckas.
********************************************************************************/
void led_list_off(struct led_list* self);

/********************************************************************************
* led_list_toggle: Togglar samtliga lysdioder lagrade i angiven lista.
*
*                  - self: Pekare till listan vars lysdioder ska togglas.
********************************************************************************/
void led_list_toggle(struct led_list* self);

/********************************************************************************
* led_list_blink_collectively: Genomf�r kollektiv (synkroniserad) blinkning
*                              av samtliga lysdioder lagrade i angiven lista.
*
*                              - self          : Pekare till listan vars
*                                                lysdioder ska blinkas.
*                              - blink_speed_ms: Lysdiodernas blinkhastighet
*                                                m�tt i millisekunder.
********************************************************************************/
void led_list_blink_colletively(struct led_list* self,
                                const uint16_t blink_speed_ms);

/********************************************************************************
* led_list_blink_forward: Genomf�r sekventiell blinkning fram�t av samtliga
*                         lysdioder lagrade i angiven lista. D�rmed blinkar
*                         lysdioderna fram�t i en sekvens en efter en.
*
*                         - self          : Pekare till listan vars lysdioder
*                                           ska blinkas.
*                         - blink_speed_ms: Lysdiodernas blinkhastighet
*                                           m�tt i millisekunder.
********************************************************************************/
void led_list_blink_forward(struct led_list* self,
                            const uint16_t blink_speed_ms);

/********************************************************************************
* led_list_blink_backward: Genomf�r sekventiell blinkning bak�t av samtliga
*                          lysdioder lagrade i angiven lista. D�rmed blinkar
*                          lysdioderna bak�t i en sekvens en efter en.
*
*                          - self          : Pekare till listan vars lysdioder
*                                            ska blinkas.
*                          - blink_speed_ms: Lysdiodernas blinkhastighet
*                                            m�tt i millisekunder.
********************************************************************************/
void led_list_blink_backward(struct led_list* self,
                             const uint16_t blink_speed_ms);

#endif /* LED_LIST_H_ */