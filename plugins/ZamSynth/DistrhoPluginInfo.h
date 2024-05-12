/*
 * ZamSynth  
 * Copyright (C) 2014 Damien Zammit <damien@zamaudio.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * For a full copy of the GNU General Public License see the doc/GPL.txt file.
 */

#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_BRAND "ZamAudio"
#define DISTRHO_PLUGIN_NAME  "ZamSynth"

#define DISTRHO_PLUGIN_HAS_UI        1
#define DISTRHO_PLUGIN_IS_SYNTH      1

#define DISTRHO_PLUGIN_NUM_INPUTS    0
#define DISTRHO_PLUGIN_NUM_OUTPUTS   2

#define DISTRHO_PLUGIN_WANT_LATENCY  0
#define DISTRHO_PLUGIN_WANT_PROGRAMS 0
#define DISTRHO_PLUGIN_WANT_STATE    1
#define DISTRHO_PLUGIN_WANT_TIMEPOS  0

#define DISTRHO_PLUGIN_URI "urn:zamaudio:ZamSynth"
#define DISTRHO_PLUGIN_LV2_CATEGORY "lv2:InstrumentPlugin"

#define DPF_VST3_DONT_USE_BRAND_ID   1
#define DISTRHO_PLUGIN_BRAND_ID ZamP
#define DISTRHO_PLUGIN_UNIQUE_ID zsth

#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED
