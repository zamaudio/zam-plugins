/*
 * ZamPhono
 * Copyright (C) 2016  Damien Zammit <damien@zamaudio.com>
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
 */

#ifndef ZAMPHONOPLUGIN_HPP_INCLUDED
#define ZAMPHONOPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include <complex>

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamPhonoPlugin : public Plugin
{
public:
    enum Parameters
    {
        paramToggle = 0,
        paramType,
        paramCount
    };

    ZamPhonoPlugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZamPhono";
    }

    const char* getMaker() const noexcept override
    {
        return "Damien Zammit";
    }

    const char* getLicense() const noexcept override
    {
        return "GPL v2+";
    }

    uint32_t getVersion() const noexcept override
    {
        return d_version(4, 3, 0);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'M', 'P', 'H');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) ;
    void initProgramName(uint32_t index, String& programName) ;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;
    void  loadProgram(uint32_t index) override;

    // -------------------------------------------------------------------
    // Process
	void aweight(float);

	static inline double
	sanitize_denormal(double v) {
	        if(!std::isnormal(v))
	                return 0.f;
	        return v;
	}

	static inline double
	from_dB(double gdb) {
	        return (exp(0.05*gdb*log(10.0)));
	}

	static inline double
	to_dB(double g) {
	        return (20.0*log10(g));
	}

    void activate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;
    void emphasis(float srate);
    double run_filter(double in);
    void brickwall(float fc, float srate);
    void clearbrickwall(void);
    double run_brickwall(double in);

        double zn1, zn2, zd1, zd2;
        double b0, b1, b2;
        double a1, a2;
    double state[4];
    double A0, A1, A2, B0, B1, B2;

    // -------------------------------------------------------------------

private:
    float type, inv, typeold, invold;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMPHONO_HPP_INCLUDED
