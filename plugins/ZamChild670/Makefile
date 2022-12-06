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

FILES_DSP = \
	ZamChild670Plugin.cpp \
	sidechainamplifier.cpp \
	tubemodel.cpp \
	variablemuamplifier.cpp \
	wdfcircuits.cpp \
	wavechild670.cpp

FILES_UI  = \
	ZamChild670Artwork.cpp \
	ZamChild670UI.cpp

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
