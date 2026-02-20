DEMO=polygon

LZ4?=0
MOD?=0
SKUNK?=0
LOG?=0

export LOG

OS:=$(shell uname -s)

TJASS= lyxass
RMAC= rmac
RLN= rln

_68KFLAGS=-4 ~oall -i$(BJL_ROOT)
//->TJFLAGS= -w -sh

TJFLAGS+= -D MOD=$(MOD) -sh

%.o	: %.js
	$(TJASS) $(TJFLAGS) $<

ALL: bin/$(DEMO).j64

obl0_50.bin: obl.S video.h
	$(RMAC) $(_68KFLAGS) -fr -D_PAL -D__0 $< -o $@

obl1_50.bin: obl.S video.h
	$(RMAC) $(_68KFLAGS) -fr -D_PAL -D__1 $< -o $@

obl0_60.bin: obl.S video.h
	$(RMAC) $(_68KFLAGS) -fr -D_NTSC -D__0 $< -o $@

obl1_60.bin: obl.S video.h
	$(RMAC) $(_68KFLAGS) -fr -D_NTSC -D__1 $< -o $@

OBL=obl0_50.bin obl1_50.bin obl0_60.bin obl1_60.bin video.h

OBJ_INC = objects/torus.inc
OBJ_INC+= objects/cube.inc
OBJ_INC+= objects/bigball.inc
OBJ_INC+= objects/diamant.inc
OBJ_INC+= objects/prisma.inc
OBJ_INC+= objects/tunnel.inc
OBJ_INC+= objects/ship.inc
OBJ_INC+= plane2.inc
OBJ_INC+= pobjects.inc

SRC_INC= visible.inc project.inc rotate.inc structs.inc
SRC_INC+= gouraud.inc createplane.inc sintab.inc addobj.inc
SRC_INC+= plane_prep.js
# Renderer
# Flat
SRC_INC+= draw2_nog.inc
# Texture
SRC_INC+= draw2_txt.inc
# Gouraud
SRC_INC+= draw2.inc

DEPEND = $(SRC_INC) control.js irq.js globalreg.h engine.h hively_player.bin
DEPEND += $(OBL) $(OBJ_INC) engine.js

$(DEMO).o : $(DEMO).js $(DEPEND)
	$(TJASS)  $(TJFLAGS) $<

$(DEMO).bin : $(DEMO).js $(DEPEND)

%.bin : %.js
	$Q$(TJASS) $(TJFLAGS) -d -o $@ $<

ifeq ($(LZ4),0)
#
# uncompressed ROM
#
ifeq ($(SKUNK),1)
bin/$(DEMO).j64: loader/romloader.bin
	cp $(BJL_ROOT)/bin/fastbt2_nb.bin $@
	cat loader/romloader.bin >> $@
	bzcat $(BJL_ROOT)/bin/allff.bin.bz2 >>$@
ifeq ($(MOD),0)
	truncate -s 1M $@
else
	truncate -s 2M $@
endif

else
bin/$(DEMO).j64: $(DEMO).o
	@cat loader/sbl.XXX >$@
	cat $< >> $@
	bzcat $(BJL_ROOT)/bin/allff.bin.bz2 >> $@
ifeq ($(MOD),0)
	truncate -s 1M $@
else
	truncate -s 2M $@
endif
endif
else
#
# LZ4 compressed ROM
#
ifeq ($(SKUNK),1)
$(DEMO).j64: loader/romloader_lz4.bin
	cp $(BJL_ROOT)/bin/fastbt2_nb.bin $@
	cat loader/romloader_lz4.bin >> $@
	bzcat $(BJL_ROOT)/bin/allff.bin.bz2 >>$@
ifeq ($(MOD),0)
	truncate -s 256K $@
else
	truncate -s 512K $@
endif

else
bin/$(DEMO).j64: loader/sbl_lz4.XXX header.bin
	$Qcp $< $@
	cat header.bin >> $@

ifeq ($(MOD),0)
	truncate -s 256K $@
else
	truncate -s 512K $@
endif
endif
endif

loader/romloader_lz4.bin: loader/romloader.S $(DEMO).bin.lz4
	$(RMAC) -fr -DLZ4=1 -o $@ $<

loader/romloader.bin: loader/romloader.S $(DEMO).bin
	$(RMAC) -fr -DLZ4=0 -o $@ $<

$(DEMO).bin.lz4: $(DEMO).bin
	lz4 -f -9 -l --no-frame-crc $<

header.bin: $(DEMO).bin.lz4

hively_player.bin: hively_player_v21.S
	rmac -4 -fb -u -i$(BJL_ROOT)/68k_inc $<
	rln -z -n -a f1b000 x x -o $@ hively_player_v21.o

include Rules.launch

.ONESHELL:
.PHONY: clean
clean:
	rm -f *.o
	rm -f *.equ
	rm -f *~
	rm -f *.cof *.j64 *.rom *.abs $(DEMO).bin header.bin *.lz4
	rm -f loader/romloader*.bin
