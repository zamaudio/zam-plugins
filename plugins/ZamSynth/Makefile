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

FILES_DSP = \
	ZamSynthPlugin.cpp

FILES_UI  = \
	ZamSynthArtwork.cpp \
	ZamSynthUI.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS += jack
TARGETS += lv2_sep
TARGETS += vst2
TARGETS += vst3
TARGETS += au

all: $(TARGETS)

# --------------------------------------------------------------
