#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamChild670

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ZamChild670Plugin.cpp.o \
	sidechainamplifier.cpp.o \
	tubemodel.cpp.o \
	variablemuamplifier.cpp.o \
	wdfcircuits.cpp.o \
	wavechild670.cpp.o

OBJS_UI  = \
	ZamChild670Artwork.cpp.o \
	ZamChild670UI.cpp.o

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
