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

FILES_DSP = \
	ZamGatePlugin.cpp

FILES_UI  = \
	ZamGateArtwork.cpp \
	ZamGateUI.cpp

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
TARGETS += clap
TARGETS += au

all: $(TARGETS)

# --------------------------------------------------------------
