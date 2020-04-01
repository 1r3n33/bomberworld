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
egypt_bg0_til_begin:
.incbin "egypt_bg0.pic"
egypt_bg0_til_end:

egypt_bg0_pal_begin:
.incbin "egypt_bg0.pal"
egypt_bg0_pal_end:

egypt_bg1_til_begin:
.incbin "egypt_bg1.pic"
egypt_bg1_til_end:

egypt_bg1_pal_begin:
.incbin "egypt_bg1.pal"
egypt_bg1_pal_end:

egypt_bg1_map_begin:
.incbin "egypt_bg1.map"
egypt_bg1_map_end:

.ends

.section ".rodata2" superfree

; Sea level
sea_bg0_til_begin:
.incbin "sea_bg0.pic"
sea_bg0_til_end:

sea_bg0_pal_begin:
.incbin "sea_bg0.pal"
sea_bg0_pal_end:

sea_bg0_map_begin:
.incbin "sea_bg0.map"
sea_bg0_map_end:

sea_bg1_til_begin:
.incbin "sea_bg1.pic"
sea_bg1_til_end:

sea_bg1_pal_begin:
.incbin "sea_bg1.pal"
sea_bg1_pal_end:

sea_bg1_map_begin:
.incbin "sea_bg1.map"
sea_bg1_map_end:

sea_bg2_til_begin:
.incbin "sea_bg2.pic"
sea_bg2_til_end:

sea_bg2_pal_begin:
.incbin "sea_bg2.pal"
sea_bg2_pal_end:

sea_bg2_map_begin:
.incbin "sea_bg2.map"
sea_bg2_map_end:

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

; Larry's airship - City
airship_city_bg1_til_begin:
.incbin "airship_city_bg1.til"
airship_city_bg1_til_end:

airship_city_bg1_pal_begin:
.incbin "airship_city_bg1.pal"
airship_city_bg1_pal_end:

airship_city_bg1_map_begin:
.incbin "airship_city_bg1.map"
airship_city_bg1_map_end:

airship_city_bg2_til_begin:
.incbin "airship_city_bg2.til"
airship_city_bg2_til_end:

airship_city_bg2_pal_begin:
.incbin "airship_city_bg2.pal"
airship_city_bg2_pal_end:

airship_city_bg2_map_begin:
.incbin "airship_city_bg2.map"
airship_city_bg2_map_end:

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

; Larry's airship - Egypt
airship_egypt_bg1_til_begin:
.incbin "airship_egypt_bg1.til"
airship_egypt_bg1_til_end:

airship_egypt_bg1_pal_begin:
.incbin "airship_egypt_bg1.pal"
airship_egypt_bg1_pal_end:

airship_egypt_bg1_map_begin:
.incbin "airship_egypt_bg1.map"
airship_egypt_bg1_map_end:

airship_sea_bg1_til_begin:
.incbin "airship_sea_bg1.til"
airship_sea_bg1_til_end:

airship_sea_bg1_pal_begin:
.incbin "airship_sea_bg1.pal"
airship_sea_bg1_pal_end:

; Larry's airship - Sea
airship_sea_bg1_map_begin:
.incbin "airship_sea_bg1.map"
airship_sea_bg1_map_end:

airship_sea_bg2_til_begin:
.incbin "airship_sea_bg2.til"
airship_sea_bg2_til_end:

airship_sea_bg2_pal_begin:
.incbin "airship_sea_bg2.pal"
airship_sea_bg2_pal_end:

airship_sea_bg2_map_begin:
.incbin "airship_sea_bg2.map"
airship_sea_bg2_map_end:

; Moon
moon_bg0_til_begin:
.incbin "moon_bg0.pic"
moon_bg0_til_end:

moon_bg0_pal_begin:
.incbin "moon_bg0.pal"
moon_bg0_pal_end:

moon_bg0_map_begin:
.incbin "moon_bg0.map"
moon_bg0_map_end:

moon_bg2_til_begin:
.incbin "moon_bg2.pic"
moon_bg2_til_end:

moon_bg2_pal_begin:
.incbin "moon_bg2.pal"
moon_bg2_pal_end:

moon_bg2_map_begin:
.incbin "moon_bg2.map"
moon_bg2_map_end:

.ends
