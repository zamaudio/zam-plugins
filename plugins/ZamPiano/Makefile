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

FILES_DSP = \
	instrument.cpp \
	PianoNote.cpp \
	ZamPianoPlugin.cpp

FILES_UI  = \
	ZamPianoArtwork.cpp \
	ZamPianoUI.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS += jack
TARGETS += lv2_sep
TARGETS += vst2
TARGETS += vst3

all: $(TARGETS)

# --------------------------------------------------------------
