#include <snes.h>
#include "irq.h"

// User defined irq handler.
irq_handler_t irq_handler;

// Clear the i flag to enable irq.
// By default, the pvsneslib init disable irq.
extern void enable_irq();

// Called by the main internal irq handler.
void call_irq_handler()
{
    irq_handler();
}

// irq settings can be reset after setMode, setMode7, pixSetMode.
void set_irq(u8 flags, irq_handler_t handler)
{
    // Set the handler before enabling irq.
    irq_handler = handler;

    // Set the H & V Timer to control H & V irq trigger.
    (*(vuint16*)0x4207) = SCREEN_WIDTH; // HTIME
    (*(vuint16*)0x4209) = SCREEN_HEIGHT; // VTIME

    // REG_NMITIMEN is a write only register.
    // Set the flag along with the other pvsneslib default values.
    // Changing the video mode (setMode, setMode7, pixSetMode) resets REG_NMITIMEN to default.
    REG_NMITIMEN = INT_VBLENABLE | INT_JOYPAD_ENABLE | flags;

    // Clear the i flag to enable irq.
    // By default, the pvsneslib init disable irq.
    enable_irq();
}
