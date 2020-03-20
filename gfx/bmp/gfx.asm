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

fonts_til_begin:
.incbin "fonts.pic"
fonts_til_end:

fonts_pal_begin:
.incbin "fonts.pal"
fonts_pal_end:

bkg_sky_16_til_begin:
.incbin "bkg_sky_16.pic"
bkg_sky_16_til_end:

bkg_sky_16_pal_begin:
.incbin "bkg_sky_16.pal"
bkg_sky_16_pal_end:

bkg_sky_16_map_begin:
.incbin "bkg_sky_16.map"
bkg_sky_16_map_end:

bkg_titlescreen_til_begin:
.incbin "bkg_titlescreen.pic"
bkg_titlescreen_til_end:

bkg_titlescreen_pal_begin:
.incbin "bkg_titlescreen.pal"
bkg_titlescreen_pal_end:

bkg_titlescreen_map_begin:
.incbin "bkg_titlescreen.map"
bkg_titlescreen_map_end:

; City level
city_bg0_til_begin:
.incbin "city_bg0.pic"
city_bg0_til_end:

city_bg0_pal_begin:
.incbin "city_bg0.pal"
city_bg0_pal_end:

city_bg1_til_begin:
.incbin "city_bg1.pic"
city_bg1_til_end:

city_bg1_pal_begin:
.incbin "city_bg1.pal"
city_bg1_pal_end:

city_bg1_map_begin:
.incbin "city_bg1.map"
city_bg1_map_end:

city_bg2_til_begin:
.incbin "city_bg2.pic"
city_bg2_til_end:

city_bg2_pal_begin:
.incbin "city_bg2.pal"
city_bg2_pal_end:

city_bg2_map_begin:
.incbin "city_bg2.map"
city_bg2_map_end:

; Egypt level
til_pyramid_til_begin:
.incbin "til_pyramid.pic"
til_pyramid_til_end:

til_pyramid_pal_begin:
.incbin "til_pyramid.pal"
til_pyramid_pal_end:

bkg_egypt_til_begin:
.incbin "bkg_egypt.pic"
bkg_egypt_til_end:

bkg_egypt_pal_begin:
.incbin "bkg_egypt.pal"
bkg_egypt_pal_end:

bkg_egypt_map_begin:
.incbin "bkg_egypt.map"
bkg_egypt_map_end:

.ends

.section ".rodata2" superfree

; Sea level
bkg_sea_til_begin:
.incbin "bkg_sea.pic"
bkg_sea_til_end:

bkg_sea_pal_begin:
.incbin "bkg_sea.pal"
bkg_sea_pal_end:

bkg_sea_map_begin:
.incbin "bkg_sea.map"
bkg_sea_map_end:

bkg_ship_til_begin:
.incbin "bkg_ship.pic"
bkg_ship_til_end:

bkg_ship_pal_begin:
.incbin "bkg_ship.pal"
bkg_ship_pal_end:

bkg_ship_map_begin:
.incbin "bkg_ship.map"
bkg_ship_map_end:

bkg_sea_back_til_begin:
.incbin "bkg_sea_back.pic"
bkg_sea_back_til_end:

bkg_sea_back_pal_begin:
.incbin "bkg_sea_back.pal"
bkg_sea_back_pal_end:

bkg_sea_back_map_begin:
.incbin "bkg_sea_back.map"
bkg_sea_back_map_end:

; Larry's airship
airship_bg0_til_begin:
.incbin "airship_bg0.til"
airship_bg0_til_end:

airship_bg0_pal_begin:
.incbin "airship_bg0.pal"
airship_bg0_pal_end:

airship_bg0_map_begin:
.incbin "airship_bg0.map"
airship_bg0_map_end:

airship_bg1_til_begin:
.incbin "airship_bg1.til"
airship_bg1_til_end:

airship_bg1_pal_begin:
.incbin "airship_bg1.pal"
airship_bg1_pal_end:

airship_bg1_map_begin:
.incbin "airship_bg1.map"
airship_bg1_map_end:

airship_bg2_til_begin:
.incbin "airship_bg2.til"
airship_bg2_til_end:

airship_bg2_pal_begin:
.incbin "airship_bg2.pal"
airship_bg2_pal_end:

airship_bg2_map_begin:
.incbin "airship_bg2.map"
airship_bg2_map_end:

.ends

.section ".rodata3" superfree

; Larry's shop
shop_bg0_til_begin:
.incbin "shop_bg0.til"
shop_bg0_til_end:

shop_bg0_pal_begin:
.incbin "shop_bg0.pal"
shop_bg0_pal_end:

shop_bg1_til_begin:
.incbin "shop_bg1.til"
shop_bg1_til_end:

shop_bg1_pal_begin:
.incbin "shop_bg1.pal"
shop_bg1_pal_end:

shop_bg1_map_begin:
.incbin "shop_bg1.map"
shop_bg1_map_end:

.ends
