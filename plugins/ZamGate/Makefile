#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamGate

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ZamGatePlugin.cpp.o

OBJS_UI  = \
	ZamGateArtwork.cpp.o \
	ZamGateUI.cpp.o

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
