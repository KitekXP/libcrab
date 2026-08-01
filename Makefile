CC          ?= gcc
CFLAGS      += $(OPT_RAW) $(CINC) -fPIC
OPTMISATION ?= high
CINC         = -Iinclude

LD      ?= gcc
LDFLAGS += -shared -fPIC -Wl,-Map=$(OUTDIR)/$(DYNAMIC).map

AR      ?= ar
ARFLAGS  = rcs


DESTDIR ?= /usr

OUTDIR  ?= out
OBJDIR   = $(OUTDIR)/obj

SRC      = $(wildcard src/*.cpp)
OBJECTS  = $(patsubst src/%.cpp, $(OBJDIR)/%.o, $(SRC))

STATIC   = libcrab$(SUFFIX).a
DYNAMIC  = libcrab$(SUFFIX).so

DEBUG    = 1



ifeq ($(DEBUG), 1)
	SUFFIX = -debug
	OPTIMISATION ?= debug
else ifeq ($(RELEASE), 1)
	SUFFIX = -release
else
	SUFFIX =
endif

ifndef OPT_RAW
	ifeq ($(OPTMISATION), size)
		OPT_RAW = -Os
	else ifeq ($(OPTMISATION), high)
		OPT_RAW = -O3
	else ifeq ($(OPTMISATION), mid)
		OPT_RAW = -O2
	else ifeq ($(OPTMISATION), low)
		OPT_RAW = -O1
	else ifeq ($(OPTMISATION), size-mid)
		OPT_RAW = -Oz
	else ifeq ($(OPTMISATION), debug)
		OPT_RAW = -Og
	else
		OPT_RAW =
	endif
endif



build: static dynamic

static:  $(OUTDIR)/$(STATIC)
dynamic: $(OUTDIR)/$(DYNAMIC)

buildObj: $(OBJECTS)

cleanObj:
	rm -rf $(wildcard $(OBJDIR))

clean: cleanObj
	rm -rf $(wildcard $(OUTDIR))


$(OUTDIR)/$(STATIC): $(OBJECTS)
	mkdir -p $(OUTDIR)
	$(AR) $(ARFLAGS) $@ $^

$(OUTDIR)/$(DYNAMIC): $(OBJECTS)
	mkdir -p $(OUTDIR)
	$(CC) $(LDFLAGS) $^ -o $@

out/obj/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


install: build
	sudo mkdir -p $(DESTDIR)/lib $(DESTDIR)/include
	sudo install $(OUTDIR)/$(STATIC) $(DESTDIR)/lib
	sudo install $(OUTDIR)/$(DYNAMIC) $(DESTDIR)/lib
	sudo cp -r include/crab $(DESTDIR)/include/crab