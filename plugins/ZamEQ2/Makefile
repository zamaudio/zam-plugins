#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamEQ2

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ZamEQ2Plugin.cpp.o

OBJS_UI  = \
	ZamEQ2Artwork.cpp.o \
	ZamEQ2UI.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(LINUX),true)
all: jack ladspa dssi lv2_sep vst
else
all: ladspa dssi lv2_sep vst
endif

# --------------------------------------------------------------
