/* -*- Mode: C++ ; c-basic-offset: 8 -*- */

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

#include "sfz.h"

#include <iostream>
#include <sstream>

#include <boost/lexical_cast.hpp>

namespace sfz
{

	/////////////////////////////////////////////////////////////
	// class Articulation

	Articulation::Articulation()
	{
	}

	Articulation::~Articulation()
	{
	}

	/////////////////////////////////////////////////////////////
	// class Definition
	
	Definition::Definition()
	{
	}

	Definition::~Definition()
	{
	}

	/////////////////////////////////////////////////////////////
	// class Region

	Region::Region()
	{
	}

	Region::~Region()
	{
	}

	bool
	Region::OnKey(uint8_t chan, uint8_t key, uint8_t vel,
		      int bend, uint8_t bpm, uint8_t chanaft, uint8_t polyaft,
		      uint8_t prog, float rand, trigger_t trig, uint8_t* cc,
		      float timer, uint8_t seq, bool* sw, uint8_t last_sw_key, uint8_t prev_sw_key)
	{
		// chan        (MIDI channel)
		// key         (MIDI note)
		// vel         (MIDI velocity)

		// bend        (MIDI pitch bend)
		// bpm         (host BPM)
		// chanaft     (MIDI channel pressure)
		// polyaft     (MIDI polyphonic aftertouch)
		// prog        (MIDI program change)
		// rand        (generated random number)
		// trigger     (how it was triggered)
		// cc          (all 128 CC values)

		// timer       (time since previous region in the group was triggered)
		// seq         (the state of the region sequence counter)
		// sw          (the state of region key switches, 128 possible values)
		// last_sw_key (the last key pressed in the key switch range)
		// prev_sw_key (the previous note value)

 		bool is_triggered (
 			chan    >= lochan     &&  chan    <= hichan     &&
 			key     >= lokey      &&  key     <= hikey      &&
 			vel     >= lovel      &&  vel     <= hivel      &&
 			bend    >= lobend     &&  bend    <= hibend     &&
 			bpm     >= lobpm      &&  bpm     <= hibpm      &&
 			chanaft >= lochanaft  &&  chanaft <= hichanaft  &&
 			polyaft >= lopolyaft  &&  polyaft <= hipolyaft  &&
			prog    >= loprog     &&  prog    <= hiprog     &&
 			rand    >= lorand     &&  rand    <= hirand     &&
			timer   >= lotimer    &&  timer   <= hitimer    &&
 			seq == seq_position   &&
			((sw_last >= sw_lokey && sw_last <= sw_hikey) ? (last_sw_key == sw_last) : true)  &&
                        ((sw_down >= sw_lokey && sw_down <= sw_hikey) ? (sw[sw_down]) : true)  &&
                        ((sw_up   >= sw_lokey && sw_up   <= sw_hikey) ? (!sw[sw_up])  : true)  &&
			((sw_previous != -1)                          ? (prev_sw_key == sw_previous) : true)  &&
 			((trigger && trig) != 0)
 			);

		if (!is_triggered)
			return false;

		for (int i = 0; i < 128; ++i)
		{
			if (!(cc[i] >= locc[i] && cc[i] <= hicc[i]))
				return false;
		}

		return true;
	}

	bool
	Region::OnControl(uint8_t chan, uint8_t cont, uint8_t val,
			  int bend, uint8_t bpm, uint8_t chanaft, uint8_t polyaft,
			  uint8_t prog, float rand, trigger_t trig, uint8_t* cc,
			  float timer, uint8_t seq, bool* sw, uint8_t last_sw_key, uint8_t prev_sw_key)
	{
		// chan      (MIDI channel)
		// cont      (MIDI controller)
		// val       (MIDI controller value)

		// bend      (MIDI pitch bend)
		// bpm       (host BPM)
		// chanaft   (MIDI channel pressure)
		// polyaft   (MIDI polyphonic aftertouch)
		// prog      (MIDI program change)
		// rand      (generated random number)
		// trigger   (how it was triggered)
		// cc        (all CC values)

		// timer       (time since previous region in the group was triggered)
		// seq         (the state of the region sequence counter)
		// sw          (the state of region key switches, 128 possible values)
		// last_sw_key (the last key pressed in the key switch range)
		// prev_sw_key (the previous note value)

		bool is_triggered = (
 			chan    >= lochan           &&  chan    <= hichan             &&
			((val   >= on_locc[cont]    &&  val     <= on_hicc[cont])     ||
			 (val   >= start_locc[cont] &&  val     <= start_hicc[cont])) &&
 			bend    >= lobend           &&  bend    <= hibend             &&
 			bpm     >= lobpm            &&  bpm     <= hibpm              &&
 			chanaft >= lochanaft        &&  chanaft <= hichanaft          &&
 			polyaft >= lopolyaft        &&  polyaft <= hipolyaft          &&
			prog    >= loprog           &&  prog    <= hiprog             &&
 			rand    >= lorand           &&  rand    <= hirand             &&
			timer   >= lotimer          &&  timer   <= hitimer            &&
 			seq == seq_position   &&
			((sw_last >= sw_lokey && sw_last <= sw_hikey) ? (last_sw_key == sw_last) : true)  &&
                        ((sw_down >= sw_lokey && sw_down <= sw_hikey) ? (sw[sw_down]) : true)  &&
                        ((sw_up   >= sw_lokey && sw_up   <= sw_hikey) ? (!sw[sw_up])  : true)  &&
			((sw_previous != -1)                          ? (prev_sw_key == sw_previous) : true)  &&
 			((trigger && trig) != 0)
 			);

		if (!is_triggered)
			return false;

		for (int i = 0; i < 128; ++i)
		{
			if (!(cc[i] >= locc[i] && cc[i] <= hicc[i]))
				return false;
		}

		return true;
	}

	Articulation* 
	Region::GetArticulation(int bend, uint8_t bpm, uint8_t chanaft, uint8_t polyaft, uint8_t* cc)
	{
		return new Articulation(); //todo: implement GetArticulation()
	}

	/////////////////////////////////////////////////////////////
	// class Instrument

	Instrument::Instrument()
	{
	}

	Instrument::~Instrument()
	{
		for (uint32_t i = 0; i < regions.size(); i++) {
			delete regions[i];
		}
	}

	/////////////////////////////////////////////////////////////
	// class Group

	Group::Group() :
		id(0)
	{
		Reset();
	}

	Group::~Group()
	{
		
	}

	void
	Group::Reset()
	{
		// This is where all the default values are set.

		// sample definition default
		sample = "";

		// input control
		lochan = 1; hichan = 16;
		lokey = 0; hikey = 127;
		lovel = 0; hivel = 127;
		lobend = -8192; hibend = 8192;
		lobpm = 0; hibpm = 500;
		lochanaft = 0; hichanaft = 127;
		lopolyaft = 0; hipolyaft = 127;
		loprog = 0; hiprog = 127;
		lorand = 0.0; hirand = 1.0;
		lotimer = 0.0; hitimer = 0.0;

		seq_length = 1;
		seq_position = 1;

		sw_lokey = -1; sw_hikey = -1;
		sw_last = -1;
		sw_down = -1;
		sw_up = -1;
		sw_previous = -1;
		sw_vel = VEL_CURRENT;

		trigger = TRIGGER_ATTACK;

		group = 0;
		off_by = 0;
		off_mode = OFF_FAST;

		// sample player
		count = 0;
		delay = 0; delay_random = 0; 
		delay_beats = 0; stop_beats = 0;
		delay_samples = 0;
		end = 0;
		loop_crossfade = 0;
		offset = 0; offset_random = 0;
		loop_mode = NO_LOOP;
		loop_start = 0; loop_end = 0;
		sync_beats = 0; sync_offset = 0;

		// amplifier
		volume = 0;
		pan = 0;
		width = 100;
		position = 0;
		amp_keytrack = 0;
		amp_keycenter = 60;
		amp_veltrack = 100;
		amp_random = 0;
		rt_decay = 0;
		xfin_lokey = 0; xfin_hikey = 0;
		xfout_lokey = 127; xfout_hikey = 127;
		xf_keycurve = POWER;
		xfin_lovel = 0;	xfin_hivel = 0;
		xfout_lovel = 127; xfout_hivel = 127;
		xf_velcurve = POWER;
		xf_cccurve = POWER;

		// pitch
		transpose = 0;
		tune = 0;
		pitch_keycenter = 60;
		pitch_keytrack = 100;
		pitch_veltrack = 0;
		pitch_random = 0;
		bend_up = 200;
		bend_down = -200;
		bend_step = 1;

		// filter
		fil_type = LPF_2P;
		cutoff = 0;
		cutoff_chanaft = 0;
		cutoff_polyaft = 0;
		resonance = 0;
		fil_keytrack = 0;
		fil_keycenter = 60;
		fil_veltrack = 0;
		fil_random = 0;

		fil2_type = LPF_2P;
		cutoff2 = 0;
		cutoff2_chanaft = 0;
		cutoff2_polyaft = 0;
		resonance2 = 0;
		fil2_keytrack = 0;
		fil2_keycenter = 60;
		fil2_veltrack = 0;
		fil2_random = 0;

		// per voice equalizer
		eq1_freq = 50;
		eq2_freq = 500;
		eq3_freq = 5000;
		eq1_vel2freq = 0;
		eq2_vel2freq = 0;
		eq3_vel2freq = 0;
		eq1_bw = 1;
		eq2_bw = 1;
		eq3_bw = 1;
		eq1_gain = 0;
		eq2_gain = 0;
		eq3_gain = 0;
		eq1_vel2gain = 0;
		eq2_vel2gain = 0;
		eq3_vel2gain = 0;

		// CCs
		for (int i = 0; i < 128; ++i)
		{
			// input control
			locc[i] = 0;
			hicc[i] = 127;
			start_locc[i] = -1;
			start_hicc[i] = -1;
			stop_locc[i] = -1;
			stop_hicc[i] = -1;
			on_locc[i] = -1;
			on_hicc[i] = -1;

			// sample player
			delay_oncc[i] = 0;
			delay_samples_oncc[i] = 0;
			offset_oncc[i] = 0;

			// amplifier
			amp_velcurve_[i] = 0; //fixme: 20 log (127^2 / i^2)
			gain_oncc[i] = 0;
			xfin_locc[i] = 0;
			xfin_hicc[i] = 0;
			xfout_locc[i] = 127;
			xfout_hicc[i] = 127;

			// filter
			cutoff_oncc[i] = 0;
			cutoff_smoothcc[i] = 0;
			cutoff_stepcc[i] = 0;
			cutoff_curvecc[i] = 0;
			resonance_oncc[i] = 0;
			resonance_smoothcc[i] = 0;
			resonance_stepcc[i] = 0;
			resonance_curvecc[i] = 0;

			cutoff2_oncc[i] = 0;
			cutoff2_smoothcc[i] = 0;
			cutoff2_stepcc[i] = 0;
			cutoff2_curvecc[i] = 0;
			resonance2_oncc[i] = 0;
			resonance2_smoothcc[i] = 0;
			resonance2_stepcc[i] = 0;
			resonance2_curvecc[i] = 0;

			// per voice equalizer
			eq1_freq_oncc[i] = 0;
			eq2_freq_oncc[i] = 0;
			eq3_freq_oncc[i] = 0;
			eq1_bw_oncc[i] = 0;
			eq2_bw_oncc[i] = 0;
			eq3_bw_oncc[i] = 0;
			eq1_gain_oncc[i] = 0;
			eq2_gain_oncc[i] = 0;
			eq3_gain_oncc[i] = 0;
		}
	}

	Region*
	Group::RegionFactory()
	{
		// This is where the current group setting are copied to the new region.

		Region* region = new Region();

		region->id = id++;

		// sample definition
		region->sample = sample;

		// input control
		region->lochan = lochan;
		region->hichan = hichan;
		region->lokey = lokey;
		region->hikey = hikey;
		region->lovel = lovel;
		region->hivel = hivel;
		region->locc = locc;
		region->hicc = hicc;
		region->lobend = lobend;
		region->hibend = hibend;
		region->lobpm = lobpm;
		region->hibpm = hibpm;
		region->lochanaft = lochanaft;
		region->hichanaft = hichanaft;
		region->lopolyaft = lopolyaft;
		region->hipolyaft = hipolyaft;
		region->loprog = loprog;
		region->hiprog = hiprog;
		region->lorand = lorand;
		region->hirand = hirand;
		region->lotimer = lotimer;
		region->hitimer = hitimer;
		region->seq_length = seq_length;
		region->seq_position = seq_position;
		region->start_locc = start_locc;
		region->start_hicc = start_hicc;
		region->stop_locc = stop_locc;
		region->stop_hicc = stop_hicc;
		region->sw_lokey = sw_lokey;
		region->sw_hikey = sw_hikey;
		region->sw_last = sw_last;
		region->sw_down = sw_down;
		region->sw_up = sw_up;
		region->sw_previous = sw_previous;
		region->sw_vel = sw_vel;
		region->trigger = trigger;
		region->group = group;
		region->off_by = off_by;
		region->off_mode = off_mode;
		region->on_locc = on_locc;
		region->on_hicc = on_hicc;

		// sample player
		region->count = count;
		region->delay = delay;
		region->delay_random = delay_random;
		region->delay_oncc = delay_oncc;
		region->delay_beats = delay_beats;
		region->stop_beats = stop_beats;
		region->delay_samples = delay_samples;
		region->delay_samples_oncc = delay_samples_oncc;
		region->end = end;
		region->loop_crossfade = loop_crossfade;
		region->offset = offset;
		region->offset_random = offset_random;
		region->offset_oncc = offset_oncc;
		region->loop_mode = loop_mode;
		region->loop_start = loop_start;
		region->loop_end = loop_end;
		region->sync_beats = sync_beats;
		region->sync_offset = sync_offset;

		// amplifier
		region->volume = volume;
		region->pan = pan;
		region->width = width;
		region->position = position;
		region->amp_keytrack = amp_keytrack;
		region->amp_keycenter = amp_keycenter;
		region->amp_veltrack = amp_veltrack;
		region->amp_velcurve_ = amp_velcurve_;
		region->amp_random = amp_random;
		region->rt_decay = rt_decay;
		region->gain_oncc = gain_oncc;
		region->xfin_lokey = xfin_lokey;
		region->xfin_hikey = xfin_hikey;
		region->xfout_lokey = xfout_lokey;
		region->xfout_hikey = xfout_hikey;
		region->xf_keycurve = xf_keycurve;
		region->xfin_lovel = xfin_lovel;
		region->xfin_hivel = xfin_lovel;
		region->xfout_lovel = xfout_lovel;
		region->xfout_hivel = xfout_hivel;
		region->xf_velcurve = xf_velcurve;
		region->xfin_locc = xfin_locc;
		region->xfin_hicc = xfin_hicc;
		region->xfout_locc = xfout_locc;
		region->xfout_hicc = xfout_hicc;
		region->xf_cccurve = xf_cccurve;

		// pitch
		region->transpose = transpose;
		region->tune = tune;
		region->pitch_keycenter = pitch_keycenter;
		region->pitch_keytrack = pitch_keytrack;
		region->pitch_veltrack = pitch_veltrack;
		region->pitch_random = pitch_random;
		region->bend_up = bend_up;
		region->bend_down = bend_down;
		region->bend_step = bend_step;

		// filter
		region->fil_type = fil_type;
		region->cutoff = cutoff;
		region->cutoff_oncc = cutoff_oncc;
		region->cutoff_smoothcc = cutoff_smoothcc;
		region->cutoff_stepcc = cutoff_stepcc;
		region->cutoff_curvecc = cutoff_curvecc;
		region->cutoff_chanaft = cutoff_chanaft;
		region->cutoff_polyaft = cutoff_polyaft;
		region->resonance = resonance;
		region->resonance_oncc = resonance_oncc;
		region->resonance_smoothcc = resonance_smoothcc;
		region->resonance_stepcc = resonance_stepcc;
		region->resonance_curvecc = resonance_curvecc;
		region->fil_keytrack = fil_keytrack;
		region->fil_keycenter = fil_keycenter;
		region->fil_veltrack = fil_veltrack;
		region->fil_random = fil_random;

		region->fil2_type = fil2_type;
		region->cutoff2 = cutoff2;
		region->cutoff2_oncc = cutoff2_oncc;
		region->cutoff2_smoothcc = cutoff2_smoothcc;
		region->cutoff2_stepcc = cutoff2_stepcc;
		region->cutoff2_curvecc = cutoff2_curvecc;
		region->cutoff2_chanaft = cutoff2_chanaft;
		region->cutoff2_polyaft = cutoff2_polyaft;
		region->resonance2 = resonance2;
		region->resonance2_oncc = resonance2_oncc;
		region->resonance2_smoothcc = resonance2_smoothcc;
		region->resonance2_stepcc = resonance2_stepcc;
		region->resonance2_curvecc = resonance2_curvecc;
		region->fil2_keytrack = fil2_keytrack;
		region->fil2_keycenter = fil2_keycenter;
		region->fil2_veltrack = fil2_veltrack;
		region->fil2_random = fil2_random;

		// per voice equalizer
		region->eq1_freq = eq1_freq;
		region->eq2_freq = eq2_freq;
		region->eq3_freq = eq3_freq;
		region->eq1_freq_oncc = eq1_freq_oncc;
		region->eq2_freq_oncc = eq2_freq_oncc;
		region->eq3_freq_oncc = eq3_freq_oncc;
		region->eq1_vel2freq = eq1_vel2freq;
		region->eq2_vel2freq = eq2_vel2freq;
		region->eq3_vel2freq = eq3_vel2freq;
		region->eq1_bw = eq1_bw;
		region->eq2_bw = eq2_bw;
		region->eq3_bw = eq3_bw;
		region->eq1_bw_oncc = eq1_bw_oncc;
		region->eq2_bw_oncc = eq2_bw_oncc;
		region->eq3_bw_oncc = eq3_bw_oncc;
		region->eq1_gain = eq1_gain;
		region->eq2_gain = eq2_gain;
		region->eq3_gain = eq3_gain;
		region->eq1_gain_oncc = eq1_gain_oncc;
		region->eq2_gain_oncc = eq2_gain_oncc;
		region->eq3_gain_oncc = eq3_gain_oncc;
		region->eq1_vel2gain = eq1_vel2gain;
		region->eq2_vel2gain = eq2_vel2gain;
		region->eq3_vel2gain = eq3_vel2gain;

		return region;
	}

	/////////////////////////////////////////////////////////////
	// class File

	File::File(std::string file, std::string path) :
		_instrument(new Instrument()),
		_current_section(GROUP),
		_current_group(new Group()),
		default_path(path.c_str()),
		octave_offset(0),
		note_offset(0)
	{
		
		enum token_type_t { HEADER, OPCODE };
		token_type_t token_type;
		std::string token_string;
		
		default_path = default_path + "/";
		std::string fullpath = default_path + file;
		std::ifstream fs(fullpath.c_str());
		std::string token;
		std::string line;
		token_type = HEADER;

		int currentLine = 0;

		while (std::getline(fs, line))
		{
		    currentLine++;
		    printf("line %d: %s\n", currentLine, line.c_str());
		    // COMMENT
		    std::string::size_type slash_index = line.find("//");
		    if (slash_index != std::string::npos)
		        line.resize(slash_index);

		    // #include
		    if (line.find("#include ") == 0) {
		        size_t fname_start = line.find("\"");
		        if (fname_start == std::string::npos) continue;

		        size_t fname_end = line.find("\"", fname_start + 1);
		        if (fname_end == std::string::npos || fname_start == fname_end)
		    	continue;
		        std::string fname = line.substr(fname_start + 1, fname_end - fname_start - 1);

		        std::string cd = default_path;
		        int cl = currentLine;
		        File(file, cd);
		        default_path = cd;
		        currentLine = cl;
		        continue;
		    }

		    // DEFINITION
		    std::stringstream linestream(line);
		    int spaces = 0;
		    while (linestream >> token)
		    {
		        linestream >> std::noskipws;
		        if (token[0] == '<' && token[token.size()-1] == '>')
		        {
		    	// HEAD
		    	if (!token_string.empty())
		    	{
		    	    switch (token_type)
		    	    {
		    	    case HEADER:
		    		push_header(token_string);
		    		break;
		    	    case OPCODE:
		    		push_opcode(token_string);
		    		break;
		    	    }
		    	    token_string.erase();
		    	}
		    	token_string.append(token);
		    	token_type = HEADER;
		        }
		        else if (token.find('=') != std::string::npos)
		        {
		    	// HEAD
		    	if (!token_string.empty())
		    	{
		    	    switch (token_type)
		    	    {
		    	    case HEADER:
		    		push_header(token_string);
		    		break;
		    	    case OPCODE:
		    		push_opcode(token_string);
		    		break;
		    	    }
		    	    token_string.erase();
		    	}
		    	token_string.append(token);
		    	token_type = OPCODE;
		        }
		        else
		        {
		    	// TAIL
		    	token_string.append(spaces, ' ');
		    	token_string.append(token);
		        }
		        spaces = 0;
		        while (isspace(linestream.peek())) {
		    	linestream.ignore();
		    	spaces++;
		        }
		    }

		    // EOL
		    if (!token_string.empty())
		    {
		        switch (token_type)
		        {
		        case HEADER:
		    	push_header(token_string);
		    	break;
		        case OPCODE:
		    	push_opcode(token_string);
		    	break;
		        }
		        token_string.erase();
		    }
		}
	}

	File::~File()
	{
		delete _current_group;
		delete _instrument;
	}

	Instrument*
	File::GetInstrument()
	{
		return _instrument;
	}

	void 
	File::push_header(std::string token)
	{
		if (token == "<group>")
		{
			_current_section = GROUP;
			_current_group->Reset();
		}
		else if (token == "<region>")
		{
			_current_section = REGION;
			_current_region = _current_group->RegionFactory();
			_instrument->regions.push_back(_current_region);
		}
		else if (token == "<control>")
		{
			_current_section = CONTROL;
			octave_offset = 0;
			note_offset = 0;
		}
		else 
		{
			_current_section = UNKNOWN;
			std::cerr << "The header '" << token << "' is unsupported by libsfz!" << std::endl;
		}
	}
	
	void 
	File::push_opcode(std::string token)
	{
		if (_current_section == UNKNOWN)
			return;

		std::string::size_type delimiter_index = token.find('=');
		std::string key = token.substr(0, delimiter_index);
		std::string value = token.substr(delimiter_index + 1);

		// sample definition
		if ("sample" == key) 
		{
			std::string path = default_path + value;
			for (uint32_t i = 0; i < path.length(); i++) if (path[i] == '\\') path[i] = '/';
			switch (_current_section)
			{
			case REGION:
				_current_region->sample = path;
			case GROUP:
				_current_group->sample = path;
			}
			return;
		}

		// control header directives
		else if ("default_path" == key)
		{
			switch (_current_section)
			{
			case CONTROL:
				default_path = value;
			}
			return;
		}
		else if ("octave_offset" == key)
		{
			switch (_current_section)
			{
			case CONTROL:
				octave_offset = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("note_offset" == key)
		{
			switch (_current_section)
			{
			case CONTROL:
				note_offset = boost::lexical_cast<int>(value);
			}
			return;
		}

		// input controls
		else if ("lochan" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->lochan = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->lochan = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("hichan" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->hichan = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->hichan = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("lokey" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->lokey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->lokey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("hikey" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->hikey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->hikey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("key" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->lokey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
				_current_region->hikey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->lokey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
				_current_group->hikey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("lovel" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->lovel = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->lovel = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("hivel" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->hivel = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->hivel = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("lobend" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->lobend = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->lobend = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("hibend" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->hibend = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->hibend = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("lobpm" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->lobpm = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->lobpm = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("hibpm" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->hibpm = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->hibpm = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("lochanaft" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->lochanaft = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->lochanaft = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("hichanaft" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->hichanaft = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->hichanaft = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("lopolyaft" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->lopolyaft = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->lopolyaft = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("hipolyaft" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->hipolyaft = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->hipolyaft = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("loprog" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->loprog = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->loprog = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("hiprog" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->hiprog = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->hiprog = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("lorand" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->lorand = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->lorand = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("hirand" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->hirand = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->hirand = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("lotimer" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->lotimer = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->lotimer = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("hitimer" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->hitimer = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->hitimer = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("seq_length" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->seq_length = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->seq_length = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("seq_position" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->seq_position = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->seq_position = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("sw_lokey" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->sw_lokey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->sw_lokey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("sw_hikey" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->sw_hikey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->sw_hikey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("sw_last" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->sw_last = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->sw_last = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("sw_down" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->sw_down = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->sw_down = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("sw_up" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->sw_up = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->sw_up = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("sw_previous" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->sw_previous = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->sw_previous = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("sw_vel" == key)
		{
			switch (_current_section)
			{
			case REGION:
				if (value == "current") 
					_current_region->sw_vel = VEL_CURRENT;
				else if (value == "previous") 
					_current_region->sw_vel = VEL_PREVIOUS;
			case GROUP:
				if (value == "current") 
					_current_group->sw_vel = VEL_CURRENT;
				else if (value == "previous") 
					_current_group->sw_vel = VEL_PREVIOUS;
			}
			return;
		}
		else if ("trigger" == key)
		{
			switch (_current_section)
			{
			case REGION:
				if (value == "attack") 
					_current_region->trigger = TRIGGER_ATTACK;
				else if (value == "release") 
					_current_region->trigger = TRIGGER_RELEASE;
				else if (value == "first") 
					_current_region->trigger = TRIGGER_FIRST;
				else if (value == "legato") 
					_current_region->trigger = TRIGGER_LEGATO;
			case GROUP:
				if (value == "attack") 
					_current_group->trigger = TRIGGER_ATTACK;
				else if (value == "release") 
					_current_group->trigger = TRIGGER_RELEASE;
				else if (value == "first") 
					_current_group->trigger = TRIGGER_FIRST;
				else if (value == "legato") 
					_current_group->trigger = TRIGGER_LEGATO;
			}
			return;
		}
		else if ("group" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->group = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->group = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("off_by" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->off_by = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->off_by = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("off_mode" == key)
		{
			switch (_current_section)
			{
			case REGION:
				if (value == "fast") 
					_current_region->off_mode = OFF_FAST;
				else if (value == "normal") 
					_current_region->off_mode = OFF_NORMAL;
			case GROUP:
				if (value == "fast") 
					_current_group->off_mode = OFF_FAST;
				else if (value == "normal") 
					_current_group->off_mode = OFF_NORMAL;
			}
			return;
		}

		// sample player
		else if ("count" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->count = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->count = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("delay" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->delay = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->delay = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("delay_random" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->delay_random = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->delay_random = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("delay_beats" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->delay_beats = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->delay_beats = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("stop_beats" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->stop_beats = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->stop_beats = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("delay_samples" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->delay_samples = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->delay_samples = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("end" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->end = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->end = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("loop_crossfade" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->loop_crossfade = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->loop_crossfade = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("offset_random" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->offset_random = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->offset_random = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("loop_mode" == key)
		{
			switch (_current_section)
			{
			case REGION:
				if (value == "no_loop")
					_current_region->loop_mode = NO_LOOP;
				else if (value == "one_shot")
					_current_region->loop_mode = ONE_SHOT;
				else if (value == "loop_continous")
					_current_region->loop_mode = LOOP_CONTINOUS;
				else if (value == "loop_sustain")
					_current_region->loop_mode = LOOP_SUSTAIN;
			case GROUP:
				if (value == "no_loop")
					_current_group->loop_mode = NO_LOOP;
				else if (value == "one_shot")
					_current_group->loop_mode = ONE_SHOT;
				else if (value == "loop_continous")
					_current_group->loop_mode = LOOP_CONTINOUS;
				else if (value == "loop_sustain")
					_current_group->loop_mode = LOOP_SUSTAIN;
			}
			return;
		}
		else if ("loop_start" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->loop_start = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->loop_start = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("loop_end" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->loop_end = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->loop_end = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("sync_beats" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->sync_beats = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->sync_beats = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("sync_offset" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->sync_offset = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->sync_offset = boost::lexical_cast<int>(value);
			}
			return;
		}

		// amplifier
		else if ("volume" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->volume = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->volume = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("pan" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->pan = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->pan = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("width" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->width = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->width = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("position" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->position = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->position = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("amp_keytrack" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->amp_keytrack = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->amp_keytrack = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("amp_keycenter" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->amp_keycenter = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->amp_keycenter = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("amp_veltrack" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->amp_veltrack = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->amp_veltrack = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("amp_random" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->amp_random = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->amp_random = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("rt_decay" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->rt_decay = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->rt_decay = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("xfin_lokey" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->xfin_lokey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->xfin_lokey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("xfin_hikey" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->xfin_hikey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->xfin_hikey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("xfout_lokey" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->xfout_lokey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->xfout_lokey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("xfout_hikey" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->xfout_hikey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->xfout_hikey = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("xf_keycurve" == key)
		{
			switch (_current_section)
			{
			case REGION:
				if (value == "gain")
					_current_region->xf_keycurve = GAIN;
				else if (value == "power")
					_current_region->xf_keycurve = POWER;
			case GROUP:
				if (value == "gain")
					_current_group->xf_keycurve = GAIN;
				else if (value == "power")
					_current_group->xf_keycurve = POWER;
			}
			return;
		}
		else if ("xfin_lovel" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->xfin_lovel = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->xfin_lovel = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("xfin_hivel" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->xfin_hivel = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->xfin_hivel = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("xfout_lovel" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->xfout_lovel = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->xfout_lovel = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("xfout_hivel" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->xfout_hivel = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->xfout_hivel = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("xf_velcurve" == key)
		{
			switch (_current_section)
			{
			case REGION:
				if (value == "gain")
					_current_region->xf_velcurve = GAIN;
				else if (value == "power")
					_current_region->xf_velcurve = POWER;
			case GROUP:
				if (value == "gain")
					_current_group->xf_velcurve = GAIN;
				else if (value == "power")
					_current_group->xf_velcurve = POWER;
			}
			return;
		}
		else if ("xf_cccurve" == key)
		{
			switch (_current_section)
			{
			case REGION:
				if (value == "gain")
					_current_region->xf_cccurve = GAIN;
				else if (value == "power")
					_current_region->xf_cccurve = POWER;
			case GROUP:
				if (value == "gain")
					_current_group->xf_cccurve = GAIN;
				else if (value == "power")
					_current_group->xf_cccurve = POWER;
			}
			return;
		}
		
		// pitch
		else if ("transpose" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->transpose = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->transpose = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("tune" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->tune = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->tune = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("pitch_keycenter" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->pitch_keycenter = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->pitch_keycenter = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("pitch_keytrack" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->pitch_keytrack = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->pitch_keytrack = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("pitch_veltrack" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->pitch_veltrack = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->pitch_veltrack = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("pitch_random" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->pitch_random = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->pitch_random = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("bend_up" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->bend_up = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->bend_up = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("bend_down" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->bend_down = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->bend_down = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("bend_step" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->bend_step = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->bend_step = boost::lexical_cast<int>(value);
			}
			return;
		}

		// filter
		else if ("fil_type" == key)
		{
			switch (_current_section)
			{
			case REGION:
				if (value == "lpf_1p")
					_current_region->fil_type = LPF_1P;
				else if (value == "hpf_1p")
					_current_region->fil_type = HPF_1P;
				else if (value == "bpf_1p")
					_current_region->fil_type = BPF_1P;
				else if (value == "brf_1p")
					_current_region->fil_type = BRF_1P;
				else if (value == "apf_1p")
					_current_region->fil_type = APF_1P;
				else if (value == "lpf_2p")
					_current_region->fil_type = LPF_2P;
				else if (value == "hpf_2p")
					_current_region->fil_type = HPF_2P;
				else if (value == "bpf_2p")
					_current_region->fil_type = BPF_2P;
				else if (value == "brf_2p")
					_current_region->fil_type = BRF_2P;
				else if (value == "pkf_2p")
					_current_region->fil_type = PKF_2P;
				else if (value == "lpf_4p")
					_current_region->fil_type = LPF_4P;
				else if (value == "hpf_4p")
					_current_region->fil_type = HPF_4P;
				else if (value == "lpf_6p")
					_current_region->fil_type = LPF_6P;
				else if (value == "hpf_6p")
					_current_region->fil_type = HPF_6P;
			case GROUP:
				if (value == "lpf_1p")
					_current_group->fil_type = LPF_1P;
				else if (value == "hpf_1p")
					_current_group->fil_type = HPF_1P;
				else if (value == "bpf_1p")
					_current_group->fil_type = BPF_1P;
				else if (value == "brf_1p")
					_current_group->fil_type = BRF_1P;
				else if (value == "apf_1p")
					_current_group->fil_type = APF_1P;
				else if (value == "lpf_2p")
					_current_group->fil_type = LPF_2P;
				else if (value == "hpf_2p")
					_current_group->fil_type = HPF_2P;
				else if (value == "bpf_2p")
					_current_group->fil_type = BPF_2P;
				else if (value == "brf_2p")
					_current_group->fil_type = BRF_2P;
				else if (value == "pkf_2p")
					_current_group->fil_type = PKF_2P;
				else if (value == "lpf_4p")
					_current_group->fil_type = LPF_4P;
				else if (value == "hpf_4p")
					_current_group->fil_type = HPF_4P;
				else if (value == "lpf_6p")
					_current_group->fil_type = LPF_6P;
				else if (value == "hpf_6p")
					_current_group->fil_type = HPF_6P;
			}
			return;
		}
		else if ("fil2_type" == key)
		{
			switch (_current_section)
			{
			case REGION:
				if (value == "lpf_1p")
					_current_region->fil2_type = LPF_1P;
				else if (value == "hpf_1p")
					_current_region->fil2_type = HPF_1P;
				else if (value == "bpf_1p")
					_current_region->fil2_type = BPF_1P;
				else if (value == "brf_1p")
					_current_region->fil2_type = BRF_1P;
				else if (value == "apf_1p")
					_current_region->fil2_type = APF_1P;
				else if (value == "lpf_2p")
					_current_region->fil2_type = LPF_2P;
				else if (value == "hpf_2p")
					_current_region->fil2_type = HPF_2P;
				else if (value == "bpf_2p")
					_current_region->fil2_type = BPF_2P;
				else if (value == "brf_2p")
					_current_region->fil2_type = BRF_2P;
				else if (value == "pkf_2p")
					_current_region->fil2_type = PKF_2P;
				else if (value == "lpf_4p")
					_current_region->fil2_type = LPF_4P;
				else if (value == "hpf_4p")
					_current_region->fil2_type = HPF_4P;
				else if (value == "lpf_6p")
					_current_region->fil2_type = LPF_6P;
				else if (value == "hpf_6p")
					_current_region->fil2_type = HPF_6P;
			case GROUP:
				if (value == "lpf_1p")
					_current_group->fil2_type = LPF_1P;
				else if (value == "hpf_1p")
					_current_group->fil2_type = HPF_1P;
				else if (value == "bpf_1p")
					_current_group->fil2_type = BPF_1P;
				else if (value == "brf_1p")
					_current_group->fil2_type = BRF_1P;
				else if (value == "apf_1p")
					_current_group->fil2_type = APF_1P;
				else if (value == "lpf_2p")
					_current_group->fil2_type = LPF_2P;
				else if (value == "hpf_2p")
					_current_group->fil2_type = HPF_2P;
				else if (value == "bpf_2p")
					_current_group->fil2_type = BPF_2P;
				else if (value == "brf_2p")
					_current_group->fil2_type = BRF_2P;
				else if (value == "pkf_2p")
					_current_group->fil2_type = PKF_2P;
				else if (value == "lpf_4p")
					_current_group->fil2_type = LPF_4P;
				else if (value == "hpf_4p")
					_current_group->fil2_type = HPF_4P;
				else if (value == "lpf_6p")
					_current_group->fil2_type = LPF_6P;
				else if (value == "hpf_6p")
					_current_group->fil2_type = HPF_6P;
			}
			return;
		}
		else if ("cutoff" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->cutoff = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->cutoff = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("cutoff2" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->cutoff2 = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->cutoff2 = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("cutoff_chanaft" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->cutoff_chanaft = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->cutoff_chanaft = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("cutoff2_chanaft" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->cutoff2_chanaft = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->cutoff2_chanaft = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("cutoff_polyaft" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->cutoff_polyaft = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->cutoff_polyaft = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("cutoff2_polyaft" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->cutoff2_polyaft = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->cutoff2_polyaft = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("resonance" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->resonance = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->resonance = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("resonance2" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->resonance2 = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->resonance2 = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("fil_keytrack" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->fil_keytrack = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->fil_keytrack = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("fil2_keytrack" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->fil2_keytrack = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->fil2_keytrack = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("fil_keycenter" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->fil_keycenter = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->fil_keycenter = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("fil2_keycenter" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->fil2_keycenter = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			case GROUP:
				_current_group->fil2_keycenter = boost::lexical_cast<int>(value) + note_offset + 12 * octave_offset;
			}
			return;
		}
		else if ("fil_veltrack" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->fil_veltrack = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->fil_veltrack = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("fil2_veltrack" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->fil2_veltrack = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->fil2_veltrack = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("fil_random" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->fil_random = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->fil_random = boost::lexical_cast<int>(value);
			}
			return;
		}
		else if ("fil2_random" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->fil2_random = boost::lexical_cast<int>(value);
			case GROUP:
				_current_group->fil2_random = boost::lexical_cast<int>(value);
			}
			return;
		}

		// per voice equalizer
		else if ("eq1_freq" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq1_freq = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq1_freq = boost::lexical_cast<float>(value);
			}
			return;
		}		
		else if ("eq2_freq" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq2_freq = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq2_freq = boost::lexical_cast<float>(value);
			}
			return;
		}		
		else if ("eq3_freq" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq3_freq = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq3_freq = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("eq1_vel2freq" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq1_vel2freq = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq1_vel2freq = boost::lexical_cast<float>(value);
			}
			return;
		}		
		else if ("eq2_vel2freq" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq2_vel2freq = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq2_vel2freq = boost::lexical_cast<float>(value);
			}
			return;
		}		
		else if ("eq3_vel2freq" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq3_vel2freq = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq3_vel2freq = boost::lexical_cast<float>(value);
			}
			return;
		}		
		else if ("eq1_bw" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq1_bw = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq1_bw = boost::lexical_cast<float>(value);
			}
			return;
		}		
		else if ("eq2_bw" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq2_bw = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq2_bw = boost::lexical_cast<float>(value);
			}
			return;
		}		
		else if ("eq3_bw" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq3_bw = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq3_bw = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("eq1_gain" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq1_gain = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq1_gain = boost::lexical_cast<float>(value);
			}
			return;
		}		
		else if ("eq2_gain" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq2_gain = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq2_gain = boost::lexical_cast<float>(value);
			}
			return;
		}		
		else if ("eq3_gain" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq3_gain = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq3_gain = boost::lexical_cast<float>(value);
			}
			return;
		}
		else if ("eq1_vel2gain" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq1_vel2gain = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq1_vel2gain = boost::lexical_cast<float>(value);
			}
			return;
		}		
		else if ("eq2_vel2gain" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq2_vel2gain = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq2_vel2gain = boost::lexical_cast<float>(value);
			}
			return;
		}		
		else if ("eq3_vel2gain" == key)
		{
			switch (_current_section)
			{
			case REGION:
				_current_region->eq3_vel2gain = boost::lexical_cast<float>(value);
			case GROUP:
				_current_group->eq3_vel2gain = boost::lexical_cast<float>(value);
			}
			return;
		}

		//fixme: parse amp_velcurve_N

		// CCs
		else
		{
			std::string::size_type delimiter_index = key.find("cc");
			std::string key_cc = key.substr(0, delimiter_index);
			int num_cc = boost::lexical_cast<int>(key.substr(delimiter_index + 2));

			// input controls
			if ("lo" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->locc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->locc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("hi" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->hicc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->hicc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("start_lo" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->start_locc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->start_locc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("start_hi" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->start_hicc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->start_hicc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("stop_lo" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->stop_locc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->stop_locc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("stop_hi" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->stop_hicc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->stop_hicc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("on_lo" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->on_locc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->on_locc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("on_hi" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->on_hicc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->on_hicc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}

			// sample player
			else if ("delay_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->delay_oncc[num_cc] = boost::lexical_cast<float>(value);
				case GROUP:
					_current_group->delay_oncc[num_cc] = boost::lexical_cast<float>(value);
				}
				return;
			}
			else if ("delay_samples_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->delay_samples_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->delay_samples_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("offset_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->offset_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->offset_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}

			// amplifier
			else if ("gain_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->gain_oncc[num_cc] = boost::lexical_cast<float>(value);
				case GROUP:
					_current_group->gain_oncc[num_cc] = boost::lexical_cast<float>(value);
				}
				return;
			}
			else if ("xfin_lo" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->xfin_locc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->xfin_locc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("xfin_hi" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->xfin_hicc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->xfin_hicc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("xfout_lo" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->xfout_locc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->xfout_locc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("xfout_hi" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->xfout_hicc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->xfout_hicc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}

			// filter
			else if ("cutoff_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->cutoff_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->cutoff_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("cutoff2_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->cutoff2_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->cutoff2_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("cutoff_smooth" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->cutoff_smoothcc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->cutoff_smoothcc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("cutoff2_smooth" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->cutoff2_smoothcc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->cutoff2_smoothcc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("cutoff_step" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->cutoff_stepcc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->cutoff_stepcc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("cutoff2_step" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->cutoff2_stepcc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->cutoff2_stepcc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("cutoff_curve" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->cutoff_curvecc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->cutoff_curvecc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("cutoff2_curve" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->cutoff2_curvecc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->cutoff2_curvecc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("resonance_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->resonance_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->resonance_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("resonance2_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->resonance2_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->resonance2_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("resonance_smooth" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->resonance_smoothcc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->resonance_smoothcc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("resonance2_smooth" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->resonance2_smoothcc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->resonance2_smoothcc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("resonance_step" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->resonance_stepcc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->resonance_stepcc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("resonance2_step" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->resonance2_stepcc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->resonance2_stepcc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("resonance_curve" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->resonance_curvecc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->resonance_curvecc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("resonance2_curve" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->resonance2_curvecc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->resonance2_curvecc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}

			// per voice equalizer
			else if ("eq1_freq_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->eq1_freq_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->eq1_freq_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("eq2_freq_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->eq2_freq_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->eq2_freq_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("eq3_freq_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->eq3_freq_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->eq3_freq_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("eq1_bw_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->eq1_bw_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->eq1_bw_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("eq2_bw_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->eq2_bw_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->eq2_bw_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("eq3_bw_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->eq3_bw_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->eq3_bw_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("eq1_gain_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->eq1_gain_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->eq1_gain_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("eq2_gain_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->eq2_gain_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->eq2_gain_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
			else if ("eq3_gain_on" == key_cc)
			{
				switch (_current_section)
				{
				case REGION:
					_current_region->eq3_gain_oncc[num_cc] = boost::lexical_cast<int>(value);
				case GROUP:
					_current_group->eq3_gain_oncc[num_cc] = boost::lexical_cast<int>(value);
				}
				return;
			}
		}

		std::cerr << "The opcode '" << key << "' is unsupported by libsfz!" << std::endl;
	}
	

} // !namespace sfz
