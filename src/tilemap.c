#include <snes.h>
#include "graphics.h"
#include "tilemap.h"

// BG tile map containing the buildings 
u16 tilemap[32][32];

void reset_tilemap()
{
	u8 i, j;

    for (j=0; j<32; j++)
    {
        for (i=0; i<32; i++)
        {
            tilemap[j][i] = 0;
        }
    }
}

void init_tilemap(tilemap_builder_t builder)
{
    reset_tilemap();

    builder(tilemap);

	bgInitMapSet(
		0,
		(u8*)tilemap,
		32*32*2,
		SC_32x32,
		VRAM_ADDR_BG0_MAP
	);
}

u16 * get_tilemap()
{
	return (u16 *)tilemap;
}
