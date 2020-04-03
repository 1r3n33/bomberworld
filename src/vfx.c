#include <snes.h>
#include "vfx.h"

struct hdma_buffer_t
{
    u8 data[1024];
};

struct hdma_buffer_t hdma_buffer;

// HDMA unit is 1 byte header + 2 bytes value. REG_BG2HOFS is a double-write register.
#define VFX_BKG_WAVES_UNIT_SIZE     3
#define VFX_BKG_WAVES_TOTAL_SIZE    ((SCREEN_HEIGHT+32)*VFX_BKG_WAVES_UNIT_SIZE)

// Use channel 1. Channel 0 is used for pvsneslib DMA
#define VFX_BKG_WAVES_ENABLE_HDMA   (1<<1)

#define VFX_BKG_WAVES_REG_DMAP  REG_DMAP1
#define VFX_BKG_WAVES_REG_BBAD  REG_BBAD1
#define VFX_BKG_WAVES_REG_A1TLH REG_A1T1LH
#define VFX_BKG_WAVES_REG_A1B   REG_A1B1

u8 vfx_bkg_waves_offset = 0;

void init_vfx_bkg_waves()
{
    // bkg_waves:
    // Use HDMA to set H_SCROLL value for each line.
    //
    // Init:
    //
    // 0 |####      | H_SCROLL value for line 0 starts at 0.
    // 1 | ####     | Increment H_SCROLL to form the wave...
    // 2 |  ####    |
    // 3 |   ####   |
    // 4 |  ####    | Then decrement H_SCROLL...
    // 5 | ####     |
    // 6 |####      | Repeat this pattern until line 224.
    //
    // Update:
    //
    // Just offset the buffer start by VFX_BKG_WAVES_UNIT_SIZE each frame.
    // It's OK to HDMA less lines as the bottom of the background is plain color.

    u16 i = 0;
    for (i=0; i<VFX_BKG_WAVES_TOTAL_SIZE; i+=18)
    {
        hdma_buffer.data[i+0]  = 1; // header byte: 1 line
        hdma_buffer.data[i+1]  = 0; // value for the H_SCROLL register (8-bit low)
        hdma_buffer.data[i+2]  = 0; // value for the H_SCROLL register (8-bit high)

        hdma_buffer.data[i+3]  = 1;
        hdma_buffer.data[i+4]  = 1;
        hdma_buffer.data[i+5]  = 0;

        hdma_buffer.data[i+6]  = 1;
        hdma_buffer.data[i+7]  = 2;
        hdma_buffer.data[i+8]  = 0;

        hdma_buffer.data[i+9]  = 1;
        hdma_buffer.data[i+10] = 3;
        hdma_buffer.data[i+11] = 0;

        hdma_buffer.data[i+12] = 1;
        hdma_buffer.data[i+13] = 2;
        hdma_buffer.data[i+14] = 0;

        hdma_buffer.data[i+15] = 1;
        hdma_buffer.data[i+16] = 1;
        hdma_buffer.data[i+17] = 0;
    }
    hdma_buffer.data[i] = 0; // header byte: terminate hdma

    // Disable all HDMA.
    // As seen in the example code. Not sure if it is really needed.
    REG_HDMAEN = 0;

    VFX_BKG_WAVES_REG_DMAP  = 2; // Mode 2: REG_BG2HOFS is a double-write register.
    VFX_BKG_WAVES_REG_BBAD  = 0x0F; // REG_BG2HOFS (8-bit low)
    VFX_BKG_WAVES_REG_A1TLH = (u16)(hdma_buffer.data); // Table address.
    VFX_BKG_WAVES_REG_A1B   = 0x7E; // Table address is in .bss section.

    REG_HDMAEN = VFX_BKG_WAVES_ENABLE_HDMA;
}

void update_vfx_bkg_waves(u8 frame)
{
    if ((frame & 7)==7) // Animate every 7 frames.
    {
        vfx_bkg_waves_offset = (++vfx_bkg_waves_offset) % 6; // Select the wave line. (0-5)
    }

    u8 offset = vfx_bkg_waves_offset * VFX_BKG_WAVES_UNIT_SIZE; // Compute the hdma_buffer offset.

    VFX_BKG_WAVES_REG_A1TLH = (u16)(hdma_buffer.data+offset); // Table address.
}

void init_vfx_moon_bkg()
{
    u16 neg = 0;
    u16 i = 0;
    for (i=0; i<VFX_BKG_WAVES_TOTAL_SIZE; i+=(16*3))
    {
        hdma_buffer.data[i+0]  = 1; // header byte: 1 line
        hdma_buffer.data[i+1]  = 0; // value for the H_SCROLL register (8-bit low)
        hdma_buffer.data[i+2]  = 0; // value for the H_SCROLL register (8-bit high)

        hdma_buffer.data[i+3]  = 1;
        hdma_buffer.data[i+4]  = neg ? 255 : 1;
        hdma_buffer.data[i+5]  = 0;

        hdma_buffer.data[i+6]  = 1;
        hdma_buffer.data[i+7]  = neg ? 254 : 2;
        hdma_buffer.data[i+8]  = 0;

        hdma_buffer.data[i+9]  = 1;
        hdma_buffer.data[i+10] = neg ? 254 : 2;
        hdma_buffer.data[i+11] = 0;

        hdma_buffer.data[i+12] = 1;
        hdma_buffer.data[i+13] = neg ? 253 : 3;
        hdma_buffer.data[i+14] = 0;

        hdma_buffer.data[i+15] = 1;
        hdma_buffer.data[i+16] = neg ? 253 : 3;
        hdma_buffer.data[i+17] = 0;

        hdma_buffer.data[i+18] = 1;
        hdma_buffer.data[i+19] = neg ? 252 : 4;
        hdma_buffer.data[i+20] = 0;

        hdma_buffer.data[i+21] = 1;
        hdma_buffer.data[i+22] = neg ? 252 : 4;
        hdma_buffer.data[i+23] = 0;

        hdma_buffer.data[i+24] = 1;
        hdma_buffer.data[i+25] = neg ? 252 : 4;
        hdma_buffer.data[i+26] = 0;

        hdma_buffer.data[i+27] = 1;
        hdma_buffer.data[i+28] = neg ? 252 : 4;
        hdma_buffer.data[i+29] = 0;

        hdma_buffer.data[i+30] = 1;
        hdma_buffer.data[i+31] = neg ? 252 : 4;
        hdma_buffer.data[i+32] = 0;

        hdma_buffer.data[i+33] = 1;
        hdma_buffer.data[i+34] = neg ? 253 : 3;
        hdma_buffer.data[i+35] = 0;

        hdma_buffer.data[i+36] = 1;
        hdma_buffer.data[i+37] = neg ? 253 : 3;
        hdma_buffer.data[i+38] = 0;

        hdma_buffer.data[i+39] = 1;
        hdma_buffer.data[i+40] = neg ? 254 : 2;
        hdma_buffer.data[i+41] = 0;

        hdma_buffer.data[i+42] = 1;
        hdma_buffer.data[i+43] = neg ? 254 : 2;
        hdma_buffer.data[i+44] = 0;

        hdma_buffer.data[i+45] = 1;
        hdma_buffer.data[i+46] = neg ? 255 : 1;
        hdma_buffer.data[i+47] = 0;

        neg = !neg;
    }
    hdma_buffer.data[i] = 0; // header byte: terminate hdma

    // Disable all HDMA.
    // As seen in the example code. Not sure if it is really needed.
    REG_HDMAEN = 0;

    VFX_BKG_WAVES_REG_DMAP  = 2; // Mode 2: REG_BG2HOFS is a double-write register.
    VFX_BKG_WAVES_REG_BBAD  = 0x0F; // REG_BG2HOFS (8-bit low)
    VFX_BKG_WAVES_REG_A1TLH = (u16)(hdma_buffer.data); // Table address.
    VFX_BKG_WAVES_REG_A1B   = 0x7E; // Table address is in .bss section.

    REG_HDMAEN = VFX_BKG_WAVES_ENABLE_HDMA;
}

void update_vfx_moon_bkg(u8 frame)
{
    if ((frame & 3)==3) // Animate every 3 frames.
    {
        vfx_bkg_waves_offset = (++vfx_bkg_waves_offset) % 31; // Select the wave line. (0-30)
    }

    u8 offset = vfx_bkg_waves_offset * VFX_BKG_WAVES_UNIT_SIZE; // Compute the hdma_buffer offset.

    VFX_BKG_WAVES_REG_A1TLH = (u16)(hdma_buffer.data+offset); // Table address.
}
