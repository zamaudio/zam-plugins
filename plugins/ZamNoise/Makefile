#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamNoise

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	ZamNoisePlugin.cpp \
	Denoise.cpp

FILES_UI  = \
	ZamNoiseArtwork.cpp \
	ZamNoiseUI.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS += jack
TARGETS += ladspa
TARGETS += lv2_sep
TARGETS += vst2
TARGETS += vst3

all: $(TARGETS)

# --------------------------------------------------------------
