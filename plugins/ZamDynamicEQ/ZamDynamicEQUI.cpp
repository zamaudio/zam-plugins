/*
 * ZamDynamicEQ
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

#include "ZamDynamicEQPlugin.hpp"
#include "ZamDynamicEQUI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamDynamicEQUI::ZamDynamicEQUI()
    : UI()
{
    setSize(ZamDynamicEQArtwork::zamdynamiceqWidth, ZamDynamicEQArtwork::zamdynamiceqHeight);

    // background
    fImgBackground = Image(ZamDynamicEQArtwork::zamdynamiceqData, ZamDynamicEQArtwork::zamdynamiceqWidth, ZamDynamicEQArtwork::zamdynamiceqHeight, kImageFormatBGR);

    // images
    fHighOnImg = Image(ZamDynamicEQArtwork::highonData, ZamDynamicEQArtwork::highonWidth, ZamDynamicEQArtwork::highonHeight);
    fHighOffImg = Image(ZamDynamicEQArtwork::highoffData, ZamDynamicEQArtwork::highoffWidth, ZamDynamicEQArtwork::highoffHeight);
    fLowOnImg = Image(ZamDynamicEQArtwork::lowonData, ZamDynamicEQArtwork::lowonWidth, ZamDynamicEQArtwork::lowonHeight);
    fLowOffImg = Image(ZamDynamicEQArtwork::lowoffData, ZamDynamicEQArtwork::lowoffWidth, ZamDynamicEQArtwork::lowoffHeight);
    fPeakOnImg = Image(ZamDynamicEQArtwork::peakonData, ZamDynamicEQArtwork::peakonWidth, ZamDynamicEQArtwork::peakonHeight);
    fPeakOffImg = Image(ZamDynamicEQArtwork::peakoffData, ZamDynamicEQArtwork::peakoffWidth, ZamDynamicEQArtwork::peakoffHeight);
    fTogOffImg = Image(ZamDynamicEQArtwork::toggleoffhorizData, ZamDynamicEQArtwork::toggleoffhorizWidth, ZamDynamicEQArtwork::toggleoffhorizHeight);
    fTogOnImg = Image(ZamDynamicEQArtwork::toggleonhorizData, ZamDynamicEQArtwork::toggleonhorizWidth, ZamDynamicEQArtwork::toggleonhorizHeight);

    Image knobImage(ZamDynamicEQArtwork::knobData, ZamDynamicEQArtwork::knobWidth, ZamDynamicEQArtwork::knobHeight);

    float i,j;
    i = 24.;
    j = 18.8;
    float yy = -64.;

    Point<int> togglePosStart(20,135+yy);

    fToggleLow = new ImageButton(this, fLowOffImg, fLowOnImg);
    fToggleLow->setAbsolutePos(togglePosStart);
    fToggleLow->setId(ZamDynamicEQPlugin::paramToggleLow);
    fToggleLow->setCallback(this);
    fToggleLow->setCheckable(true);

    togglePosStart.setY(172+yy);

    fTogglePeak = new ImageButton(this, fPeakOffImg, fPeakOnImg);
    fTogglePeak->setAbsolutePos(togglePosStart);
    fTogglePeak->setId(ZamDynamicEQPlugin::paramTogglePeak);
    fTogglePeak->setCallback(this);
    fTogglePeak->setCheckable(true);

    togglePosStart.setY(209+yy);

    fToggleHigh = new ImageButton(this, fHighOffImg, fHighOnImg);
    fToggleHigh->setAbsolutePos(togglePosStart);
    fToggleHigh->setId(ZamDynamicEQPlugin::paramToggleHigh);
    fToggleHigh->setCallback(this);
    fToggleHigh->setCheckable(true);

    togglePosStart.setX(278-230+4);
    togglePosStart.setY(113+130+5+yy);

    fCanvasArea.setPos(8, 248+yy);
    fCanvasArea.setSize(575, 150);

    i = 82;
    j = 150;

    fKnobAttack = new ZamKnob(this, knobImage);
    fKnobAttack->setAbsolutePos(24+i, 45+j+yy);
    fKnobAttack->setId(ZamDynamicEQPlugin::paramAttack);
    fKnobAttack->setRange(0.1f, 200.0f);
    fKnobAttack->setUsingLogScale(true);
    fKnobAttack->setLabel(true);
    fKnobAttack->setScrollStep(1.0f);
    fKnobAttack->setDefault(10.0f);
    fKnobAttack->setRotationAngle(240);
    fKnobAttack->setCallback(this);

    fKnobRelease = new ZamKnob(this, knobImage);
    fKnobRelease->setAbsolutePos(108+i, 45+j+yy);
    fKnobRelease->setId(ZamDynamicEQPlugin::paramRelease);
    fKnobRelease->setRange(1.0f, 500.0f);
    fKnobRelease->setLabel(true);
    fKnobRelease->setScrollStep(1.0f);
    fKnobRelease->setDefault(80.0f);
    fKnobRelease->setRotationAngle(240);
    fKnobRelease->setCallback(this);

    fKnobThresh = new ZamKnob(this, knobImage);
    fKnobThresh->setAbsolutePos(191.5+i, 45+j+yy);
    fKnobThresh->setId(ZamDynamicEQPlugin::paramThresh);
    fKnobThresh->setRange(-60.0f, 0.0f);
    fKnobThresh->setLabel(true);
    fKnobThresh->setScrollStep(1.0f);
    fKnobThresh->setDefault(0.0f);
    fKnobThresh->setRotationAngle(240);
    fKnobThresh->setCallback(this);

    fKnobRatio = new ZamKnob(this, knobImage);
    fKnobRatio->setAbsolutePos(270+i, 45+j+yy);
    fKnobRatio->setId(ZamDynamicEQPlugin::paramRatio);
    fKnobRatio->setRange(1.0f, 5.0f);
    fKnobRatio->setScrollStep(1.0f);
    fKnobRatio->setLabel(true);
    fKnobRatio->setUsingLogScale(true);
    fKnobRatio->setDefault(1.5f);
    fKnobRatio->setRotationAngle(240);
    fKnobRatio->setCallback(this);

    fKnobKnee = new ZamKnob(this, knobImage);
    fKnobKnee->setAbsolutePos(348.5+i, 45+j+yy);
    fKnobKnee->setId(ZamDynamicEQPlugin::paramKnee);
    fKnobKnee->setRange(0.0f, 8.0f);
    fKnobKnee->setLabel(true);
    fKnobKnee->setScrollStep(1.0f);
    fKnobKnee->setDefault(0.0f);
    fKnobKnee->setRotationAngle(240);
    fKnobKnee->setCallback(this);

    fKnobMax = new ZamKnob(this, knobImage);
    fKnobMax->setAbsolutePos(382, 119+yy);
    fKnobMax->setId(ZamDynamicEQPlugin::paramMaxBoostCut);
    fKnobMax->setRange(0.0f, 10.0f);
    fKnobMax->setLabel(true);
    fKnobMax->setScrollStep(1.0f);
    fKnobMax->setDefault(10.0f);
    fKnobMax->setRotationAngle(240);
    fKnobMax->setCallback(this);

    fKnobSlew = new ZamKnob(this, knobImage);
    fKnobSlew->setAbsolutePos(427.3+i, 45+j+yy);
    fKnobSlew->setId(ZamDynamicEQPlugin::paramSlew);
    fKnobSlew->setRange(1.0f, 150.0f);
    fKnobSlew->setLabel(true);
    fKnobSlew->setScrollStep(1.0f);
    fKnobSlew->setDefault(1.0f);
    fKnobSlew->setRotationAngle(240);
    fKnobSlew->setCallback(this);

    fKnobTargetFreq = new ZamKnob(this, knobImage);
    fKnobTargetFreq->setAbsolutePos(206, 119+yy);
    fKnobTargetFreq->setId(ZamDynamicEQPlugin::paramTargetFreq);
    fKnobTargetFreq->setRange(20.0f, 16000.0f);
    fKnobTargetFreq->setLabel(true);
    fKnobTargetFreq->setUsingLogScale(true);
    fKnobTargetFreq->setScrollStep(10.0f);
    fKnobTargetFreq->setDefault(1000.0f);
    fKnobTargetFreq->setRotationAngle(240);
    fKnobTargetFreq->setCallback(this);

    fKnobTargetWidth = new ZamKnob(this, knobImage);
    fKnobTargetWidth->setAbsolutePos(282, 119+yy);
    fKnobTargetWidth->setId(ZamDynamicEQPlugin::paramTargetWidth);
    fKnobTargetWidth->setRange(1.0f, 5.0f);
    fKnobTargetWidth->setLabel(true);
    fKnobTargetWidth->setScrollStep(0.1f);
    fKnobTargetWidth->setDefault(1.0f);
    fKnobTargetWidth->setRotationAngle(240);
    fKnobTargetWidth->setCallback(this);

    fKnobDetectFreq = new ZamKnob(this, knobImage);
    fKnobDetectFreq->setAbsolutePos(105, 119+yy);
    fKnobDetectFreq->setId(ZamDynamicEQPlugin::paramDetectFreq);
    fKnobDetectFreq->setRange(20.0f, 16000.0f);
    fKnobDetectFreq->setLabel(true);
    fKnobDetectFreq->setUsingLogScale(true);
    fKnobDetectFreq->setScrollStep(10.0f);
    fKnobDetectFreq->setDefault(1000.0f);
    fKnobDetectFreq->setRotationAngle(240);
    fKnobDetectFreq->setCallback(this);

    fToggleBoostCut = new ImageSwitch(this, fTogOffImg, fTogOnImg);
    fToggleBoostCut->setAbsolutePos(475, 116+yy);
    fToggleBoostCut->setId(ZamDynamicEQPlugin::paramBoostCut);
    fToggleBoostCut->setCallback(this);

    fToggleSidechain = new ImageSwitch(this, fTogOffImg, fTogOnImg);
    fToggleSidechain->setAbsolutePos(475, 142+yy);
    fToggleSidechain->setId(ZamDynamicEQPlugin::paramSidechain);
    fToggleSidechain->setCallback(this);

    // set default values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamDynamicEQUI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamDynamicEQPlugin::paramAttack:
        fKnobAttack->setValue(value);
        break;
    case ZamDynamicEQPlugin::paramRelease:
        fKnobRelease->setValue(value);
        break;
    case ZamDynamicEQPlugin::paramThresh:
        fKnobThresh->setValue(value);
        break;
    case ZamDynamicEQPlugin::paramRatio:
        fKnobRatio->setValue(value);
        break;
    case ZamDynamicEQPlugin::paramKnee:
        fKnobKnee->setValue(value);
        break;
    case ZamDynamicEQPlugin::paramMaxBoostCut:
        fKnobMax->setValue(value);
        break;
    case ZamDynamicEQPlugin::paramSlew:
        fKnobSlew->setValue(value);
        break;
    case ZamDynamicEQPlugin::paramSidechain:
        fToggleSidechain->setDown(value > 0.5);
        break;
    case ZamDynamicEQPlugin::paramBoostCut:
        fToggleBoostCut->setDown(value > 0.5);
        break;
    case ZamDynamicEQPlugin::paramToggleLow:
        fToggleLow->setChecked(value > 0.5, false);
        break;
    case ZamDynamicEQPlugin::paramTogglePeak:
        fTogglePeak->setChecked(value > 0.5, false);
        break;
    case ZamDynamicEQPlugin::paramToggleHigh:
        fToggleHigh->setChecked(value > 0.5, false);
        break;
    case ZamDynamicEQPlugin::paramControlGain:
        fControlGain = value;
        repaint();
        break;
    case ZamDynamicEQPlugin::paramDetectFreq:
        fKnobDetectFreq->setValue(value);
        break;
    case ZamDynamicEQPlugin::paramTargetFreq:
        fKnobTargetFreq->setValue(value);
        break;
    case ZamDynamicEQPlugin::paramTargetWidth:
        fKnobTargetWidth->setValue(value);
        break;
    }
}

void ZamDynamicEQUI::programLoaded(uint32_t index)
{
    // Default values
	switch(index) {
	case 0:
		fKnobAttack->setValue(10.0f);
		fKnobRelease->setValue(80.0f);
		fKnobKnee->setValue(0.0f);
		fKnobRatio->setValue(4.0f);
		fKnobThresh->setValue(0.0f);
		fKnobMax->setValue(10.0f);
		fKnobSlew->setValue(1.0f);
		fToggleSidechain->setDown(false);
		fToggleBoostCut->setDown(false);
		fKnobTargetFreq->setValue(1000.0f);
		fKnobTargetWidth->setValue(1.0f);
		fKnobDetectFreq->setValue(1000.0f);
		fToggleLow->setChecked(false, false);
		fTogglePeak->setChecked(true, false);
		fToggleHigh->setChecked(false, false);
		break;
	case 1:
		fKnobAttack->setValue(10.0f);
		fKnobRelease->setValue(80.0f);
		fKnobKnee->setValue(1.0f);
		fKnobRatio->setValue(5.0f);
		fKnobThresh->setValue(-18.0f);
		fKnobMax->setValue(10.0f);
		fKnobSlew->setValue(20.0f);
		fToggleSidechain->setDown(false);
		fToggleBoostCut->setDown(false);
		fKnobTargetFreq->setValue(1000.0f);
		fKnobTargetWidth->setValue(1.0f);
		fKnobDetectFreq->setValue(1000.0f);
		fToggleLow->setChecked(false, false);
		fTogglePeak->setChecked(true, false);
		fToggleHigh->setChecked(false, false);
		break;
	case 2:
		fKnobAttack->setValue(50.0f);
		fKnobRelease->setValue(400.0f);
		fKnobKnee->setValue(8.0f);
		fKnobRatio->setValue(5.0f);
		fKnobThresh->setValue(-16.0f);
		fKnobMax->setValue(10.0f);
		fKnobSlew->setValue(1.0f);
		fToggleSidechain->setDown(false);
		fToggleBoostCut->setDown(false);
		fKnobTargetFreq->setValue(1000.0f);
		fKnobTargetWidth->setValue(1.0f);
		fKnobDetectFreq->setValue(1000.0f);
		fToggleLow->setChecked(false, false);
		fTogglePeak->setChecked(true, false);
		fToggleHigh->setChecked(false, false);
		break;
	}
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamDynamicEQUI::imageKnobDragStarted(ZamKnob* knob)
{
    editParameter(knob->getId(), true);
}

void ZamDynamicEQUI::imageKnobDragFinished(ZamKnob* knob)
{
    editParameter(knob->getId(), false);
}

void ZamDynamicEQUI::imageKnobValueChanged(ZamKnob* knob, float value)
{
    setParameterValue(knob->getId(), value);
}

void ZamDynamicEQUI::imageSwitchClicked(ImageSwitch* tog, bool down)
{
    setParameterValue(tog->getId(), down ? 1.f : 0.f);
}

void ZamDynamicEQUI::imageButtonClicked(ImageButton* tog, int)
{
    setParameterValue(tog->getId(), 1.f);
    switch(tog->getId()) {
    case ZamDynamicEQPlugin::paramToggleLow:
        setParameterValue(ZamDynamicEQPlugin::paramToggleHigh, 0.f);
        setParameterValue(ZamDynamicEQPlugin::paramTogglePeak, 0.f);
        fTogglePeak->setChecked(false, false);
        fToggleHigh->setChecked(false, false);
        break;
    case ZamDynamicEQPlugin::paramTogglePeak:
        setParameterValue(ZamDynamicEQPlugin::paramToggleHigh, 0.f);
        setParameterValue(ZamDynamicEQPlugin::paramToggleLow, 0.f);
        fToggleLow->setChecked(false, false);
        fToggleHigh->setChecked(false, false);
        break;
    case ZamDynamicEQPlugin::paramToggleHigh:
        setParameterValue(ZamDynamicEQPlugin::paramTogglePeak, 0.f);
        setParameterValue(ZamDynamicEQPlugin::paramToggleLow, 0.f);
        fToggleLow->setChecked(false, false);
        fTogglePeak->setChecked(false, false);
        break;
    }
}

void ZamDynamicEQUI::peq(double G0, double G, double GB, double w0, double Dw,
        double *a0, double *a1, double *a2, double *b0, double *b1, double *b2, double *gn) {

        double F,G00,F00,num,den,G1,G01,G11,F01,F11,W2,Dww,C,D,B,A;
        F = fabs(G*G - GB*GB);
        G00 = fabs(G*G - G0*G0);
        F00 = fabs(GB*GB - G0*G0);
        num = G0*G0 * (w0*w0 - M_PI*M_PI)*(w0*w0 - M_PI*M_PI)
                + G*G * F00 * M_PI*M_PI * Dw*Dw / F;
        den = (w0*w0 - M_PI*M_PI)*(w0*w0 - M_PI*M_PI)
                + F00 * M_PI*M_PI * Dw*Dw / F;
        G1 = sqrt(num/den);
        G01 = fabs(G*G - G0*G1);
        G11 = fabs(G*G - G1*G1);
        F01 = fabs(GB*GB - G0*G1);
        F11 = fabs(GB*GB - G1*G1);
        W2 = sqrt(G11 / G00) * tan(w0/2.f)*tan(w0/2.f);
        Dww = (1.f + sqrt(F00 / F11) * W2) * tan(Dw/2.f);
        C = F11 * Dww*Dww - 2.f * W2 * (F01 - sqrt(F00 * F11));
        D = 2.f * W2 * (G01 - sqrt(G00 * G11));
        A = sqrt((C + D) / F);
        B = sqrt((G*G * C + GB*GB * D) / F);
        *gn = G1;
        *b0 = (G1 + G0*W2 + B) / (1.f + W2 + A);
        *b1 = -2.f*(G1 - G0*W2) / (1.f + W2 + A);
        *b2 = (G1 - B + G0*W2) / (1.f + W2 + A);
        *a0 = 1.f;
        *a1 = -2.f*(1.f - W2) / (1.f + W2 + A);
        *a2 = (1 + W2 - A) / (1.f + W2 + A);

        *b1 = sanitize_denormal(*b1);
        *b2 = sanitize_denormal(*b2);
        *a0 = sanitize_denormal(*a0);
        *a1 = sanitize_denormal(*a1);
        *a2 = sanitize_denormal(*a2);
        *gn = sanitize_denormal(*gn);
        if (!std::isnormal(*b0)) { *b0 = 1.f; }
}

void ZamDynamicEQUI::lowshelfeq(double, double G, double, double w0, double, double q, double B[], double A[]) {
        double alpha,b0,b1,b2,a0,a1,a2;
        G = powf(10.f,G/20.f);
        double AA  = sqrt(G);

        alpha = sin(w0)/2.f * sqrt( (AA + 1.f/AA)*(1.f/q - 1.f) + 2.f );
        b0 =    AA*( (AA+1.f) - (AA-1.f)*cos(w0) + 2.f*sqrt(AA)*alpha );
        b1 =  2.f*AA*( (AA-1.f) - (AA+1.f)*cos(w0)                   );
        b2 =    AA*( (AA+1.f) - (AA-1.f)*cos(w0) - 2.f*sqrt(AA)*alpha );
        a0 =        (AA+1.f) + (AA-1.f)*cos(w0) + 2.f*sqrt(AA)*alpha;
        a1 =   -2.f*( (AA-1.f) + (AA+1.f)*cos(w0)                   );
        a2 =        (AA+1.f) + (AA-1.f)*cos(w0) - 2.f*sqrt(AA)*alpha;

        B[0] = b0/a0;
        B[1] = b1/a0;
        B[2] = b2/a0;
        A[0] = 1.f;
        A[1] = a1/a0;
        A[2] = a2/a0;
}

void ZamDynamicEQUI::highshelfeq(double, double G, double, double w0, double, double q, double B[], double A[])
{
        double alpha,b0,b1,b2,a0,a1,a2;
        G = powf(10.f,G/20.f);
        double AA  = sqrt(G);

        alpha = sin(w0)/2.f * sqrt( (AA + 1.f/AA)*(1.f/q - 1.f) + 2.f );
        b0 =    AA*( (AA+1.f) + (AA-1.f)*cos(w0) + 2.f*sqrt(AA)*alpha );
        b1 =  -2.f*AA*( (AA-1.f) + (AA+1.f)*cos(w0)                   );
        b2 =    AA*( (AA+1.f) + (AA-1.f)*cos(w0) - 2.f*sqrt(AA)*alpha );
        a0 =        (AA+1.f) - (AA-1.f)*cos(w0) + 2.f*sqrt(AA)*alpha;
        a1 =   2.f*( (AA-1.f) - (AA+1.f)*cos(w0)                   );
        a2 =        (AA+1.f) - (AA-1.f)*cos(w0) - 2.f*sqrt(AA)*alpha;

        B[0] = b0/a0;
        B[1] = b1/a0;
        B[2] = b2/a0;
        A[0] = 1.f;
        A[1] = a1/a0;
        A[2] = a2/a0;
}

void ZamDynamicEQUI::calceqcurve(float x[], float y[])
{
        float SR = getSampleRate();
        double dcgain = 1.f;

	double q2 = fKnobTargetWidth->getValue();
        double qq2 = pow(2.0, 1.0/q2)/(pow(2.0, q2) - 1.0); //q from octave bw
	double boost2 = from_dB(fControlGain);
	double fc2 = fKnobTargetFreq->getValue() / SR;
        double w02 = fc2*2.f*M_PI;
        double bwgain2 = (sqrt(boost2));
        double bw2 = fc2 / qq2;

	double boostl = (fControlGain);
	double freql = fKnobTargetFreq->getValue();
        double All = sqrt(from_dB(boostl));
        double bwl = 2.f*M_PI*freql/ SR;
        double bwgaindbl = (All);

	double boosth = (fControlGain);
	double freqh = fKnobTargetFreq->getValue();
        double Ahh = sqrt(from_dB(boosth));
        double bwh = 2.f*M_PI*freqh/ SR;
        double bwgaindbh = (Ahh);

        for (uint32_t i = 0; i < EQPOINTS; ++i) {
                double f;
		x[i] = i / (float)EQPOINTS;
		f = 20.f * powf (1000.f, i / (float)fCanvasArea.getWidth());
		std::complex<double> H;
		double theta = -f * 2. * M_PI / SR;
                std::complex<double> expiw = std::polar(1.0, theta);
                std::complex<double> exp2iw = std::polar(1.0, 2.0*theta);
                double freqH;

		if (fToggleLow->isChecked()) {
			lowshelfeq(0.f,boostl,bwgaindbl,bwl,bwl,0.707f,Bl,Al);
                	H = (1. + Al[1]*expiw + Al[2]*exp2iw)/(Bl[0] + Bl[1]*expiw + Bl[2]*exp2iw);
		} else if (fTogglePeak->isChecked()) {
			peq(dcgain,boost2,bwgain2,w02,bw2,&a0y,&a1y,&a2y,&b0y,&b1y,&b2y,&gainy);
                	H = (1. + a1y*expiw + a2y*exp2iw)/(b0y + b1y*expiw + b2y*exp2iw);
		} else {
			highshelfeq(0.f,boosth,bwgaindbh,bwh,bwh,0.707f,Bh,Ah);
                	H = (1. + Ah[1]*expiw + Ah[2]*exp2iw)/(Bh[0] + Bh[1]*expiw + Bh[2]*exp2iw);
		}
		// +- 6dB max
                freqH = (rintf(fCanvasArea.getHeight() * logf (std::abs(H)) / logf (1000.)*fCanvasArea.getHeight()/50.) / (float)fCanvasArea.getHeight()) + 0.5;
		y[i] = freqH;
		x[i] = fCanvasArea.getX() + x[i]*fCanvasArea.getWidth();
		y[i] = fCanvasArea.getY() + y[i]*fCanvasArea.getHeight();
        }
}

void ZamDynamicEQUI::onDisplay()
{
    const GraphicsContext& context(getGraphicsContext());

    calceqcurve(eqx, eqy);

    glClearColor(0.f, 0.f, 0.f, 0.f);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_COLOR_BUFFER_BIT);

    fImgBackground.draw(context);

    int i;
    glLineWidth(1);
    glColor4f(1.f, 1.f, 0.235f, 1.0f);
    for (i = 1; i < EQPOINTS; ++i) {
        glBegin(GL_LINES);
            if (eqy[i-1] < fCanvasArea.getY() + fCanvasArea.getHeight()
			&& eqy[i] < fCanvasArea.getY() + fCanvasArea.getHeight()
			&& eqy[i-1] > fCanvasArea.getY()
			&& eqy[i] > fCanvasArea.getY()) {
                glVertex2f(eqx[i-1], eqy[i-1]);
                glVertex2f(eqx[i], eqy[i]);
	    }
        glEnd();
    }
    // reset color
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glLineWidth(2);

    int xval = rintf(fCanvasArea.getWidth() * logf (fKnobDetectFreq->getValue() / 20.0) / logf (1000.0));
    glBegin(GL_LINES);
    	glVertex2f(xval+fCanvasArea.getX(), 5+fCanvasArea.getY());
    	glVertex2f(xval+fCanvasArea.getX(), fCanvasArea.getHeight()-5+fCanvasArea.getY());
    glEnd();

    glColor4f(1.f, 1.f, 0.235f, 1.0f);
    xval = rintf(fCanvasArea.getWidth() * logf (fKnobTargetFreq->getValue() / 20.0) / logf (1000.0));

    glBegin(GL_LINES);
    	glVertex2f(xval+fCanvasArea.getX(), 5+fCanvasArea.getY());
    	glVertex2f(xval+fCanvasArea.getX(), fCanvasArea.getHeight()-5+fCanvasArea.getY());
    glEnd();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamDynamicEQUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
