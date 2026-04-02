#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ZamEcho

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	ZamEchoPlugin.cpp

FILES_UI  = \
	ZamEchoArtwork.cpp \
	ZamEchoUI.cpp

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
