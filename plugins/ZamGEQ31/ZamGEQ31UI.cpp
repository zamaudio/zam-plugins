/*
 * ZamGEQ31 31 band graphic equaliser
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
 */

#include "ZamGEQ31UI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamGEQ31UI::ZamGEQ31UI()
    : UI()
{
    setSize(ZamGEQ31Artwork::zamgeq31Width, ZamGEQ31Artwork::zamgeq31Height);

    // background
    fImgBackground = Image(ZamGEQ31Artwork::zamgeq31Data, ZamGEQ31Artwork::zamgeq31Width, ZamGEQ31Artwork::zamgeq31Height, GL_BGR);

    // slider image
    Image sliderImage(ZamGEQ31Artwork::slidereqData, ZamGEQ31Artwork::slidereqWidth, ZamGEQ31Artwork::slidereqHeight);

    // knob
    Image knobImage(ZamGEQ31Artwork::knobData, ZamGEQ31Artwork::knobWidth, ZamGEQ31Artwork::knobHeight);

    // master knob
    fKnobMaster = new ImageKnob(this, knobImage);
    fKnobMaster->setAbsolutePos(103, 122);
    fKnobMaster->setRange(-20.f, 20.f);
    fKnobMaster->setLabel(true);
    fKnobMaster->setScrollStep(1.0f);
    fKnobMaster->setRotationAngle(240);
    fKnobMaster->setDefault(0.0f);
    fKnobMaster->setCallback(this);

    float i,j;
    i = 24.;
    j = 18.8;
    Point<int> masterPosStart(i,18);
    Point<int> masterPosEnd(i,95);

    fSliderGain1 = new ImageSlider(this, sliderImage);
    fSliderGain1->setStartPos(masterPosStart);
    fSliderGain1->setEndPos(masterPosEnd);
    fSliderGain1->setRange(-12.f,12.f);
    fSliderGain1->setValue(0.f);
    fSliderGain1->setCallback(this);
    fSliderGain1->setInverted(true);

    masterPosStart.setX(i+j);
    masterPosEnd.setX(i+j);

    fSliderGain2 = new ImageSlider(this, sliderImage);
    fSliderGain2->setStartPos(masterPosStart);
    fSliderGain2->setEndPos(masterPosEnd);
    fSliderGain2->setRange(-12.f,12.f);
    fSliderGain2->setValue(0.f);
    fSliderGain2->setCallback(this);
    fSliderGain2->setInverted(true);

    masterPosStart.setX(i+2*j);
    masterPosEnd.setX(i+2*j);

    fSliderGain3 = new ImageSlider(this, sliderImage);
    fSliderGain3->setStartPos(masterPosStart);
    fSliderGain3->setEndPos(masterPosEnd);
    fSliderGain3->setRange(-12.f,12.f);
    fSliderGain3->setValue(0.f);
    fSliderGain3->setCallback(this);
    fSliderGain3->setInverted(true);

    masterPosStart.setX(i+3*j);
    masterPosEnd.setX(i+3*j);

    fSliderGain4 = new ImageSlider(this, sliderImage);
    fSliderGain4->setStartPos(masterPosStart);
    fSliderGain4->setEndPos(masterPosEnd);
    fSliderGain4->setRange(-12.f,12.f);
    fSliderGain4->setValue(0.f);
    fSliderGain4->setCallback(this);
    fSliderGain4->setInverted(true);

    masterPosStart.setX(i+4*j);
    masterPosEnd.setX(i+4*j);

    fSliderGain5 = new ImageSlider(this, sliderImage);
    fSliderGain5->setStartPos(masterPosStart);
    fSliderGain5->setEndPos(masterPosEnd);
    fSliderGain5->setRange(-12.f,12.f);
    fSliderGain5->setValue(0.f);
    fSliderGain5->setCallback(this);
    fSliderGain5->setInverted(true);

    masterPosStart.setX(i+5*j);
    masterPosEnd.setX(i+5*j);

    fSliderGain6 = new ImageSlider(this, sliderImage);
    fSliderGain6->setStartPos(masterPosStart);
    fSliderGain6->setEndPos(masterPosEnd);
    fSliderGain6->setRange(-12.f,12.f);
    fSliderGain6->setValue(0.f);
    fSliderGain6->setCallback(this);
    fSliderGain6->setInverted(true);

    masterPosStart.setX(i+6*j);
    masterPosEnd.setX(i+6*j);

    fSliderGain7 = new ImageSlider(this, sliderImage);
    fSliderGain7->setStartPos(masterPosStart);
    fSliderGain7->setEndPos(masterPosEnd);
    fSliderGain7->setRange(-12.f,12.f);
    fSliderGain7->setValue(0.f);
    fSliderGain7->setCallback(this);
    fSliderGain7->setInverted(true);

    masterPosStart.setX(i+7*j);
    masterPosEnd.setX(i+7*j);

    fSliderGain8 = new ImageSlider(this, sliderImage);
    fSliderGain8->setStartPos(masterPosStart);
    fSliderGain8->setEndPos(masterPosEnd);
    fSliderGain8->setRange(-12.f,12.f);
    fSliderGain8->setValue(0.f);
    fSliderGain8->setCallback(this);
    fSliderGain8->setInverted(true);

    masterPosStart.setX(i+8*j);
    masterPosEnd.setX(i+8*j);

    fSliderGain9 = new ImageSlider(this, sliderImage);
    fSliderGain9->setStartPos(masterPosStart);
    fSliderGain9->setEndPos(masterPosEnd);
    fSliderGain9->setRange(-12.f,12.f);
    fSliderGain9->setValue(0.f);
    fSliderGain9->setCallback(this);
    fSliderGain9->setInverted(true);

    masterPosStart.setX(i+9*j);
    masterPosEnd.setX(i+9*j);

    fSliderGain10 = new ImageSlider(this, sliderImage);
    fSliderGain10->setStartPos(masterPosStart);
    fSliderGain10->setEndPos(masterPosEnd);
    fSliderGain10->setRange(-12.f,12.f);
    fSliderGain10->setValue(0.f);
    fSliderGain10->setCallback(this);
    fSliderGain10->setInverted(true);

    masterPosStart.setX(i+10*j);
    masterPosEnd.setX(i+10*j);

    fSliderGain11 = new ImageSlider(this, sliderImage);
    fSliderGain11->setStartPos(masterPosStart);
    fSliderGain11->setEndPos(masterPosEnd);
    fSliderGain11->setRange(-12.f,12.f);
    fSliderGain11->setValue(0.f);
    fSliderGain11->setCallback(this);
    fSliderGain11->setInverted(true);

    masterPosStart.setX(i+11*j);
    masterPosEnd.setX(i+11*j);

    fSliderGain12 = new ImageSlider(this, sliderImage);
    fSliderGain12->setStartPos(masterPosStart);
    fSliderGain12->setEndPos(masterPosEnd);
    fSliderGain12->setRange(-12.f,12.f);
    fSliderGain12->setValue(0.f);
    fSliderGain12->setCallback(this);
    fSliderGain12->setInverted(true);

    masterPosStart.setX(i+12*j);
    masterPosEnd.setX(i+12*j);

    fSliderGain13 = new ImageSlider(this, sliderImage);
    fSliderGain13->setStartPos(masterPosStart);
    fSliderGain13->setEndPos(masterPosEnd);
    fSliderGain13->setRange(-12.f,12.f);
    fSliderGain13->setValue(0.f);
    fSliderGain13->setCallback(this);
    fSliderGain13->setInverted(true);

    masterPosStart.setX(i+13*j);
    masterPosEnd.setX(i+13*j);

    fSliderGain14 = new ImageSlider(this, sliderImage);
    fSliderGain14->setStartPos(masterPosStart);
    fSliderGain14->setEndPos(masterPosEnd);
    fSliderGain14->setRange(-12.f,12.f);
    fSliderGain14->setValue(0.f);
    fSliderGain14->setCallback(this);
    fSliderGain14->setInverted(true);

    masterPosStart.setX(i+14*j);
    masterPosEnd.setX(i+14*j);

    fSliderGain15 = new ImageSlider(this, sliderImage);
    fSliderGain15->setStartPos(masterPosStart);
    fSliderGain15->setEndPos(masterPosEnd);
    fSliderGain15->setRange(-12.f,12.f);
    fSliderGain15->setValue(0.f);
    fSliderGain15->setCallback(this);
    fSliderGain15->setInverted(true);

    masterPosStart.setX(i+15*j);
    masterPosEnd.setX(i+15*j);

    fSliderGain16 = new ImageSlider(this, sliderImage);
    fSliderGain16->setStartPos(masterPosStart);
    fSliderGain16->setEndPos(masterPosEnd);
    fSliderGain16->setRange(-12.f,12.f);
    fSliderGain16->setValue(0.f);
    fSliderGain16->setCallback(this);
    fSliderGain16->setInverted(true);

    masterPosStart.setX(i+16*j);
    masterPosEnd.setX(i+16*j);

    fSliderGain17 = new ImageSlider(this, sliderImage);
    fSliderGain17->setStartPos(masterPosStart);
    fSliderGain17->setEndPos(masterPosEnd);
    fSliderGain17->setRange(-12.f,12.f);
    fSliderGain17->setValue(0.f);
    fSliderGain17->setCallback(this);
    fSliderGain17->setInverted(true);

    masterPosStart.setX(i+17*j);
    masterPosEnd.setX(i+17*j);

    fSliderGain18 = new ImageSlider(this, sliderImage);
    fSliderGain18->setStartPos(masterPosStart);
    fSliderGain18->setEndPos(masterPosEnd);
    fSliderGain18->setRange(-12.f,12.f);
    fSliderGain18->setValue(0.f);
    fSliderGain18->setCallback(this);
    fSliderGain18->setInverted(true);

    masterPosStart.setX(i+18*j);
    masterPosEnd.setX(i+18*j);

    fSliderGain19 = new ImageSlider(this, sliderImage);
    fSliderGain19->setStartPos(masterPosStart);
    fSliderGain19->setEndPos(masterPosEnd);
    fSliderGain19->setRange(-12.f,12.f);
    fSliderGain19->setValue(0.f);
    fSliderGain19->setCallback(this);
    fSliderGain19->setInverted(true);

    masterPosStart.setX(i+19*j);
    masterPosEnd.setX(i+19*j);

    fSliderGain20 = new ImageSlider(this, sliderImage);
    fSliderGain20->setStartPos(masterPosStart);
    fSliderGain20->setEndPos(masterPosEnd);
    fSliderGain20->setRange(-12.f,12.f);
    fSliderGain20->setValue(0.f);
    fSliderGain20->setCallback(this);
    fSliderGain20->setInverted(true);

    masterPosStart.setX(i+20*j);
    masterPosEnd.setX(i+20*j);

    fSliderGain21 = new ImageSlider(this, sliderImage);
    fSliderGain21->setStartPos(masterPosStart);
    fSliderGain21->setEndPos(masterPosEnd);
    fSliderGain21->setRange(-12.f,12.f);
    fSliderGain21->setValue(0.f);
    fSliderGain21->setCallback(this);
    fSliderGain21->setInverted(true);

    masterPosStart.setX(i+21*j);
    masterPosEnd.setX(i+21*j);

    fSliderGain22 = new ImageSlider(this, sliderImage);
    fSliderGain22->setStartPos(masterPosStart);
    fSliderGain22->setEndPos(masterPosEnd);
    fSliderGain22->setRange(-12.f,12.f);
    fSliderGain22->setValue(0.f);
    fSliderGain22->setCallback(this);
    fSliderGain22->setInverted(true);

    masterPosStart.setX(i+22*j);
    masterPosEnd.setX(i+22*j);

    fSliderGain23 = new ImageSlider(this, sliderImage);
    fSliderGain23->setStartPos(masterPosStart);
    fSliderGain23->setEndPos(masterPosEnd);
    fSliderGain23->setRange(-12.f,12.f);
    fSliderGain23->setValue(0.f);
    fSliderGain23->setCallback(this);
    fSliderGain23->setInverted(true);

    masterPosStart.setX(i+23*j);
    masterPosEnd.setX(i+23*j);

    fSliderGain24 = new ImageSlider(this, sliderImage);
    fSliderGain24->setStartPos(masterPosStart);
    fSliderGain24->setEndPos(masterPosEnd);
    fSliderGain24->setRange(-12.f,12.f);
    fSliderGain24->setValue(0.f);
    fSliderGain24->setCallback(this);
    fSliderGain24->setInverted(true);

    masterPosStart.setX(i+24*j);
    masterPosEnd.setX(i+24*j);

    fSliderGain25 = new ImageSlider(this, sliderImage);
    fSliderGain25->setStartPos(masterPosStart);
    fSliderGain25->setEndPos(masterPosEnd);
    fSliderGain25->setRange(-12.f,12.f);
    fSliderGain25->setValue(0.f);
    fSliderGain25->setCallback(this);
    fSliderGain25->setInverted(true);

    masterPosStart.setX(i+25*j);
    masterPosEnd.setX(i+25*j);

    fSliderGain26 = new ImageSlider(this, sliderImage);
    fSliderGain26->setStartPos(masterPosStart);
    fSliderGain26->setEndPos(masterPosEnd);
    fSliderGain26->setRange(-12.f,12.f);
    fSliderGain26->setValue(0.f);
    fSliderGain26->setCallback(this);
    fSliderGain26->setInverted(true);

    masterPosStart.setX(i+26*j);
    masterPosEnd.setX(i+26*j);

    fSliderGain27 = new ImageSlider(this, sliderImage);
    fSliderGain27->setStartPos(masterPosStart);
    fSliderGain27->setEndPos(masterPosEnd);
    fSliderGain27->setRange(-12.f,12.f);
    fSliderGain27->setValue(0.f);
    fSliderGain27->setCallback(this);
    fSliderGain27->setInverted(true);

    masterPosStart.setX(i+27*j);
    masterPosEnd.setX(i+27*j);

    fSliderGain28 = new ImageSlider(this, sliderImage);
    fSliderGain28->setStartPos(masterPosStart);
    fSliderGain28->setEndPos(masterPosEnd);
    fSliderGain28->setRange(-12.f,12.f);
    fSliderGain28->setValue(0.f);
    fSliderGain28->setCallback(this);
    fSliderGain28->setInverted(true);

    masterPosStart.setX(i+28*j);
    masterPosEnd.setX(i+28*j);

    fSliderGain29 = new ImageSlider(this, sliderImage);
    fSliderGain29->setStartPos(masterPosStart);
    fSliderGain29->setEndPos(masterPosEnd);
    fSliderGain29->setRange(-12.f,12.f);
    fSliderGain29->setValue(0.f);
    fSliderGain29->setCallback(this);
    fSliderGain29->setInverted(true);

    // set default values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamGEQ31UI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamGEQ31Plugin::paramMaster:
    	fKnobMaster->setValue(value);
	break;
    case ZamGEQ31Plugin::paramGain1:
        fSliderGain1->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain2:
        fSliderGain2->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain3:
        fSliderGain3->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain4:
        fSliderGain4->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain5:
        fSliderGain5->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain6:
        fSliderGain6->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain7:
        fSliderGain7->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain8:
        fSliderGain8->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain9:
        fSliderGain9->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain10:
        fSliderGain10->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain11:
        fSliderGain11->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain12:
        fSliderGain12->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain13:
        fSliderGain13->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain14:
        fSliderGain14->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain15:
        fSliderGain15->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain16:
        fSliderGain16->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain17:
        fSliderGain17->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain18:
        fSliderGain18->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain19:
        fSliderGain19->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain20:
        fSliderGain20->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain21:
        fSliderGain21->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain22:
        fSliderGain22->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain23:
        fSliderGain23->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain24:
        fSliderGain24->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain25:
        fSliderGain25->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain26:
        fSliderGain26->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain27:
        fSliderGain27->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain28:
        fSliderGain28->setValue(value);
        break;
    case ZamGEQ31Plugin::paramGain29:
        fSliderGain29->setValue(value);
        break;
    }
}

void ZamGEQ31UI::programLoaded(uint32_t index)
{
    if (index != 0)
        return;

    // Default values

    fKnobMaster->setValue(0.f);
    fSliderGain1->setValue(0.0f);
    fSliderGain2->setValue(0.0f);
    fSliderGain3->setValue(0.0f);
    fSliderGain4->setValue(0.0f);
    fSliderGain5->setValue(0.0f);
    fSliderGain6->setValue(0.0f);
    fSliderGain7->setValue(0.0f);
    fSliderGain8->setValue(0.0f);
    fSliderGain9->setValue(0.0f);
    fSliderGain10->setValue(0.0f);
    fSliderGain11->setValue(0.0f);
    fSliderGain12->setValue(0.0f);
    fSliderGain13->setValue(0.0f);
    fSliderGain14->setValue(0.0f);
    fSliderGain15->setValue(0.0f);
    fSliderGain16->setValue(0.0f);
    fSliderGain17->setValue(0.0f);
    fSliderGain18->setValue(0.0f);
    fSliderGain19->setValue(0.0f);
    fSliderGain20->setValue(0.0f);
    fSliderGain21->setValue(0.0f);
    fSliderGain22->setValue(0.0f);
    fSliderGain23->setValue(0.0f);
    fSliderGain24->setValue(0.0f);
    fSliderGain25->setValue(0.0f);
    fSliderGain26->setValue(0.0f);
    fSliderGain27->setValue(0.0f);
    fSliderGain28->setValue(0.0f);
    fSliderGain29->setValue(0.0f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamGEQ31UI::imageKnobDragStarted(ImageKnob* knob)
{
    if (knob == fKnobMaster)
        editParameter(ZamGEQ31Plugin::paramMaster, true);
}

void ZamGEQ31UI::imageKnobDragFinished(ImageKnob* knob)
{
    if (knob == fKnobMaster)
        editParameter(ZamGEQ31Plugin::paramMaster, false);
}

void ZamGEQ31UI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    if (knob == fKnobMaster)
        setParameterValue(ZamGEQ31Plugin::paramMaster, value);
}

void ZamGEQ31UI::imageSliderDragStarted(ImageSlider* slider)
{
    if (slider == fSliderGain1)
        editParameter(ZamGEQ31Plugin::paramGain1, true);
    else if (slider == fSliderGain2)
        editParameter(ZamGEQ31Plugin::paramGain2, true);
    else if (slider == fSliderGain3)
        editParameter(ZamGEQ31Plugin::paramGain3, true);
    else if (slider == fSliderGain4)
        editParameter(ZamGEQ31Plugin::paramGain4, true);
    else if (slider == fSliderGain5)
        editParameter(ZamGEQ31Plugin::paramGain5, true);
    else if (slider == fSliderGain6)
        editParameter(ZamGEQ31Plugin::paramGain6, true);
    else if (slider == fSliderGain7)
        editParameter(ZamGEQ31Plugin::paramGain7, true);
    else if (slider == fSliderGain8)
        editParameter(ZamGEQ31Plugin::paramGain8, true);
    else if (slider == fSliderGain9)
        editParameter(ZamGEQ31Plugin::paramGain9, true);
    else if (slider == fSliderGain10)
        editParameter(ZamGEQ31Plugin::paramGain10, true);
    else if (slider == fSliderGain11)
        editParameter(ZamGEQ31Plugin::paramGain11, true);
    else if (slider == fSliderGain12)
        editParameter(ZamGEQ31Plugin::paramGain12, true);
    else if (slider == fSliderGain13)
        editParameter(ZamGEQ31Plugin::paramGain13, true);
    else if (slider == fSliderGain14)
        editParameter(ZamGEQ31Plugin::paramGain14, true);
    else if (slider == fSliderGain15)
        editParameter(ZamGEQ31Plugin::paramGain15, true);
    else if (slider == fSliderGain16)
        editParameter(ZamGEQ31Plugin::paramGain16, true);
    else if (slider == fSliderGain17)
        editParameter(ZamGEQ31Plugin::paramGain17, true);
    else if (slider == fSliderGain18)
        editParameter(ZamGEQ31Plugin::paramGain18, true);
    else if (slider == fSliderGain19)
        editParameter(ZamGEQ31Plugin::paramGain19, true);
    else if (slider == fSliderGain20)
        editParameter(ZamGEQ31Plugin::paramGain20, true);
    else if (slider == fSliderGain21)
        editParameter(ZamGEQ31Plugin::paramGain21, true);
    else if (slider == fSliderGain22)
        editParameter(ZamGEQ31Plugin::paramGain22, true);
    else if (slider == fSliderGain23)
        editParameter(ZamGEQ31Plugin::paramGain23, true);
    else if (slider == fSliderGain24)
        editParameter(ZamGEQ31Plugin::paramGain24, true);
    else if (slider == fSliderGain25)
        editParameter(ZamGEQ31Plugin::paramGain25, true);
    else if (slider == fSliderGain26)
        editParameter(ZamGEQ31Plugin::paramGain26, true);
    else if (slider == fSliderGain27)
        editParameter(ZamGEQ31Plugin::paramGain27, true);
    else if (slider == fSliderGain28)
        editParameter(ZamGEQ31Plugin::paramGain28, true);
    else if (slider == fSliderGain29)
        editParameter(ZamGEQ31Plugin::paramGain29, true);
}

void ZamGEQ31UI::imageSliderDragFinished(ImageSlider* slider)
{
    if (slider == fSliderGain1)
        editParameter(ZamGEQ31Plugin::paramGain1, false);
    else if (slider == fSliderGain2)
        editParameter(ZamGEQ31Plugin::paramGain2, false);
    else if (slider == fSliderGain3)
        editParameter(ZamGEQ31Plugin::paramGain3, false);
    else if (slider == fSliderGain4)
        editParameter(ZamGEQ31Plugin::paramGain4, false);
    else if (slider == fSliderGain5)
        editParameter(ZamGEQ31Plugin::paramGain5, false);
    else if (slider == fSliderGain6)
        editParameter(ZamGEQ31Plugin::paramGain6, false);
    else if (slider == fSliderGain7)
        editParameter(ZamGEQ31Plugin::paramGain7, false);
    else if (slider == fSliderGain8)
        editParameter(ZamGEQ31Plugin::paramGain8, false);
    else if (slider == fSliderGain9)
        editParameter(ZamGEQ31Plugin::paramGain9, false);
    else if (slider == fSliderGain10)
        editParameter(ZamGEQ31Plugin::paramGain10, false);
    else if (slider == fSliderGain11)
        editParameter(ZamGEQ31Plugin::paramGain11, false);
    else if (slider == fSliderGain12)
        editParameter(ZamGEQ31Plugin::paramGain12, false);
    else if (slider == fSliderGain13)
        editParameter(ZamGEQ31Plugin::paramGain13, false);
    else if (slider == fSliderGain14)
        editParameter(ZamGEQ31Plugin::paramGain14, false);
    else if (slider == fSliderGain15)
        editParameter(ZamGEQ31Plugin::paramGain15, false);
    else if (slider == fSliderGain16)
        editParameter(ZamGEQ31Plugin::paramGain16, false);
    else if (slider == fSliderGain17)
        editParameter(ZamGEQ31Plugin::paramGain17, false);
    else if (slider == fSliderGain18)
        editParameter(ZamGEQ31Plugin::paramGain18, false);
    else if (slider == fSliderGain19)
        editParameter(ZamGEQ31Plugin::paramGain19, false);
    else if (slider == fSliderGain20)
        editParameter(ZamGEQ31Plugin::paramGain20, false);
    else if (slider == fSliderGain21)
        editParameter(ZamGEQ31Plugin::paramGain21, false);
    else if (slider == fSliderGain22)
        editParameter(ZamGEQ31Plugin::paramGain22, false);
    else if (slider == fSliderGain23)
        editParameter(ZamGEQ31Plugin::paramGain23, false);
    else if (slider == fSliderGain24)
        editParameter(ZamGEQ31Plugin::paramGain24, false);
    else if (slider == fSliderGain25)
        editParameter(ZamGEQ31Plugin::paramGain25, false);
    else if (slider == fSliderGain26)
        editParameter(ZamGEQ31Plugin::paramGain26, false);
    else if (slider == fSliderGain27)
        editParameter(ZamGEQ31Plugin::paramGain27, false);
    else if (slider == fSliderGain28)
        editParameter(ZamGEQ31Plugin::paramGain28, false);
    else if (slider == fSliderGain29)
        editParameter(ZamGEQ31Plugin::paramGain29, false);
}

void ZamGEQ31UI::imageSliderValueChanged(ImageSlider* slider, float value)
{
    if (slider == fSliderGain1)
        setParameterValue(ZamGEQ31Plugin::paramGain1, value);
    else if (slider == fSliderGain2)
        setParameterValue(ZamGEQ31Plugin::paramGain2, value);
    else if (slider == fSliderGain3)
        setParameterValue(ZamGEQ31Plugin::paramGain3, value);
    else if (slider == fSliderGain4)
        setParameterValue(ZamGEQ31Plugin::paramGain4, value);
    else if (slider == fSliderGain5)
        setParameterValue(ZamGEQ31Plugin::paramGain5, value);
    else if (slider == fSliderGain6)
        setParameterValue(ZamGEQ31Plugin::paramGain6, value);
    else if (slider == fSliderGain7)
        setParameterValue(ZamGEQ31Plugin::paramGain7, value);
    else if (slider == fSliderGain8)
        setParameterValue(ZamGEQ31Plugin::paramGain8, value);
    else if (slider == fSliderGain9)
        setParameterValue(ZamGEQ31Plugin::paramGain9, value);
    else if (slider == fSliderGain10)
        setParameterValue(ZamGEQ31Plugin::paramGain10, value);
    else if (slider == fSliderGain11)
        setParameterValue(ZamGEQ31Plugin::paramGain11, value);
    else if (slider == fSliderGain12)
        setParameterValue(ZamGEQ31Plugin::paramGain12, value);
    else if (slider == fSliderGain13)
        setParameterValue(ZamGEQ31Plugin::paramGain13, value);
    else if (slider == fSliderGain14)
        setParameterValue(ZamGEQ31Plugin::paramGain14, value);
    else if (slider == fSliderGain15)
        setParameterValue(ZamGEQ31Plugin::paramGain15, value);
    else if (slider == fSliderGain16)
        setParameterValue(ZamGEQ31Plugin::paramGain16, value);
    else if (slider == fSliderGain17)
        setParameterValue(ZamGEQ31Plugin::paramGain17, value);
    else if (slider == fSliderGain18)
        setParameterValue(ZamGEQ31Plugin::paramGain18, value);
    else if (slider == fSliderGain19)
        setParameterValue(ZamGEQ31Plugin::paramGain19, value);
    else if (slider == fSliderGain20)
        setParameterValue(ZamGEQ31Plugin::paramGain20, value);
    else if (slider == fSliderGain21)
        setParameterValue(ZamGEQ31Plugin::paramGain21, value);
    else if (slider == fSliderGain22)
        setParameterValue(ZamGEQ31Plugin::paramGain22, value);
    else if (slider == fSliderGain23)
        setParameterValue(ZamGEQ31Plugin::paramGain23, value);
    else if (slider == fSliderGain24)
        setParameterValue(ZamGEQ31Plugin::paramGain24, value);
    else if (slider == fSliderGain25)
        setParameterValue(ZamGEQ31Plugin::paramGain25, value);
    else if (slider == fSliderGain26)
        setParameterValue(ZamGEQ31Plugin::paramGain26, value);
    else if (slider == fSliderGain27)
        setParameterValue(ZamGEQ31Plugin::paramGain27, value);
    else if (slider == fSliderGain28)
        setParameterValue(ZamGEQ31Plugin::paramGain28, value);
    else if (slider == fSliderGain29)
        setParameterValue(ZamGEQ31Plugin::paramGain29, value);
}

void ZamGEQ31UI::onDisplay()
{
    fImgBackground.draw();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamGEQ31UI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
