#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKIT65XX)),)
$(error "Please set DEVKIT65XX in your environment. export DEVKIT65XX=<path to>devkit65XX")
endif

include $(DEVKIT65XX)/snes_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
#---------------------------------------------------------------------------------
TARGET		:=	bw

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
SRCDIR := ./src/
OBJDIR := $(SRCDIR) $(shell ls -d $(SRCDIR)/*/) $(GFXDIR)
BINDIR := ./bin/

export OUTPUT:=	$(BINDIR)/$(TARGET)
  
#---------------------------------------------------------------------------------
export OFILES	:=	$(foreach dir,$(OBJDIR),$(wildcard $(dir)/*.obj))
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

GTITLE 		:= -ht"$(TARGET)"
 
#---------------------------------------------------------------------------------
all:
	$(MAKE) -C $(GFXDIR)
	$(MAKE) -C $(SRCDIR)
	mkdir -p $(BINDIR)
	$(MAKE) $(OUTPUT).sfc
	$(SNTOOLS) -hi! $(GTITLE) $(OUTPUT).sfc

clean:
	$(MAKE) clean -C $(GFXDIR)
	$(MAKE) clean -C $(SRCDIR)
	rm -f $(OUTPUT).sfc $(OUTPUT).sym

#---------------------------------------------------------------------------------
$(OUTPUT).sfc: $(OFILES)
