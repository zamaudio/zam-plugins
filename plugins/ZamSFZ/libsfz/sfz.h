/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef LIBSFZ_SFZ_H
#define LIBSFZ_SFZ_H

// SFZ 1.0
// Copyright (c) 2008-2009, Anders Dahnielson
//
// Contact: anders@dahnielson.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include <boost/array.hpp>

#define TRIGGER_ATTACK  ((unsigned char) (1 << 0)) // 0x01
#define TRIGGER_RELEASE ((unsigned char) (1 << 1)) // 0x02
#define TRIGGER_FIRST   ((unsigned char) (1 << 2)) // 0x04
#define TRIGGER_LEGATO  ((unsigned char) (1 << 3)) // 0x08

namespace sfz 
{

	// Forward declarations
	class Articulation;
	class Region;
	class Group;
	class Instrument;
	class File;

	// Enumerations
	enum sw_vel_t    { VEL_CURRENT, VEL_PREVIOUS };
	enum off_mode_t  { OFF_FAST, OFF_NORMAL };
	enum loop_mode_t { NO_LOOP, ONE_SHOT, LOOP_CONTINOUS, LOOP_SUSTAIN };
	enum curve_t     { GAIN, POWER };
	enum filter_t    { LPF_1P, HPF_1P, BPF_1P, BRF_1P, APF_1P, 
			   LPF_2P, HPF_2P, BPF_2P, BRF_2P, PKF_2P, 
			   LPF_4P, HPF_4P, 
			   LPF_6P, HPF_6P };

	typedef unsigned char trigger_t;
	typedef unsigned char uint8_t;

	/////////////////////////////////////////////////////////////
	// class Articulation

	// Articulation containing all performance parameters for synthesis
	class Articulation
	{
	public:
		Articulation();
		virtual ~Articulation();
	};

	/////////////////////////////////////////////////////////////
	// class Definition

	// Base definition used by groups and regions
	class Definition
	{
	public:
		Definition();
		virtual ~Definition();

		// sample definition
		std::string sample;

		// input controls
		int   lochan;    int   hichan;
		int   lokey;     int   hikey;
		int   lovel;     int   hivel;
		boost::array<int, 128> locc; boost::array<int, 128> hicc;
		int   lobend;    int   hibend;
		int   lobpm;     int   hibpm;
		int   lochanaft; int   hichanaft;
		int   lopolyaft; int   hipolyaft;
		int   loprog;    int   hiprog;
		float lorand;    float hirand;
		float lotimer;   float hitimer;

		int seq_length;  
		int seq_position;

		boost::array<int, 128> start_locc; boost::array<int, 128> start_hicc;
		boost::array<int, 128> stop_locc;  boost::array<int, 128> stop_hicc;

		int sw_lokey;    int sw_hikey;  
		int sw_last;
		int sw_down;     
		int sw_up;
		int sw_previous; 
		sw_vel_t sw_vel;

		trigger_t trigger;

		int group;
		int off_by;
		off_mode_t off_mode;

		boost::array<int, 128> on_locc; boost::array<int, 128> on_hicc;

		// sample player
		int count;
		float delay; float delay_random; boost::array<float, 128> delay_oncc;
		int delay_beats; int stop_beats;
		int delay_samples; boost::array<int, 128> delay_samples_oncc;
		int end;
		float loop_crossfade;
		int offset; int offset_random; boost::array<int, 128> offset_oncc;
		loop_mode_t loop_mode;
		int loop_start; int loop_end;
		int sync_beats;
		int sync_offset;
		
		// amplifier
		float volume;
		float pan;
		float width;
		float position;
		float amp_keytrack; int amp_keycenter; float amp_veltrack; boost::array<float, 128> amp_velcurve_; float amp_random;
		float rt_decay;
		boost::array<float, 128> gain_oncc;
		int xfin_lokey; int xfin_hikey;
		int xfout_lokey; int xfout_hikey;
		curve_t xf_keycurve;
		int xfin_lovel; int xfin_hivel;
		int xfout_lovel; int xfout_hivel;
		curve_t xf_velcurve;
		boost::array<int, 128> xfin_locc; boost::array<int, 128> xfin_hicc;
		boost::array<int, 128> xfout_locc; boost::array<int, 128> xfout_hicc;
		curve_t xf_cccurve;

		// pitch
		int transpose;
		int tune;
		int pitch_keycenter; int pitch_keytrack; int pitch_veltrack; int pitch_random;
		int bend_up; int bend_down; int bend_step;

		// filter
		filter_t fil_type; filter_t fil2_type;
		float cutoff; float cutoff2;
		boost::array<int, 128> cutoff_oncc; boost::array<int, 128> cutoff2_oncc;
		boost::array<int, 128> cutoff_smoothcc; boost::array<int, 128> cutoff2_smoothcc;
		boost::array<int, 128> cutoff_stepcc; boost::array<int, 128> cutoff2_stepcc;
		boost::array<int, 128> cutoff_curvecc; boost::array<int, 128> cutoff2_curvecc;
		int cutoff_chanaft; int cutoff2_chanaft;
		int cutoff_polyaft; int cutoff2_polyaft;
		float resonance; float resonance2;
		boost::array<int, 128> resonance_oncc; boost::array<int, 128> resonance2_oncc;
		boost::array<int, 128> resonance_smoothcc; boost::array<int, 128> resonance2_smoothcc;
		boost::array<int, 128> resonance_stepcc; boost::array<int, 128> resonance2_stepcc;
		boost::array<int, 128> resonance_curvecc; boost::array<int, 128> resonance2_curvecc;
		int fil_keytrack; int fil2_keytrack;
		int fil_keycenter; int fil2_keycenter;
		int fil_veltrack; int fil2_veltrack;
		int fil_random; int fil2_random;

		// per voice equalizer
		float eq1_freq; float eq2_freq; float eq3_freq;
		boost::array<float, 128> eq1_freq_oncc; boost::array<float, 128> eq2_freq_oncc; boost::array<float, 128> eq3_freq_oncc;
		float eq1_vel2freq; float eq2_vel2freq; float eq3_vel2freq;
		float eq1_bw; float eq2_bw; float eq3_bw;
		boost::array<float, 128> eq1_bw_oncc; boost::array<float, 128> eq2_bw_oncc; boost::array<float, 128> eq3_bw_oncc;
		float eq1_gain; float eq2_gain; float eq3_gain;
		boost::array<float, 128> eq1_gain_oncc; boost::array<float, 128> eq2_gain_oncc; boost::array<float, 128> eq3_gain_oncc;
		float eq1_vel2gain; float eq2_vel2gain; float eq3_vel2gain;
	};
	
	/////////////////////////////////////////////////////////////
	// class Region

	/// Defines Region information of an Instrument
	class Region :
		public Definition
	{
	public:
		Region();
		virtual ~Region();

		/// Return true if region is triggered by key
		bool OnKey(uint8_t chan, uint8_t key, uint8_t vel,
			   int bend, uint8_t bpm, uint8_t chanaft, uint8_t polyaft,
			   uint8_t prog, float rand, trigger_t trig, uint8_t* cc,
			   float timer, uint8_t seq, bool* sw, uint8_t last_sw_key, uint8_t prev_sw_key);

		/// Return true if region is triggered by control change
		bool OnControl(uint8_t chan, uint8_t cont, uint8_t val,
			       int bend, uint8_t bpm, uint8_t chanaft, uint8_t polyaft,
			       uint8_t prog, float rand, trigger_t trig, uint8_t* cc,
			       float timer, uint8_t seq, bool* sw, uint8_t last_sw_key, uint8_t prev_sw_key);

		/// Return an articulation for the current state
 		Articulation* GetArticulation(int bend, uint8_t bpm, uint8_t chanaft, uint8_t polyaft, uint8_t* cc);

		// unique region id
		int id;
	};

	/////////////////////////////////////////////////////////////
	// class Instrument

	/// Provides all neccessary information for the synthesis of an Instrument
	class Instrument
	{
	public:
		Instrument();
		virtual ~Instrument();

		/// List of Regions belonging to this Instrument
		std::vector<Region*> regions;
	};

	/////////////////////////////////////////////////////////////
	// class Group

	/// A Group act just as a template containing Region default values
	class Group :
		public Definition
	{
	public:
		Group();
		virtual ~Group();

		/// Reset Group to default values
		void Reset();

		/// Create a new Region
		Region* RegionFactory();

		// id counter
		int id;

	};

	/////////////////////////////////////////////////////////////
	// class File

	/// Parses SFZ files and provides abstract access to the data
	class File
	{
	public:
		/// Load an existing SFZ file
		File(std::string file, std::string path);
		virtual ~File();

		/// Returns a pointer to the instrument object
		Instrument* GetInstrument();

	private:
		void push_header(std::string token);
		void push_opcode(std::string token);

		/// Pointer to the Instrument belonging to this file
		Instrument* _instrument;

		// state variables
		enum section_t { UNKNOWN, GROUP, REGION, CONTROL };
		section_t _current_section;
		Region* _current_region;
		Group* _current_group;

		// control header directives
		std::string default_path;
		int octave_offset;
		int note_offset;
	};

} // !namespace sfz

#endif // !LIBSFZ_SFZ_H
