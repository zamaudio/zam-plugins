#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamHeadX2

# --------------------------------------------------------------
# Files to build

FILES_DSP += \
	convolution.cpp \
	ZamHeadX2Plugin.cpp

ifneq ($(HAVE_ZITA_CONVOLVER),true)
FILES_DSP += \
	zita-convolver.cpp
endif

FILES_UI  = \
	ZamHeadX2Artwork.cpp \
	ZamHeadX2UI.cpp

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

ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif

ifeq ($(HAVE_DGL),true)
TARGETS += lv2_sep
else
TARGETS += lv2_dsp
endif

TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
