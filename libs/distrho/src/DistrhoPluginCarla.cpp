/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2014 Filipe Coelho <falktx@falktx.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * For a full copy of the license see the LGPL.txt file
 */

#include "DistrhoPluginInternal.hpp"

#if DISTRHO_PLUGIN_HAS_UI
# include "DistrhoUIInternal.hpp"
#endif

#include "CarlaNative.hpp"

// -----------------------------------------------------------------------

START_NAMESPACE_DISTRHO

#if DISTRHO_PLUGIN_HAS_UI
// -----------------------------------------------------------------------
// Carla UI

#if ! DISTRHO_PLUGIN_WANT_STATE
static const setStateFunc setStateCallback = nullptr;
#endif
#if ! DISTRHO_PLUGIN_IS_SYNTH
static const sendNoteFunc sendNoteCallback = nullptr;
#endif

class UICarla
{
public:
    UICarla(const NativeHostDescriptor* const host, PluginExporter* const plugin)
        : fHost(host),
          fPlugin(plugin),
          fUI(this, 0, editParameterCallback, setParameterCallback, setStateCallback, sendNoteCallback, setSizeCallback, plugin->getInstancePointer())
    {
        fUI.setTitle(host->uiName);

        if (host->uiParentId != 0)
            fUI.setTransientWinId(host->uiParentId);
    }

    // ---------------------------------------------

    void carla_show(const bool yesNo)
    {
        fUI.setVisible(yesNo);
    }

    void carla_idle()
    {
        fUI.idle();
    }

    void carla_setParameterValue(const uint32_t index, const float value)
    {
        fUI.parameterChanged(index, value);
    }

#if DISTRHO_PLUGIN_WANT_PROGRAMS
    void carla_setMidiProgram(const uint32_t realProgram)
    {
        fUI.programChanged(realProgram);
    }
#endif

#if DISTRHO_PLUGIN_WANT_STATE
    void carla_setCustomData(const char* const key, const char* const value)
    {
        fUI.stateChanged(key, value);
    }
#endif

    void carla_setUiTitle(const char* const uiTitle)
    {
        fUI.setTitle(uiTitle);
    }

    // ---------------------------------------------

protected:
    void handleEditParameter(const uint32_t, const bool)
    {
        // TODO
    }

    void handleSetParameterValue(const uint32_t rindex, const float value)
    {
        fHost->ui_parameter_changed(fHost->handle, rindex, value);
    }

    void handleSetState(const char* const key, const char* const value)
    {
        fHost->ui_custom_data_changed(fHost->handle, key, value);
    }

    void handleSendNote(const uint8_t, const uint8_t, const uint8_t)
    {
        // TODO
    }

    void handleSetSize(const uint width, const uint height)
    {
        fUI.setSize(width, height);
    }

    // ---------------------------------------------

private:
    // Plugin stuff
    const NativeHostDescriptor* const fHost;
    PluginExporter* const fPlugin;

    // UI
    UIExporter fUI;

    // ---------------------------------------------
    // Callbacks

    #define handlePtr ((UICarla*)ptr)

    static void editParameterCallback(void* ptr, uint32_t index, bool started)
    {
        handlePtr->handleEditParameter(index, started);
    }

    static void setParameterCallback(void* ptr, uint32_t rindex, float value)
    {
        handlePtr->handleSetParameterValue(rindex, value);
    }

#if DISTRHO_PLUGIN_WANT_STATE
    static void setStateCallback(void* ptr, const char* key, const char* value)
    {
        handlePtr->handleSetState(key, value);
    }
#endif

#if DISTRHO_PLUGIN_IS_SYNTH
    static void sendNoteCallback(void* ptr, uint8_t channel, uint8_t note, uint8_t velocity)
    {
        handlePtr->handleSendNote(channel, note, velocity);
    }
#endif

    static void setSizeCallback(void* ptr, uint width, uint height)
    {
        handlePtr->handleSetSize(width, height);
    }

    #undef handlePtr

    CARLA_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UICarla)
};
#endif // DISTRHO_PLUGIN_HAS_UI

// -----------------------------------------------------------------------
// Carla Plugin

class PluginCarla : public NativePluginClass
{
public:
    PluginCarla(const NativeHostDescriptor* const host)
        : NativePluginClass(host)
    {
#if DISTRHO_PLUGIN_HAS_UI
        fUiPtr = nullptr;
#endif
    }

    ~PluginCarla() override
    {
#if DISTRHO_PLUGIN_HAS_UI
        if (fUiPtr != nullptr)
        {
            delete fUiPtr;
            fUiPtr = nullptr;
        }
#endif
    }

protected:
    // -------------------------------------------------------------------
    // Plugin parameter calls

    uint32_t getParameterCount() const override
    {
        return fPlugin.getParameterCount();
    }

    const NativeParameter* getParameterInfo(const uint32_t index) const override
    {
        CARLA_SAFE_ASSERT_RETURN(index < getParameterCount(), nullptr);

        static NativeParameter param;

        // reset
        param.hints = ::PARAMETER_IS_ENABLED;
        param.scalePointCount = 0;
        param.scalePoints = nullptr;

        {
            int      nativeParamHints = ::PARAMETER_IS_ENABLED;
            const uint32_t paramHints = fPlugin.getParameterHints(index);

            if (paramHints & PARAMETER_IS_AUTOMABLE)
                nativeParamHints |= ::PARAMETER_IS_AUTOMABLE;
            if (paramHints & PARAMETER_IS_BOOLEAN)
                nativeParamHints |= ::PARAMETER_IS_BOOLEAN;
            if (paramHints & PARAMETER_IS_INTEGER)
                nativeParamHints |= ::PARAMETER_IS_INTEGER;
            if (paramHints & PARAMETER_IS_LOGARITHMIC)
                nativeParamHints |= ::PARAMETER_IS_LOGARITHMIC;
            if (paramHints & PARAMETER_IS_OUTPUT)
                nativeParamHints |= ::PARAMETER_IS_OUTPUT;

            param.hints = static_cast<NativeParameterHints>(nativeParamHints);
        }

        param.name = fPlugin.getParameterName(index);
        param.unit = fPlugin.getParameterUnit(index);

        {
            const ParameterRanges& ranges(fPlugin.getParameterRanges(index));

            param.ranges.def = ranges.def;
            param.ranges.min = ranges.min;
            param.ranges.max = ranges.max;
        }

        return &param;
    }

    float getParameterValue(const uint32_t index) const override
    {
        CARLA_SAFE_ASSERT_RETURN(index < getParameterCount(), 0.0f);

        return fPlugin.getParameterValue(index);
    }

    // -------------------------------------------------------------------
    // Plugin midi-program calls

#if DISTRHO_PLUGIN_WANT_PROGRAMS
    uint32_t getMidiProgramCount() const override
    {
        return fPlugin.getProgramCount();
    }

    const NativeMidiProgram* getMidiProgramInfo(const uint32_t index) const override
    {
        CARLA_SAFE_ASSERT_RETURN(index < getMidiProgramCount(), nullptr);

        static NativeMidiProgram midiProgram;

        midiProgram.bank    = index / 128;
        midiProgram.program = index % 128;
        midiProgram.name    = fPlugin.getProgramName(index);

        return &midiProgram;
    }
#endif

    // -------------------------------------------------------------------
    // Plugin state calls

    void setParameterValue(const uint32_t index, const float value) override
    {
        CARLA_SAFE_ASSERT_RETURN(index < getParameterCount(),);

        fPlugin.setParameterValue(index, value);
    }

#if DISTRHO_PLUGIN_WANT_PROGRAMS
    void setMidiProgram(const uint8_t, const uint32_t bank, const uint32_t program) override
    {
        const uint32_t realProgram(bank * 128 + program);

        CARLA_SAFE_ASSERT_RETURN(realProgram < getMidiProgramCount(),);

        fPlugin.setProgram(realProgram);
    }
#endif

#if DISTRHO_PLUGIN_WANT_STATE
    void setCustomData(const char* const key, const char* const value) override
    {
        CARLA_SAFE_ASSERT_RETURN(key != nullptr && key[0] != '\0',);
        CARLA_SAFE_ASSERT_RETURN(value != nullptr,);

        fPlugin.setState(key, value);
    }
#endif

    // -------------------------------------------------------------------
    // Plugin process calls

    void activate() override
    {
        fPlugin.activate();
    }

    void deactivate() override
    {
        fPlugin.deactivate();
    }

#if DISTRHO_PLUGIN_IS_SYNTH
    void process(float** const inBuffer, float** const outBuffer, const uint32_t frames, const NativeMidiEvent* const midiEvents, const uint32_t midiEventCount) override
    {
        MidiEvent realMidiEvents[midiEventCount];

        for (uint32_t i=0; i < midiEventCount; ++i)
        {
            const NativeMidiEvent& midiEvent(midiEvents[i]);
            MidiEvent& realMidiEvent(realMidiEvents[i]);

            realMidiEvent.frame = midiEvent.time;
            realMidiEvent.size  = midiEvent.size;

            carla_copy<uint8_t>(realMidiEvent.buf, midiEvent.data, midiEvent.size);
        }

        fPlugin.run(const_cast<const float**>(inBuffer), outBuffer, frames, realMidiEvents, midiEventCount);
    }
#else
    void process(float** const inBuffer, float** const outBuffer, const uint32_t frames, const NativeMidiEvent* const, const uint32_t) override
    {
        fPlugin.run(const_cast<const float**>(inBuffer), outBuffer, frames);
    }
#endif

    // -------------------------------------------------------------------
    // Plugin UI calls

#if DISTRHO_PLUGIN_HAS_UI
    void uiShow(const bool show) override
    {
        if (show)
            createUiIfNeeded();

        if (fUiPtr != nullptr)
            fUiPtr->carla_show(show);
    }

    void uiIdle() override
    {
        CARLA_SAFE_ASSERT_RETURN(fUiPtr != nullptr,);

        fUiPtr->carla_idle();
    }

    void uiSetParameterValue(const uint32_t index, const float value) override
    {
        CARLA_SAFE_ASSERT_RETURN(fUiPtr != nullptr,);
        CARLA_SAFE_ASSERT_RETURN(index < getParameterCount(),);

        fUiPtr->carla_setParameterValue(index, value);
    }

# if DISTRHO_PLUGIN_WANT_PROGRAMS
    void uiSetMidiProgram(const uint8_t, const uint32_t bank, const uint32_t program) override
    {
        CARLA_SAFE_ASSERT_RETURN(fUiPtr != nullptr,);

        const uint32_t realProgram(bank * 128 + program);

        CARLA_SAFE_ASSERT_RETURN(realProgram < getMidiProgramCount(),);

        fUiPtr->carla_setMidiProgram(realProgram);
    }
# endif

# if DISTRHO_PLUGIN_WANT_STATE
    void uiSetCustomData(const char* const key, const char* const value) override
    {
        CARLA_SAFE_ASSERT_RETURN(fUiPtr != nullptr,);
        CARLA_SAFE_ASSERT_RETURN(key != nullptr && key[0] != '\0',);
        CARLA_SAFE_ASSERT_RETURN(value != nullptr,);

        fUiPtr->carla_setCustomData(key, value);
    }
# endif
#endif

    // -------------------------------------------------------------------
    // Plugin dispatcher calls

    void bufferSizeChanged(const uint32_t bufferSize) override
    {
        fPlugin.setBufferSize(bufferSize, true);
    }

    void sampleRateChanged(const double sampleRate) override
    {
        fPlugin.setSampleRate(sampleRate, true);
    }

#if DISTRHO_PLUGIN_HAS_UI
    void uiNameChanged(const char* const uiName) override
    {
        CARLA_SAFE_ASSERT_RETURN(fUiPtr != nullptr,);

        fUiPtr->carla_setUiTitle(uiName);
    }
#endif

    // -------------------------------------------------------------------

private:
    PluginExporter fPlugin;

#if DISTRHO_PLUGIN_HAS_UI
    // UI
    UICarla* fUiPtr;

    void createUiIfNeeded()
    {
        if (fUiPtr == nullptr)
        {
            d_lastUiSampleRate = getSampleRate();
            fUiPtr = new UICarla(getHostHandle(), &fPlugin);
        }
    }
#endif

    CARLA_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginCarla)

    // -------------------------------------------------------------------

public:
    static NativePluginHandle _instantiate(const NativeHostDescriptor* host)
    {
        d_lastBufferSize = host->get_buffer_size(host->handle);
        d_lastSampleRate = host->get_sample_rate(host->handle);
        return new PluginCarla(host);
    }

    static void _cleanup(NativePluginHandle handle)
    {
        delete (PluginCarla*)handle;
    }
};

END_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------
