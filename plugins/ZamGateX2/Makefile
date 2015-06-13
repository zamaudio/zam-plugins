#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamGateX2

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ZamGateX2Plugin.cpp.o

OBJS_UI  = \
	ZamGateX2Artwork.cpp.o \
	ZamGateX2UI.cpp.o

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
