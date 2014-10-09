#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamNoise

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ZamNoisePlugin.cpp.o \
	Denoise.cpp.o 

OBJS_UI  = \
	ZamNoiseArtwork.cpp.o \
	ZamNoiseUI.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(LINUX),true)
all: jack ladspa dssi lv2_sep vst
else
all: ladspa vst
endif

# --------------------------------------------------------------
