# path to snesdev root directory (for emulators, devkitsnes, libsnes)
export DEVKITSNES := /c/snesdev/

# path to devkitsnes root directory for compiler
export DEVKIT65XX := /c/snesdev/devkitsnes

#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKIT65XX)),)
$(error "Please set DEVKIT65XX in your environment. export DEVKIT65XX=<path to>devkit65XX")
endif

include $(DEVKIT65XX)/snes_rules

#---------------------------------------------------------------------------------
all: gfx

clean:	
	rm -f *.pic *.pal *.map

#---------------------------------------------------------------------------------
spr_sprites.pic: spr_sprites.bmp
	$(GFXCONV) -gs32 -pc16 -po16 -n $<

# Editor sprites 
spr_editor.pic: spr_editor.bmp
	$(GFXCONV) -fbmp -m -n -gs8 -pc16 -po16 -pe0 $< 

bkg_city0.pic: bkg_city0.bmp
	$(GFXCONV) -pr -pc4 -n -gs8 -pe0 -fbmp -m $< 

bkg_sky_4.pic: bkg_sky_4.bmp
	$(GFXCONV) -pr -pc4 -n -gs8 -pe0 -fbmp -m -m32p $< 

bkg_buildings.pic: bkg_buildings.bmp
	$(GFXCONV) -pc4 -n -gs8 -pe0 -po4 -fbmp -m -n -mR! $<

# Titlescreen (BG0 for mode 1)
# 16-colors for the top title part
# 16-colors for the subtitle part
# This command will generate 2 16-colors palettes (p0, p1)
bkg_titlescreen.pic: bkg_titlescreen.bmp
	$(GFXCONV) -fbmp -m -n -gs8 -pc16 -pr -pe0 $< 

# Sky for titlescreen (BG1 for mode 1)
# This command will generate 1 16-colors palette (p2)
bkg_sky_16.pic: bkg_sky_16.bmp
	$(GFXCONV) -fbmp -m -n -gs8 -pc16 -pr -pe2 $< 

gfx:	spr_sprites.pic		\
		spr_editor.pic		\
		bkg_city0.pic		\
		bkg_sky_4.pic		\
		bkg_sky_16.pic		\
		bkg_buildings.pic	\
		bkg_titlescreen.pic
