#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZaMultiCompX2

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ZaMultiCompX2Plugin.cpp.o

OBJS_UI  = \
	ZaMultiCompX2Artwork.cpp.o \
	ZaMultiCompX2UI.cpp.o

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
