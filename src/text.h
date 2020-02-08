#ifndef TEXT_H
#define TEXT_H

#include <snes.h>

void reset_text();

void set_text(char * text, u8 x, u8 y);

void set_text_number(u16 id, u16 number, u8 x, u8 y);

#endif // TEXT_H
