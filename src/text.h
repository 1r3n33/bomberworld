#ifndef TEXT_H
#define TEXT_H

#include <snes.h>

void reset_text();

u16 set_text(u16 id, u8 * text, u8 x, u8 y);

u16 set_text_number(u16 id, u16 number, u8 x, u8 y);

#endif // TEXT_H
