.include "hdr.asm"

.section ".rodata1" superfree

spr_game_til_begin:
.incbin "spr_game.pic"
spr_game_til_end:

spr_game_pal_begin:
.incbin "spr_game.pal"
spr_game_pal_end:

spr_editor_til_begin:
.incbin "spr_editor.pic"
spr_editor_til_end:

spr_editor_pal_begin:
.incbin "spr_editor.pal"
spr_editor_pal_end:

spr_fonts_til_begin:
.incbin "spr_fonts.pic"
spr_fonts_til_end:

spr_fonts_pal_begin:
.incbin "spr_fonts.pal"
spr_fonts_pal_end:

bkg_city0_til_begin:
.incbin "bkg_city0.pic"
bkg_city0_til_end:

bkg_city0_pal_begin:
.incbin "bkg_city0.pal"
bkg_city0_pal_end:

bkg_city0_map_begin:
.incbin "bkg_city0.map"
bkg_city0_map_end:

bkg_sky_4_til_begin:
.incbin "bkg_sky_4.pic"
bkg_sky_4_til_end:

bkg_sky_4_pal_begin:
.incbin "bkg_sky_4.pal"
bkg_sky_4_pal_end:

bkg_sky_4_map_begin:
.incbin "bkg_sky_4.map"
bkg_sky_4_map_end:

bkg_sky_16_til_begin:
.incbin "bkg_sky_16.pic"
bkg_sky_16_til_end:

bkg_sky_16_pal_begin:
.incbin "bkg_sky_16.pal"
bkg_sky_16_pal_end:

bkg_sky_16_map_begin:
.incbin "bkg_sky_16.map"
bkg_sky_16_map_end:

bkg_buildings_til_begin:
.incbin "bkg_buildings.pic"
bkg_buildings_til_end:

bkg_buildings_pal_begin:
.incbin "bkg_buildings.pal"
bkg_buildings_pal_end:

bkg_titlescreen_til_begin:
.incbin "bkg_titlescreen.pic"
bkg_titlescreen_til_end:

bkg_titlescreen_pal_begin:
.incbin "bkg_titlescreen.pal"
bkg_titlescreen_pal_end:

bkg_titlescreen_map_begin:
.incbin "bkg_titlescreen.map"
bkg_titlescreen_map_end:

.ends