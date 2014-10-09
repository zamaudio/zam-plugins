#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZaMultiComp

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ZaMultiCompPlugin.cpp.o

OBJS_UI  = \
	ZaMultiCompArtwork.cpp.o \
	ZaMultiCompUI.cpp.o

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
