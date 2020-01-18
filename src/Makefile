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
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(shell basename $(CURDIR))
SOURCES		:=	.

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS	+=	$(INCLUDE) 

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(PVSNESLIB)
 
export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.asm)))
 
#---------------------------------------------------------------------------------
export OFILES	:=	$(BINFILES:.bin=.obj) $(CFILES:.c=.obj) $(SFILES:.asm=.obj)
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

GTITLE 		:= -ht"$(TARGET)"
 
.PHONY: bitmaps all
 
#---------------------------------------------------------------------------------
all	:	bitmaps $(OUTPUT).sfc
	$(SNTOOLS) -hi! $(GTITLE) $(TARGET).sfc

clean:
	@echo clean...
	@rm -f $(OFILES) $(TARGET).sfc *.pic *.pal *.map *sym

#---------------------------------------------------------------------------------
spr_sprites.pic: spr_sprites.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -gs32 -pc16 -po16 -n $<

# Editor sprites 
spr_editor.pic: spr_editor.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -fbmp -m -n -gs8 -pc16 -po16 -pe0 $< 

bkg_city0.pic: bkg_city0.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc4 -n -gs8 -pe0 -fbmp -m $< 
#	$(GFXCONV) -pc4 -n -gs8 -pe0 -po4 -fbmp -m -n $<

bkg_sky_4.pic: bkg_sky_4.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc4 -n -gs8 -pe0 -fbmp -m -m32p $< 

bkg_buildings.pic: bkg_buildings.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pc4 -n -gs8 -pe0 -po4 -fbmp -m -n -mR! $<

# Titlescreen (BG0 for mode 1)
# 16-colors for the top title part
# 16-colors for the subtitle part
# This command will generate 2 16-colors palettes (p0, p1)
bkg_titlescreen.pic: bkg_titlescreen.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -fbmp -m -n -gs8 -pc16 -pr -pe0 $< 

# Sky for titlescreen (BG1 for mode 1)
# This command will generate 1 16-colors palette (p2)
bkg_sky_16.pic: bkg_sky_16.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -fbmp -m -n -gs8 -pc16 -pr -pe2 $< 

bitmaps:	spr_sprites.pic		\
			spr_editor.pic		\
			bkg_city0.pic		\
			bkg_sky_4.pic		\
			bkg_sky_16.pic		\
			bkg_buildings.pic	\
			bkg_titlescreen.pic

#---------------------------------------------------------------------------------
$(OUTPUT).sfc	: $(OFILES)
