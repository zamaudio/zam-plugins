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

OBJS_DSP = \
	../../lib/zita-convolver-4.0.0/zita-convolver.cpp.o \
	convolution.cpp.o \
	ZamHeadX2Plugin.cpp.o

OBJS_UI  = \
	ZamHeadX2Artwork.cpp.o \
	ZamHeadX2UI.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.mk

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
