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

ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif

ifeq ($(UNIX),true)
TARGETS += ladspa
endif

ifeq ($(HAVE_DGL),true)
TARGETS += lv2_sep
else
TARGETS += lv2_dsp
endif

TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
