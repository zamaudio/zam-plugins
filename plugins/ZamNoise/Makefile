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

BASE_FLAGS += $(shell pkg-config --cflags fftw3f)
LINK_FLAGS += $(shell pkg-config --libs fftw3f)

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS += jack
TARGETS += ladspa
TARGETS += lv2_sep
TARGETS += vst2
TARGETS += vst3
TARGETS += clap

all: $(TARGETS)

# --------------------------------------------------------------
