#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamVerb

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	ZamVerbPlugin.cpp \
	ZamVerbImpulses.cpp \
	convolution.cpp

ifneq ($(HAVE_ZITA_CONVOLVER),true)
FILES_DSP += \
	zita-convolver.cpp
endif

FILES_UI  = \
	ZamVerbArtwork.cpp \
	ZamVerbUI.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk

ifeq ($(HAVE_ZITA_CONVOLVER),true)
BASE_FLAGS += -DHAVE_ZITA_CONVOLVER
LINK_FLAGS += -lzita-convolver
else
include ../zita-convolver.mk
endif

# --------------------------------------------------------------
# Extra flags

ifeq ($(LINUX),true)
BASE_FLAGS += $(shell pkg-config --cflags samplerate fftw3f)
LINK_FLAGS += $(shell pkg-config --libs fftw3f samplerate)
else
BASE_FLAGS += $(shell pkg-config --static --cflags samplerate fftw3f)
LINK_FLAGS += $(shell pkg-config --static --libs fftw3f samplerate)
endif

LINK_FLAGS += -lpthread

# --------------------------------------------------------------

TARGETS += jack
TARGETS += lv2_sep
TARGETS += vst2
TARGETS += vst3

all: $(TARGETS)

# --------------------------------------------------------------
