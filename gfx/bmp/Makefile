#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKIT65XX)),)
$(error "Please set DEVKIT65XX in your environment. export DEVKIT65XX=<path to>devkit65XX")
endif

include $(DEVKIT65XX)/snes_rules

#---------------------------------------------------------------------------------
all: gfx gfx.obj

clean:
	rm -f gfx.obj *.pic *.pal *.map *.til *.gfx

#---------------------------------------------------------------------------------
# Game sprites (plane, bomb)
spr_game.pic: spr_game.bmp
	$(GFXCONV) -gs16 -pc16 -po16 -fbmp -n $<

# Editor sprites
spr_editor.pic: spr_editor.bmp
	$(GFXCONV) -gs8 -pc16 -po16 -fbmp -n $<

# Fonts sprites
fonts.gfx: spr_fonts.bmp
	$(GFXCONV) -gs8 -pc16 -po16 -fbmp -n $<
	mv spr_fonts.pic fonts.pic
	mv spr_fonts.pal fonts.pal
	touch $@

# City level (mode 0 BGs)
# BG0 used for the building tiles. The tilemap is generated programmaticaly
city_bg0.gfx: bkg_buildings.bmp
	$(GFXCONV) -gs8 -m -mR! -pc4 -po4 -pe0 -fbmp -n $<
	mv bkg_buildings.pic city_bg0.pic
	mv bkg_buildings.pal city_bg0.pal
	touch $@
# BG1 used for static background
city_bg1.gfx: bkg_city0.bmp
	$(GFXCONV) -gs8 -m -pc4 -po4 -pe0 -pr -fbmp -n $<
	mv bkg_city0.pic city_bg1.pic
	mv bkg_city0.pal city_bg1.pal
	mv bkg_city0.map city_bg1.map
	touch $@
# BG2 used for scrolling background
city_bg2.gfx: bkg_sky_4.bmp
	$(GFXCONV) -gs8 -m -pc4 -po4 -pe0 -pr -fbmp -n $<
	mv bkg_sky_4.pic city_bg2.pic
	mv bkg_sky_4.pal city_bg2.pal
	mv bkg_sky_4.map city_bg2.map
	touch $@

# Egypt level (mode 0 BGs)
# BG0 used for the pyramid tiles. The tilemap is generated programmaticaly
# This image will generate 2 4-colors palettes
egypt_bg0.gfx: til_pyramid.bmp
	$(GFXCONV) -gs8 -m -mR! -pc4 -pe0 -pr -fbmp -n $<
	mv til_pyramid.pic egypt_bg0.pic
	mv til_pyramid.pal egypt_bg0.pal
	touch $@
# BG1 used for static background
# This image will generate 5 4-colors palettes
egypt_bg1.gfx: bkg_egypt.bmp
	$(GFXCONV) -gs8 -m -pc4 -pe0 -pr -fbmp -n $<
	mv bkg_egypt.pic egypt_bg1.pic
	mv bkg_egypt.pal egypt_bg1.pal
	mv bkg_egypt.map egypt_bg1.map
	touch $@

# Sea level (mode 0 BGs)
# BG0 used for the front sea waves.
# This image will generate 2 4-colors palettes
sea_bg0.gfx: bkg_sea.bmp
	$(GFXCONV) -gs8 -m -pc4 -pe0 -pr -fbmp -n $<
	mv bkg_sea.pic sea_bg0.pic
	mv bkg_sea.pal sea_bg0.pal
	mv bkg_sea.map sea_bg0.map
	touch $@
# BG1 used for the ship tiles.
# This image will generate 4 4-colors palettes
sea_bg1.gfx: bkg_ship.bmp
	$(GFXCONV) -gs8 -m -pc4 -pe0 -pr -fbmp -n $<
	mv bkg_ship.pic sea_bg1.pic
	mv bkg_ship.pal sea_bg1.pal
	mv bkg_ship.map sea_bg1.map
	touch $@
# BG2 used for the back sea waves.
# This image will generate 2 4-colors palettes
sea_bg2.gfx: bkg_sea_back.bmp
	$(GFXCONV) -gs8 -m -pc4 -pe0 -pr -fbmp -n $<
	mv bkg_sea_back.pic sea_bg2.pic
	mv bkg_sea_back.pal sea_bg2.pal
	mv bkg_sea_back.map sea_bg2.map
	touch $@

# Moon level (mode 0 BGs)
# BG0 used for the front.
# This image will generate 1 4-colors palettes
moon_bg0.gfx: bkg_moon_front.bmp
	$(GFXCONV) -gs8 -m -pc4 -po4 -pe0 -pr -fbmp -n $<
	mv bkg_moon_front.pic moon_bg0.pic
	mv bkg_moon_front.pal moon_bg0.pal
	mv bkg_moon_front.map moon_bg0.map
	touch $@
# BG1 used for the tentacles.
# This image will generate 1 4-colors palettes
moon_bg1.gfx: til_tentacles.bmp
	$(GFXCONV) -gs8 -m -mR! -pc4 -po4 -pe0 -pr -fbmp -n $<
	mv til_tentacles.pic moon_bg1.pic
	mv til_tentacles.pal moon_bg1.pal
	touch $@
# BG2 used for the back.
# This image will generate 1 4-colors palettes
moon_bg2.gfx: bkg_moon_back.bmp
	$(GFXCONV) -gs8 -m -pc4 -po4 -pe0 -pr -fbmp -n $<
	mv bkg_moon_back.pic moon_bg2.pic
	mv bkg_moon_back.pal moon_bg2.pal
	mv bkg_moon_back.map moon_bg2.map
	touch $@

# Larry's airship (Mode 1)
# Palette 0 & 1 are reserved to the 4-colors BG2
# Palette 2 is reserved for the 16-colors BG0 (larry's airship)
# Palette 3 is reserved for the 16-colors BG1 
# This image will generate 1 16-colors palette
airship_bg0.gfx: bkg_airship.bmp
	$(GFXCONV) -gs8 -m -mp -pc16 -po16 -pe2 -pr -fbmp -n $<
	mv bkg_airship.pic airship_bg0.til
	mv bkg_airship.pal airship_bg0.pal
	mv bkg_airship.map airship_bg0.map
	touch $@
# This image will generate 1 16-colors palette
airship_city_bg1.gfx: bkg_city0.bmp
	$(GFXCONV) -gs8 -m -pc16 -po16 -pe3 -pr -fbmp -n $<
	mv bkg_city0.pic airship_city_bg1.til
	mv bkg_city0.pal airship_city_bg1.pal
	mv bkg_city0.map airship_city_bg1.map
	touch $@
# This image will generate 1 4-colors palette
# TODO: this is exactely the same setup as in the city level
airship_city_bg2.gfx: bkg_sky_4.bmp
	$(GFXCONV) -gs8 -m -pc4 -po4 -pe0 -pr -fbmp -n $<
	mv bkg_sky_4.pic airship_city_bg2.til
	mv bkg_sky_4.pal airship_city_bg2.pal
	mv bkg_sky_4.map airship_city_bg2.map
	touch $@
# This image will generate 1 16-colors palette
airship_egypt_bg1.gfx: bkg_egypt.bmp
	$(GFXCONV) -gs8 -m -pc16 -po16 -pe3 -pr -fbmp -n $<
	mv bkg_egypt.pic airship_egypt_bg1.til
	mv bkg_egypt.pal airship_egypt_bg1.pal
	mv bkg_egypt.map airship_egypt_bg1.map
	touch $@
# This image will generate 1 16-colors palette
airship_sea_bg1.gfx: bkg_sea.bmp
	$(GFXCONV) -gs8 -m -pc16 -po16 -pe3 -pr -fbmp -n $<
	mv bkg_sea.pic airship_sea_bg1.til
	mv bkg_sea.pal airship_sea_bg1.pal
	mv bkg_sea.map airship_sea_bg1.map
	touch $@
# This image will generate 2 4-colors palette
# TODO: this is exactely the same setup as in the city level
airship_sea_bg2.gfx: bkg_sea_back.bmp
	$(GFXCONV) -gs8 -m -pc4 -pe0 -pr -fbmp -n $<
	mv bkg_sea_back.pic airship_sea_bg2.til
	mv bkg_sea_back.pal airship_sea_bg2.pal
	mv bkg_sea_back.map airship_sea_bg2.map
	touch $@

# Larry's shop (Mode 1)
# This image will generate 1 16-colors palette
shop_bg0.gfx: til_shop.bmp
	$(GFXCONV) -gs8 -pc16 -po16 -pe0 -pr -fbmp -n $<
	mv til_shop.pic shop_bg0.til
	mv til_shop.pal shop_bg0.pal
	touch $@
# This image will generate 1 16-colors palette
shop_bg1.gfx: bkg_larry.bmp
	$(GFXCONV) -gs8 -m -pc16 -po16 -pe1 -pr -fbmp -n $<
	mv bkg_larry.pic shop_bg1.til
	mv bkg_larry.pal shop_bg1.pal
	mv bkg_larry.map shop_bg1.map
	touch $@

# Titlescreen (BG0 for mode 1)
# 16-colors for the top title part
# 16-colors for the subtitle part
# This image will generate 2 16-colors palettes
# Use palette entries 1 and 2 to avoid collision with the BG2 4-colors palette
bkg_titlescreen.pic: bkg_titlescreen.bmp
	$(GFXCONV) -gs8 -m -pc16 -po32 -pe1 -pr -fbmp -n $<
# Sky for titlescreen (BG1 for mode 1)
# This image will generate 1 16-colors palette
# Use palette entry 3
bkg_sky_16.pic: bkg_sky_16.bmp
	$(GFXCONV) -gs8 -m -pc16 -po16 -pe3 -pr -fbmp -n $<

gfx:	spr_game.pic			\
		spr_editor.pic			\
		fonts.gfx				\
		city_bg0.gfx			\
		city_bg1.gfx			\
		city_bg2.gfx			\
		egypt_bg0.gfx			\
		egypt_bg1.gfx			\
		sea_bg0.gfx				\
		sea_bg1.gfx				\
		sea_bg2.gfx				\
		moon_bg0.gfx			\
		moon_bg1.gfx			\
		moon_bg2.gfx			\
		airship_bg0.gfx			\
		airship_city_bg1.gfx	\
		airship_city_bg2.gfx	\
		airship_egypt_bg1.gfx	\
		airship_sea_bg1.gfx		\
		airship_sea_bg2.gfx		\
		shop_bg0.gfx			\
		shop_bg1.gfx			\
		bkg_titlescreen.pic		\
		bkg_sky_16.pic
