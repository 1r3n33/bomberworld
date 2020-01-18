#include <snes.h>
#include "building.h"
#include "graphics.h"

// BG tile map containing the buildings 
u16 tilemap[32][32];

// Number of building blocks to destroy
u16 building_block_count;

void reset_building()
{
	u8 i, j;

	building_block_count = 0;

    for (j=0; j<32; j++)
    {
        for (i=0; i<32; i++)
        {
            tilemap[j][i] = 0;
        }
    }

    bgInitMapSet(
        0,
        (u8*)tilemap,
        32*32*2,
        SC_32x32,
        VRAM_BG0_MAP
    );
}

void init_building(u8 min, u8 max)
{
	u8 i, j;
	u8 rnd;

	reset_building();

	for (i=0; i<32; i+=4)
    {
		rnd = (rand()%(max-min))+min;

    	for (j=0; j<rnd; j++)
        {
			if (j == 0)
			{
            	tilemap[26-j][i+1] = TIL_BUILDING_FLOOR_SIDE_DOOR;
            	tilemap[26-j][i+2] = TIL_BUILDING_FLOOR_SIDE_DOOR | 0x4000;
				building_block_count += 2;
			}
			else if (j == (rnd-1))
			{
            	tilemap[26-j][i+1] = TIL_BUILDING_CEILING_SIDE_WINDOW_0;
            	tilemap[26-j][i+2] = TIL_BUILDING_CEILING_SIDE_WINDOW_0 | 0x4000;
				building_block_count += 2;
			}
			else
			{
            	tilemap[26-j][i+1] = TIL_BUILDING_BODY_SIDE_WINDOW_0;
            	tilemap[26-j][i+2] = TIL_BUILDING_BODY_SIDE_WINDOW_0 | 0x4000;
				building_block_count += 2;
			}
        }
    }

	bgInitMapSet(
		0,
		(u8*)tilemap,
		32*32*2,
		SC_32x32,
		VRAM_BG0_MAP
	);
}

void copy_building(u16 * editor_tilemap)
{
	u8 i, j;

	building_block_count = 0;

    for (j=0; j<32; j++)
    {
        for (i=0; i<32; i++)
        {
			u16 tile = editor_tilemap[(j*32)+i];
            tilemap[j][i] = tile;
			if (tile > 0)
			{
				building_block_count++;
			}
        }
    }

    bgInitMapSet(
        0,
        (u8*)tilemap,
        32*32*2,
        SC_32x32,
        VRAM_BG0_MAP
    );
}

u16 * get_building_tilemap()
{
	return (u16 *)tilemap;
}

void hit_building()
{
	--building_block_count;
}

u16 is_building_destroyed()
{
	return building_block_count == 0;
}
