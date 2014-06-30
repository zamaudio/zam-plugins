#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamAutoSat

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ZamAutoSatPlugin.cpp.o

OBJS_UI  = \
	ZamAutoSatArtwork.cpp.o \
	ZamAutoSatUI.cpp.o

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
