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
TARGET		:=	bw
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

GFXDIR := ./gfx/bmp/
OBJDIR := ./src/
BINDIR := ./bin/

export OUTPUT:=	$(BINDIR)/$(TARGET)
 
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.asm)))
 
#---------------------------------------------------------------------------------
export OFILES	:=	$(wildcard $(OBJDIR)/*.obj) $(wildcard $(GFXDIR)/*.obj)
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

GTITLE 		:= -ht"$(TARGET)"
 
#---------------------------------------------------------------------------------
all:
	$(MAKE) -C $(GFXDIR)
	$(MAKE) -C $(OBJDIR)
	$(MAKE) $(OUTPUT).sfc
	$(SNTOOLS) -hi! $(GTITLE) $(OUTPUT).sfc

clean:
	$(MAKE) clean -C $(GFXDIR)
	$(MAKE) clean -C $(OBJDIR)
	rm -f $(OUTPUT).sfc $(OUTPUT).sym

#---------------------------------------------------------------------------------
$(OUTPUT).sfc: $(OFILES)
