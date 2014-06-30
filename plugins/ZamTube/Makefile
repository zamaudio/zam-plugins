#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamTube

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ZamTubePlugin.cpp.o \
	wdf.cpp.o

OBJS_UI  = \
	ZamTubeArtwork.cpp.o \
	ZamTubeUI.cpp.o

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
