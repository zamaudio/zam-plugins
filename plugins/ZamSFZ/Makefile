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

OBJS_DSP = \
	libsfz/sfz.cpp.o \
	Sfz.cpp.o \
	ZamSFZPlugin.cpp.o

OBJS_UI  = \
	ZamSFZArtwork.cpp.o \
	ZamSFZUI.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.mk

# --------------------------------------------------------------
# Extra flags

BASE_FLAGS += $(shell pkg-config --cflags sndfile rubberband)
LINK_FLAGS += $(shell pkg-config --libs sndfile rubberband)

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(LINUX),true)
all: jack lv2_sep
#all: jack dssi lv2_sep vst
else
all: dssi lv2_sep vst
endif

# --------------------------------------------------------------
