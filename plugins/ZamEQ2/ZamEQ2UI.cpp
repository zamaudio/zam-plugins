/*
 * ZamEQ2 2 band parametric equaliser
 * Copyright (C) 2014  Damien Zammit <damien@zamaudio.com> 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * For a full copy of the GNU General Public License see the doc/GPL.txt file.
 */

#include "ZamEQ2UI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamEQ2UI::ZamEQ2UI()
    : UI()
{
    // background
    fImgBackground = Image(ZamEQ2Artwork::zameq2Data, ZamEQ2Artwork::zameq2Width, ZamEQ2Artwork::zameq2Height, GL_BGR);

    // knob
    Image knobImage(ZamEQ2Artwork::knobData, ZamEQ2Artwork::knobWidth, ZamEQ2Artwork::knobHeight);

    // knob 
    fKnobGain1 = new ImageKnob(this, knobImage);
    fKnobGain1->setPos(91, 172);
    fKnobGain1->setRange(-50.f, 20.0f);
    fKnobGain1->setStep(1.f);
    fKnobGain1->setValue(0.0f);
    fKnobGain1->setRotationAngle(240);
    fKnobGain1->setCallback(this);

    fKnobQ1 = new ImageKnob(this, knobImage);
    fKnobQ1->setPos(91, 122);
    fKnobQ1->setRange(0.1f, 7.0f);
    fKnobQ1->setStep(0.1f);
    fKnobQ1->setValue(1.0f);
    fKnobQ1->setRotationAngle(240);
    fKnobQ1->setCallback(this);

    fKnobFreq1 = new ImageKnob(this, knobImage);
    fKnobFreq1->setPos(23, 144);
    fKnobFreq1->setRange(20.f, 20000.0f);
    fKnobFreq1->setLogScale(true);
    fKnobFreq1->setStep(1.f);
    fKnobFreq1->setValue(500.0f);
    fKnobFreq1->setRotationAngle(240);
    fKnobFreq1->setCallback(this);

    fKnobGain2 = new ImageKnob(this, knobImage);
    fKnobGain2->setPos(567, 172);
    fKnobGain2->setRange(-50.f, 20.0f);
    fKnobGain2->setStep(1.f);
    fKnobGain2->setValue(0.0f);
    fKnobGain2->setRotationAngle(240);
    fKnobGain2->setCallback(this);

    fKnobQ2 = new ImageKnob(this, knobImage);
    fKnobQ2->setPos(567, 122);
    fKnobQ2->setRange(0.1f, 7.0f);
    fKnobQ2->setStep(0.1f);
    fKnobQ2->setValue(1.0f);
    fKnobQ2->setRotationAngle(240);
    fKnobQ2->setCallback(this);

    fKnobFreq2 = new ImageKnob(this, knobImage);
    fKnobFreq2->setPos(499, 144);
    fKnobFreq2->setRange(20.f, 20000.0f);
    fKnobFreq2->setStep(1.f);
    fKnobFreq2->setLogScale(true);
    fKnobFreq2->setValue(500.0f);
    fKnobFreq2->setRotationAngle(240);
    fKnobFreq2->setCallback(this);

    fKnobGainL = new ImageKnob(this, knobImage);
    fKnobGainL->setPos(91, 52);
    fKnobGainL->setRange(-50.f, 20.0f);
    fKnobGainL->setStep(1.f);
    fKnobGainL->setValue(0.0f);
    fKnobGainL->setRotationAngle(240);
    fKnobGainL->setCallback(this);

    fKnobFreqL = new ImageKnob(this, knobImage);
    fKnobFreqL->setPos(23, 23);
    fKnobFreqL->setRange(20.f, 20000.0f);
    fKnobFreqL->setStep(1.f);
    fKnobFreqL->setLogScale(true);
    fKnobFreqL->setValue(500.0f);
    fKnobFreqL->setRotationAngle(240);
    fKnobFreqL->setCallback(this);

    fKnobGainH = new ImageKnob(this, knobImage);
    fKnobGainH->setPos(567, 53);
    fKnobGainH->setRange(-50.f, 20.0f);
    fKnobGainH->setStep(1.f);
    fKnobGainH->setValue(0.0f);
    fKnobGainH->setRotationAngle(240);
    fKnobGainH->setCallback(this);

    fKnobFreqH = new ImageKnob(this, knobImage);
    fKnobFreqH->setPos(499, 24);
    fKnobFreqH->setRange(20.f, 20000.0f);
    fKnobFreqH->setStep(1.f);
    fKnobFreqH->setLogScale(true);
    fKnobFreqH->setValue(500.0f);
    fKnobFreqH->setRotationAngle(240);
    fKnobFreqH->setCallback(this);

}

ZamEQ2UI::~ZamEQ2UI()
{
    delete fKnobGain1;
    delete fKnobQ1;
    delete fKnobFreq1;
    delete fKnobGain2;
    delete fKnobQ2;
    delete fKnobFreq2;
    delete fKnobGainL;
    delete fKnobFreqL;
    delete fKnobGainH;
    delete fKnobFreqH;
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamEQ2UI::d_parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamEQ2Plugin::paramGain1:
        fKnobGain1->setValue(value);
        break;
    case ZamEQ2Plugin::paramQ1:
        fKnobQ1->setValue(value);
        break;
    case ZamEQ2Plugin::paramFreq1:
        fKnobFreq1->setValue(value);
        break;
    case ZamEQ2Plugin::paramGain2:
        fKnobGain2->setValue(value);
        break;
    case ZamEQ2Plugin::paramQ2:
        fKnobQ2->setValue(value);
        break;
    case ZamEQ2Plugin::paramFreq2:
        fKnobFreq2->setValue(value);
        break;
    case ZamEQ2Plugin::paramGainL:
        fKnobGainL->setValue(value);
        break;
    case ZamEQ2Plugin::paramFreqL:
        fKnobFreqL->setValue(value);
        break;
    case ZamEQ2Plugin::paramGainH:
        fKnobGainH->setValue(value);
        break;
    case ZamEQ2Plugin::paramFreqH:
        fKnobFreqH->setValue(value);
        break;
    }
}

void ZamEQ2UI::d_programChanged(uint32_t index)
{
    if (index != 0)
        return;

    // Default values
    fKnobGain1->setValue(0.0f);
    fKnobQ1->setValue(1.0f);
    fKnobFreq1->setValue(500.0f);
    fKnobGain2->setValue(0.0f);
    fKnobQ2->setValue(1.0f);
    fKnobFreq2->setValue(3000.0f);
    fKnobGainL->setValue(0.0f);
    fKnobFreqL->setValue(250.0f);
    fKnobGainH->setValue(0.0f);
    fKnobFreqH->setValue(8000.0f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamEQ2UI::imageKnobDragStarted(ImageKnob* knob)
{
    if (knob == fKnobGain1)
        d_editParameter(ZamEQ2Plugin::paramGain1, true);
    else if (knob == fKnobQ1)
        d_editParameter(ZamEQ2Plugin::paramQ1, true);
    else if (knob == fKnobFreq1)
        d_editParameter(ZamEQ2Plugin::paramFreq1, true);
    else if (knob == fKnobGain2)
        d_editParameter(ZamEQ2Plugin::paramGain2, true);
    else if (knob == fKnobQ2)
        d_editParameter(ZamEQ2Plugin::paramQ2, true);
    else if (knob == fKnobFreq2)
        d_editParameter(ZamEQ2Plugin::paramFreq2, true);
    else if (knob == fKnobGainL)
        d_editParameter(ZamEQ2Plugin::paramGainL, true);
    else if (knob == fKnobFreqL)
        d_editParameter(ZamEQ2Plugin::paramFreqL, true);
    else if (knob == fKnobGainH)
        d_editParameter(ZamEQ2Plugin::paramGainH, true);
    else if (knob == fKnobFreqH)
        d_editParameter(ZamEQ2Plugin::paramFreqH, true);
}

void ZamEQ2UI::imageKnobDragFinished(ImageKnob* knob)
{
    if (knob == fKnobGain1)
        d_editParameter(ZamEQ2Plugin::paramGain1, false);
    else if (knob == fKnobQ1)
        d_editParameter(ZamEQ2Plugin::paramQ1, false);
    else if (knob == fKnobFreq1)
        d_editParameter(ZamEQ2Plugin::paramFreq1, false);
    else if (knob == fKnobGain2)
        d_editParameter(ZamEQ2Plugin::paramGain2, false);
    else if (knob == fKnobQ2)
        d_editParameter(ZamEQ2Plugin::paramQ2, false);
    else if (knob == fKnobFreq2)
        d_editParameter(ZamEQ2Plugin::paramFreq2, false);
    else if (knob == fKnobGainL)
        d_editParameter(ZamEQ2Plugin::paramGainL, false);
    else if (knob == fKnobFreqL)
        d_editParameter(ZamEQ2Plugin::paramFreqL, false);
    else if (knob == fKnobGainH)
        d_editParameter(ZamEQ2Plugin::paramGainH, false);
    else if (knob == fKnobFreqH)
        d_editParameter(ZamEQ2Plugin::paramFreqH, false);
}

void ZamEQ2UI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    if (knob == fKnobGain1)
        d_setParameterValue(ZamEQ2Plugin::paramGain1, value);
    else if (knob == fKnobQ1)
        d_setParameterValue(ZamEQ2Plugin::paramQ1, value);
    else if (knob == fKnobFreq1)
        d_setParameterValue(ZamEQ2Plugin::paramFreq1, value);
    else if (knob == fKnobGain2)
        d_setParameterValue(ZamEQ2Plugin::paramGain2, value);
    else if (knob == fKnobQ2)
        d_setParameterValue(ZamEQ2Plugin::paramQ2, value);
    else if (knob == fKnobFreq2)
        d_setParameterValue(ZamEQ2Plugin::paramFreq2, value);
    else if (knob == fKnobGainL)
        d_setParameterValue(ZamEQ2Plugin::paramGainL, value);
    else if (knob == fKnobFreqL)
        d_setParameterValue(ZamEQ2Plugin::paramFreqL, value);
    else if (knob == fKnobGainH)
        d_setParameterValue(ZamEQ2Plugin::paramGainH, value);
    else if (knob == fKnobFreqH)
        d_setParameterValue(ZamEQ2Plugin::paramFreqH, value);
}

void ZamEQ2UI::onDisplay()
{
    fImgBackground.draw();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamEQ2UI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
