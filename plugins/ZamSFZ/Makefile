#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamSFZ

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	libsfz/sfz.cpp \
	Sfz.cpp \
	ZamSFZPlugin.cpp

FILES_UI  = \
	ZamSFZArtwork.cpp \
	ZamSFZUI.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Extra flags

BASE_FLAGS += $(shell pkg-config --cflags sndfile rubberband samplerate)
LINK_FLAGS += $(shell pkg-config --libs sndfile rubberband samplerate)

# --------------------------------------------------------------
# Enable all possible plugin types

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
