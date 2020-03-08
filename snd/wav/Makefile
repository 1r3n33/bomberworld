#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKIT65XX)),)
$(error "Please set DEVKIT65XX in your environment. export DEVKIT65XX=<path to>devkit65XX")
endif

include $(DEVKIT65XX)/snes_rules

#---------------------------------------------------------------------------------
WAVFILES :=	$(wildcard *.wav)
BRRFILES :=	$(WAVFILES:.wav=.brr)

#---------------------------------------------------------------------------------
all: $(BRRFILES) snd.obj

clean:
	rm -f *.brr *.obj
