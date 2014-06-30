#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamSynth

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ZamSynthPlugin.cpp.o

OBJS_UI  = \
	ZamSynthArtwork.cpp.o \
	ZamSynthUI.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(LINUX),true)
all: jack dssi lv2_sep vst
else
all: dssi lv2_sep vst
endif

# --------------------------------------------------------------
