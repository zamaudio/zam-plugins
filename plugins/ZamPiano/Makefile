#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamPiano

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	instrument.cpp.o \
	ZamPianoPlugin.cpp.o

OBJS_UI  = \
	ZamPianoArtwork.cpp.o \
	ZamPianoUI.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(LINUX),true)
all: jack lv2_sep
#all: jack dssi lv2_sep vst
else
all: dssi lv2_sep vst
endif

# --------------------------------------------------------------
