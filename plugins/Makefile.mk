#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

# NAME, OBJS_DSP and OBJS_UI have been defined before

include ../../Makefile.mk

# --------------------------------------------------------------
# Basic setup

TARGET_DIR = ../../bin

BUILD_C_FLAGS   += -I.
BUILD_CXX_FLAGS += -I. -I../../libs/distrho -I../../libs/dgl

# --------------------------------------------------------------
# Enable all plugin types

all: ladspa lv2 dssi vst
#all: lv2

# --------------------------------------------------------------
# Set plugin binary file targets

ladspa_dsp = $(TARGET_DIR)/$(NAME)-ladspa.$(EXT)
dssi_dsp   = $(TARGET_DIR)/$(NAME)-dssi.$(EXT)
dssi_ui    = $(TARGET_DIR)/$(NAME)-dssi/$(NAME)_ui
lv2_dsp    = $(TARGET_DIR)/$(NAME).lv2/$(NAME).$(EXT)
lv2_ui     = $(TARGET_DIR)/$(NAME).lv2/$(NAME)_ui.$(EXT)
vst        = $(TARGET_DIR)/$(NAME)-vst.$(EXT)

ifeq ($(WIN32),true)
dssi_ui += .exe
endif

# TODO: MacOS VST bundle

# --------------------------------------------------------------
# Set distrho code files

DISTRHO_PLUGIN_FILES = ../../libs/distrho/DistrhoPluginMain.cpp
DISTRHO_UI_FILES     = ../../libs/distrho/DistrhoUIMain.cpp ../../libs/libdgl.a

# --------------------------------------------------------------
# Handle plugins without UI

ifeq ($(TARGET_NOUI),true)
dssi_ui =
lv2_ui =
DISTRHO_UI_FILES =
DGL_LIBS =
OBJS_UI =
endif

# --------------------------------------------------------------
# Common

%.c.o: %.c
	$(CXX) $< $(BUILD_CXX_FLAGS) -c -o $@

%.cpp.o: %.cpp
	$(CXX) $< $(BUILD_CXX_FLAGS) -c -o $@

clean:
	$(RM) *.o
	$(RM) -r $(TARGET_DIR)/$(NAME)-* $(TARGET_DIR)/$(NAME).lv2/

# --------------------------------------------------------------
# LADSPA

ladspa: $(ladspa_dsp)

$(ladspa_dsp): $(OBJS_DSP) $(DISTRHO_PLUGIN_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(SHARED) -DDISTRHO_PLUGIN_TARGET_LADSPA -o $@

# --------------------------------------------------------------
# DSSI

dssi: $(dssi_dsp) $(dssi_ui)

$(dssi_dsp): $(OBJS_DSP) $(DISTRHO_PLUGIN_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(SHARED) -DDISTRHO_PLUGIN_TARGET_DSSI -o $@

$(dssi_ui): $(OBJS_UI) $(DISTRHO_UI_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(DGL_LIBS) $(shell pkg-config --cflags --libs liblo) -DDISTRHO_PLUGIN_TARGET_DSSI -o $@

# --------------------------------------------------------------
# LV2

lv2: $(lv2_dsp) $(lv2_ui)

$(lv2_dsp): $(OBJS_DSP) $(DISTRHO_PLUGIN_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(SHARED) -DDISTRHO_PLUGIN_TARGET_LV2 -o $@

$(lv2_ui): $(OBJS_UI) $(DISTRHO_UI_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(DGL_LIBS) $(SHARED) -DDISTRHO_PLUGIN_TARGET_LV2 -o $@

# --------------------------------------------------------------
# VST

vst: $(vst)

$(vst): $(OBJS_DSP) $(OBJS_UI) $(DISTRHO_PLUGIN_FILES) $(DISTRHO_UI_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(DGL_LIBS) $(SHARED) -DDISTRHO_PLUGIN_TARGET_VST -o $@

# --------------------------------------------------------------
