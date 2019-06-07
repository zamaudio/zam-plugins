#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZaMultiCompX2

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	ZaMultiCompX2Plugin.cpp

FILES_UI  = \
	ZaMultiCompX2Artwork.cpp \
	ZaMultiCompX2UI.cpp

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
