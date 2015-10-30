#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamChild670

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ZamChild670Plugin.cpp.o \
	sidechainamplifier.cpp.o \
	tubemodel.cpp.o \
	variablemuamplifier.cpp.o \
	wdfcircuits.cpp.o \
	wavechild670.cpp.o

OBJS_UI  = \
	ZamChild670Artwork.cpp.o \
	ZamChild670UI.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif

ifeq ($(LINUX),true)
TARGETS += ladspa
ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_LIBLO),true)
TARGETS += dssi
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
