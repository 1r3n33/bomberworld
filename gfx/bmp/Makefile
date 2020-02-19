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
	rm -f gfx.obj *.pic *.pal *.map

#---------------------------------------------------------------------------------
# Game sprites (plane, bomb)
spr_game.pic: spr_game.bmp
	$(GFXCONV) -gs16 -pc16 -po16 -fbmp -n $<

# Editor sprites
spr_editor.pic: spr_editor.bmp
	$(GFXCONV) -gs8 -pc16 -po16 -fbmp -n $<

# Fonts sprites
spr_fonts.pic: spr_fonts.bmp
	$(GFXCONV) -gs8 -pc16 -po16 -fbmp -n $<

# City level (mode 0 BGs)
# BG0 used for the building tiles. The tilemap is generated programmaticaly
bkg_buildings.pic: bkg_buildings.bmp
	$(GFXCONV) -gs8 -m -mR! -pc4 -po4 -pe0 -fbmp -n $<
# BG1 used for static background
bkg_city0.pic: bkg_city0.bmp
	$(GFXCONV) -gs8 -m -pc4 -po4 -pe0 -pr -fbmp -n $<
# BG2 used for scrolling background
bkg_sky_4.pic: bkg_sky_4.bmp
	$(GFXCONV) -gs8 -m -pc4 -po4 -pe0 -pr -fbmp -n $<

# Egypt level (mode 0 BGs)
# BG0 used for the pyramid tiles. The tilemap is generated programmaticaly
# This image will generate 2 4-colors palettes
til_pyramid.pic: til_pyramid.bmp
	$(GFXCONV) -gs8 -m -mR! -pc4 -pe0 -pr -fbmp -n $<
# BG1 used for static background
# This image will generate 5 4-colors palettes
bkg_egypt.pic: bkg_egypt.bmp
	$(GFXCONV) -gs8 -m -pc4 -pe0 -pr -fbmp -n $<

# Sea level (mode 0 BGs)
# BG0 used for the ship tiles.
# This image will generate 4 4-colors palettes
bkg_ship.pic: bkg_ship.bmp
	$(GFXCONV) -gs8 -m -pc4 -pe0 -pr -fbmp -n $<

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

gfx:	spr_game.pic		\
		spr_editor.pic		\
		spr_fonts.pic		\
		bkg_buildings.pic	\
		bkg_city0.pic		\
		bkg_sky_4.pic		\
		til_pyramid.pic		\
		bkg_egypt.pic		\
		bkg_ship.pic		\
		bkg_titlescreen.pic	\
		bkg_sky_16.pic
