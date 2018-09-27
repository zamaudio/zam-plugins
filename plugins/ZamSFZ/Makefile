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

BASE_FLAGS += $(shell pkg-config --cflags sndfile rubberband)
LINK_FLAGS += $(shell pkg-config --libs sndfile rubberband)

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif

ifeq ($(LINUX),true)
TARGETS += dssi_dsp
ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_LIBLO),true)
TARGETS += dssi_ui
endif
endif
endif

ifeq ($(HAVE_DGL),true)
TARGETS += lv2_sep
else
TARGETS += lv2_dsp
endif

TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
