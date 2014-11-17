#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamGEQ31X2

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ZamGEQ31X2Plugin.cpp.o

OBJS_UI  = \
	ZamGEQ31X2Artwork.cpp.o \
	ZamGEQ31X2UI.cpp.o

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
