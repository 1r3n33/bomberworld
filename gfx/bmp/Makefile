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

bkg_city0.pic: bkg_city0.bmp
	$(GFXCONV) -gs8 -m -pc4 -po4 -pe0 -pr -fbmp -n $< 

bkg_sky_4.pic: bkg_sky_4.bmp
	$(GFXCONV) -gs8 -m -pc4 -po4 -pe0 -pr -fbmp -n $< 

bkg_buildings.pic: bkg_buildings.bmp
	$(GFXCONV) -gs8 -m -mR! -pc4 -po4 -pe0 -fbmp -n $<

# Titlescreen (BG0 for mode 1)
# 16-colors for the top title part
# 16-colors for the subtitle part
# This command will generate 2 16-colors palettes
# Use palette entries 1 and 2 to avoid collision with the BG2 4-colors palette
bkg_titlescreen.pic: bkg_titlescreen.bmp
	$(GFXCONV) -gs8 -m -pc16 -po32 -pe1 -pr -fbmp -n $< 

# Sky for titlescreen (BG1 for mode 1)
# This command will generate 1 16-colors palette
# Use palette entry 3
bkg_sky_16.pic: bkg_sky_16.bmp
	$(GFXCONV) -gs8 -m -pc16 -po16 -pe3 -pr -fbmp -n $< 

gfx:	spr_game.pic		\
		spr_editor.pic		\
		spr_fonts.pic		\
		bkg_city0.pic		\
		bkg_sky_4.pic		\
		bkg_sky_16.pic		\
		bkg_buildings.pic	\
		bkg_titlescreen.pic
