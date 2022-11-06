/********************************************************************************
* button.h: Innehåller funktionsdefinitioner för enkel implementering av 
*           tryckknappar samt andra digitala inportar via strukten button.
********************************************************************************/
#include "button.h"

/********************************************************************************
* button_init: Initierar ny tryckknapp på angiven pin.
*
*              - self: Pekare till tryckknappen som ska initieras.
*              - pin : Tryckknappens pin-nummer på Arduino Uno, exempelvis 13.
*                      Alternativt kan motsvarande port-nummer på ATmega328P
*                      anges, exempelvis B5 för pin 13 eller D3 för pin 3.
********************************************************************************/
void button_init(struct button* self,
                 const uint8_t pin)
{
   if (pin >= 0 && pin <= 7)
   {
      self->io_port = IO_PORTD;
      self->pin = pin;
      PORTD |= (1 << self->pin);
   }
   else if (pin >= 8 && pin <= 13)
   {
      self->io_port = IO_PORTB;
      self->pin = pin - 8;
      PORTB |= (1 << self->pin);
   }
   else if (pin >= 14 && pin <= 19)
   {
      self->io_port = IO_PORTC;
      self->pin = pin - 14;
      PORTC |= (1 << self->pin);
   }
   else
   {
      self->io_port = IO_PORT_NONE;
      self->pin = 0;
   }

   self->interrupt_enabled = false;
   return;
}

/********************************************************************************
* button_clear: Nollställer tryckknapp samt motsvarande pin.
*
*               - self: Pekare till tryckknappen som ska nollställas.
********************************************************************************/
void button_clear(struct button* self)
{
   button_disable_interrupt(self);

   if (self->io_port == IO_PORTB)
   {
      PORTB &= ~(1 << self->pin);
   }
   else if (self->io_port == IO_PORTC)
   {
      PORTC &= ~(1 << self->pin);
   }
   else if (self->io_port == IO_PORTD)
   {
      PORTD &= ~(1 << self->pin);
   }

   self->io_port = IO_PORT_NONE;
   self->pin = 0;
   return;
}

/********************************************************************************
* button_is_pressed: Läser av tryckknappens pin och indikerar ifall denna är
*                    nedtryckt. I så fall returneras true, annars false.
*
*                    - self: Pekare till tryckknappen som ska läsas av.
********************************************************************************/
bool button_is_pressed(const struct button* self)
{
   if (self->io_port == IO_PORTB)
   {
      return (PINB & (1 << self->pin));
   }
   else if (self->io_port == IO_PORTC)
   {
      return (PINC & (1 << self->pin));
   }
   else if (self->io_port == IO_PORTD)
   {
      return (PIND & (1 << self->pin));
   }
   else
   {
      return false;
   }
}

/********************************************************************************
* button_enable_interrupt: Aktiverar PCI-avbrott på angiven tryckknapp så att
*                          event på tryckknappens pin medför avbrott, både på
*                          stigande och fallande flank. Kontroll av flank måste
*                          skötas av användaren, förslagsvis via avläsning i
*                          motsvarande avbrottsrutin.
*
*                          Nedan visas sambandet mellan använd I/O-port samt
*                          avbrottsvektorn för motsvarande avbrottsrutin:
*
*                          I/O-port     pin (Arduino Uno)     Avbrottsvektor
*                             B              8 - 13             PCINT0_vect
*                             C             A0 - A5             PCINT1_vect
*                             D              0 - 7              PCINT2_vect
*
*                          - self: Pekare till tryckknappen som PCI-avbrott
*                                  ska aktiveras på.
********************************************************************************/
void button_enable_interrupt(struct button* self)
{
   asm("SEI");

   if (self->io_port == IO_PORTB)
   {
      PCICR |= (1 << PCIE0);
      PCMSK0 |= (1 << self->pin);
   }
   else if (self->io_port == IO_PORTC)
   {
      PCICR |= (1 << PCIE1);
      PCMSK1 |= (1 << self->pin);
   }
   else if (self->io_port == IO_PORTD)
   {
      PCICR |= (1 << PCIE2);
      PCMSK2 |= (1 << self->pin);
   }

   self->interrupt_enabled = true;
   return;
}

/********************************************************************************
* button_disable_interrupt: Inaktiverar PCI-avbrott på angiven tryckknapp.
*
*                           - self: Pekare till tryckknappen som PCI-avbrott
*                                  ska inaktiveras på.
********************************************************************************/
void button_disable_interrupt(struct button* self)
{
   if (self->io_port == IO_PORTB)
   {
      PCMSK0 &= ~(1 << self->pin);
   }
   else if (self->io_port == IO_PORTC)
   {
      PCMSK1 &= ~(1 << self->pin);
   }
   else if (self->io_port == IO_PORTD)
   {
      PCMSK2 &= ~(1 << self->pin);
   }

   self->interrupt_enabled = false;
   return;
}

/********************************************************************************
* button_togglar_interrupt: Togglar aktivering av PCI-avbrott på angiven
*                           tryckknapp. Om avbrott är aktiverat vid anrop
*                           så sker inaktivering. På samma sätt gäller att om
*                           avbrott är inaktiverat vid anrop så sker aktivering.
*
*                           - self: Pekare till tryckknappen som aktivering av
*                                   PCI-avbrott ska inaktiveras på.
********************************************************************************/
void button_toggle_interrupt(struct button* self)
{
   if (self->interrupt_enabled)
   {
      button_disable_interrupt(self);
   }
   else
   {
      button_enable_interrupt(self);
   }

   return;
}