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

OBJS_DSP = \
	ZamVerbPlugin.cpp.o \
	ZamVerbImpulses.cpp.o \
	convolution.cpp.o \
	../../lib/zita-convolver-4.0.0/zita-convolver.cpp.o

OBJS_UI  = \
	ZamVerbArtwork.cpp.o \
	ZamVerbUI.cpp.o

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
