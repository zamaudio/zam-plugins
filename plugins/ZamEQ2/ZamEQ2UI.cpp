/*
 * ZamEQ2 2 band parametric equaliser
 * Copyright (C) 2014  Damien Zammit <damien@zamaudio.com>
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

#include "ZamEQ2UI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamEQ2UI::ZamEQ2UI()
    : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true)
{
    // background
    fImgBackground = Image(ZamEQ2Artwork::zameq2Data, ZamEQ2Artwork::zameq2Width, ZamEQ2Artwork::zameq2Height, kImageFormatBGR);

    // toggle image
    Image sliderImage(ZamEQ2Artwork::togglesliderData, ZamEQ2Artwork::togglesliderWidth, ZamEQ2Artwork::togglesliderHeight);

    // knob
    Image knobImage(ZamEQ2Artwork::knobData, ZamEQ2Artwork::knobWidth, ZamEQ2Artwork::knobHeight);

    // knob
    fKnobGain1 = new ZamKnob(this, knobImage);
    fKnobGain1->setAbsolutePos(91, 172);
    fKnobGain1->setRange(-20.f, 20.0f);
    fKnobGain1->setLabel(true);
    fKnobGain1->setScrollStep(1.0f);
    fKnobGain1->setRotationAngle(240);
    fKnobGain1->setDefault(0.0f);
    fKnobGain1->setCallback(this);

    fKnobQ1 = new ZamKnob(this, knobImage);
    fKnobQ1->setAbsolutePos(91, 122);
    fKnobQ1->setRange(0.7f, 2.5f);
    fKnobQ1->setLabel(true);
    fKnobQ1->setScrollStep(0.1f);
    fKnobQ1->setRotationAngle(240);
    fKnobQ1->setDefault(1.5f);
    fKnobQ1->setCallback(this);

    fKnobFreq1 = new ZamKnob(this, knobImage);
    fKnobFreq1->setAbsolutePos(23, 144);
    fKnobFreq1->setUsingLogScale(true);
    fKnobFreq1->setRange(200.f, 2500.0f);
    fKnobFreq1->setLabel(true);
    fKnobFreq1->setScrollStep(10.f);
    fKnobFreq1->setRotationAngle(240);
    fKnobFreq1->setDefault(1000.0f);
    fKnobFreq1->setCallback(this);

    fKnobGain2 = new ZamKnob(this, knobImage);
    fKnobGain2->setAbsolutePos(567, 172);
    fKnobGain2->setRange(-20.f, 20.0f);
    fKnobGain2->setLabel(true);
    fKnobGain2->setScrollStep(1.f);
    fKnobGain2->setRotationAngle(240);
    fKnobGain2->setDefault(0.0f);
    fKnobGain2->setCallback(this);

    fKnobQ2 = new ZamKnob(this, knobImage);
    fKnobQ2->setAbsolutePos(567, 122);
    fKnobQ2->setRange(0.7f, 2.5f);
    fKnobQ2->setLabel(true);
    fKnobQ2->setScrollStep(0.1f);
    fKnobQ2->setRotationAngle(240);
    fKnobQ2->setDefault(1.5f);
    fKnobQ2->setCallback(this);

    fKnobFreq2 = new ZamKnob(this, knobImage);
    fKnobFreq2->setAbsolutePos(499, 144);
    fKnobFreq2->setUsingLogScale(true);
    fKnobFreq2->setRange(600.f, 7000.0f);
    fKnobFreq2->setLabel(true);
    fKnobFreq2->setScrollStep(10.f);
    fKnobFreq2->setRotationAngle(240);
    fKnobFreq2->setDefault(3000.0f);
    fKnobFreq2->setCallback(this);

    fKnobGainL = new ZamKnob(this, knobImage);
    fKnobGainL->setAbsolutePos(91, 52);
    fKnobGainL->setRange(-20.f, 20.0f);
    fKnobGainL->setLabel(true);
    fKnobGainL->setScrollStep(1.f);
    fKnobGainL->setRotationAngle(240);
    fKnobGainL->setDefault(0.0f);
    fKnobGainL->setCallback(this);

    fKnobFreqL = new ZamKnob(this, knobImage);
    fKnobFreqL->setAbsolutePos(23, 23);
    fKnobFreqL->setUsingLogScale(true);
    fKnobFreqL->setRange(40.f, 600.0f);
    fKnobFreqL->setLabel(true);
    fKnobFreqL->setScrollStep(10.f);
    fKnobFreqL->setRotationAngle(240);
    fKnobFreqL->setDefault(250.0f);
    fKnobFreqL->setCallback(this);

    fKnobGainH = new ZamKnob(this, knobImage);
    fKnobGainH->setAbsolutePos(567, 53);
    fKnobGainH->setRange(-20.f, 20.0f);
    fKnobGainH->setLabel(true);
    fKnobGainH->setScrollStep(1.f);
    fKnobGainH->setRotationAngle(240);
    fKnobGainH->setDefault(0.0f);
    fKnobGainH->setCallback(this);

    fKnobFreqH = new ZamKnob(this, knobImage);
    fKnobFreqH->setAbsolutePos(499, 24);
    fKnobFreqH->setUsingLogScale(true);
    fKnobFreqH->setRange(1500.f, 22000.0f);
    fKnobFreqH->setLabel(true);
    fKnobFreqH->setScrollStep(10.f);
    fKnobFreqH->setRotationAngle(240);
    fKnobFreqH->setDefault(8000.0f);
    fKnobFreqH->setCallback(this);

    Point<int> masterPosStart(211,204);
    Point<int> masterPosEnd(288,204);

    fSliderMaster = new ImageSlider(this, sliderImage);
    fSliderMaster->setStartPos(masterPosStart);
    fSliderMaster->setEndPos(masterPosEnd);
    fSliderMaster->setRange(-12.f,12.f);
    fSliderMaster->setDefault(0.f);
    fSliderMaster->setCallback(this);

    fCanvasArea.setPos(165,10);
    fCanvasArea.setSize(305,180);

    // set default values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamEQ2UI::parameterChanged(uint32_t index, float value)
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
    case ZamEQ2Plugin::paramMaster:
    	fSliderMaster->setValue(value);
	break;
    }
}

void ZamEQ2UI::programLoaded(uint32_t index)
{
	switch(index) {
	case 0:
		fKnobGain1->setValue(0.0f);
		fKnobGain2->setValue(0.0f);
		fKnobGainL->setValue(0.0f);
		fKnobGainH->setValue(0.0f);
		fKnobQ1->setValue(1.0f);
		fKnobQ2->setValue(1.0f);
		fKnobFreq1->setValue(500.0f);
		fKnobFreq2->setValue(3000.0f);
		fKnobFreqL->setValue(250.0f);
		fKnobFreqH->setValue(8000.0f);
		fSliderMaster->setValue(0.f);
		break;
	case 1:
		fKnobGain1->setValue(0.0f);
		fKnobGain2->setValue(0.0f);
		fKnobGainL->setValue(6.0f);
		fKnobGainH->setValue(0.0f);
		fKnobQ1->setValue(1.0f);
		fKnobQ2->setValue(1.0f);
		fKnobFreq1->setValue(500.0f);
		fKnobFreq2->setValue(3000.0f);
		fKnobFreqL->setValue(60.0f);
		fKnobFreqH->setValue(8000.0f);
		fSliderMaster->setValue(-6.f);
		break;
	case 2:
		fKnobGain1->setValue(5.6f);
		fKnobGain2->setValue(0.7f);
		fKnobGainL->setValue(-1.05f);
		fKnobGainH->setValue(0.0f);
		fKnobQ1->setValue(1.46f);
		fKnobQ2->setValue(1.33f);
		fKnobFreq1->setValue(175.0f);
		fKnobFreq2->setValue(500.0f);
		fKnobFreqL->setValue(30.0f);
		fKnobFreqH->setValue(8000.0f);
		fSliderMaster->setValue(-6.f);
		break;
	case 3:
		fKnobGain1->setValue(0.0f);
		fKnobGain2->setValue(0.0f);
		fKnobGainL->setValue(6.0f);
		fKnobGainH->setValue(6.0f);
		fKnobQ1->setValue(1.0f);
		fKnobQ2->setValue(1.0f);
		fKnobFreq1->setValue(500.0f);
		fKnobFreq2->setValue(3000.0f);
		fKnobFreqL->setValue(6600.0f);
		fKnobFreqH->setValue(330.0f);
		fSliderMaster->setValue(-12.f);
		break;
	}
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamEQ2UI::imageKnobDragStarted(ZamKnob* knob)
{
    if (knob == fKnobGain1)
        editParameter(ZamEQ2Plugin::paramGain1, true);
    else if (knob == fKnobQ1)
        editParameter(ZamEQ2Plugin::paramQ1, true);
    else if (knob == fKnobFreq1)
        editParameter(ZamEQ2Plugin::paramFreq1, true);
    else if (knob == fKnobGain2)
        editParameter(ZamEQ2Plugin::paramGain2, true);
    else if (knob == fKnobQ2)
        editParameter(ZamEQ2Plugin::paramQ2, true);
    else if (knob == fKnobFreq2)
        editParameter(ZamEQ2Plugin::paramFreq2, true);
    else if (knob == fKnobGainL)
        editParameter(ZamEQ2Plugin::paramGainL, true);
    else if (knob == fKnobFreqL)
        editParameter(ZamEQ2Plugin::paramFreqL, true);
    else if (knob == fKnobGainH)
        editParameter(ZamEQ2Plugin::paramGainH, true);
    else if (knob == fKnobFreqH)
        editParameter(ZamEQ2Plugin::paramFreqH, true);
}

void ZamEQ2UI::imageKnobDragFinished(ZamKnob* knob)
{
    if (knob == fKnobGain1)
        editParameter(ZamEQ2Plugin::paramGain1, false);
    else if (knob == fKnobQ1)
        editParameter(ZamEQ2Plugin::paramQ1, false);
    else if (knob == fKnobFreq1)
        editParameter(ZamEQ2Plugin::paramFreq1, false);
    else if (knob == fKnobGain2)
        editParameter(ZamEQ2Plugin::paramGain2, false);
    else if (knob == fKnobQ2)
        editParameter(ZamEQ2Plugin::paramQ2, false);
    else if (knob == fKnobFreq2)
        editParameter(ZamEQ2Plugin::paramFreq2, false);
    else if (knob == fKnobGainL)
        editParameter(ZamEQ2Plugin::paramGainL, false);
    else if (knob == fKnobFreqL)
        editParameter(ZamEQ2Plugin::paramFreqL, false);
    else if (knob == fKnobGainH)
        editParameter(ZamEQ2Plugin::paramGainH, false);
    else if (knob == fKnobFreqH)
        editParameter(ZamEQ2Plugin::paramFreqH, false);
}

void ZamEQ2UI::imageKnobValueChanged(ZamKnob* knob, float value)
{
    if (knob == fKnobGain1)
        setParameterValue(ZamEQ2Plugin::paramGain1, value);
    else if (knob == fKnobQ1)
        setParameterValue(ZamEQ2Plugin::paramQ1, value);
    else if (knob == fKnobFreq1)
        setParameterValue(ZamEQ2Plugin::paramFreq1, value);
    else if (knob == fKnobGain2)
        setParameterValue(ZamEQ2Plugin::paramGain2, value);
    else if (knob == fKnobQ2)
        setParameterValue(ZamEQ2Plugin::paramQ2, value);
    else if (knob == fKnobFreq2)
        setParameterValue(ZamEQ2Plugin::paramFreq2, value);
    else if (knob == fKnobGainL)
        setParameterValue(ZamEQ2Plugin::paramGainL, value);
    else if (knob == fKnobFreqL)
        setParameterValue(ZamEQ2Plugin::paramFreqL, value);
    else if (knob == fKnobGainH)
        setParameterValue(ZamEQ2Plugin::paramGainH, value);
    else if (knob == fKnobFreqH)
        setParameterValue(ZamEQ2Plugin::paramFreqH, value);
}

void ZamEQ2UI::imageSliderDragStarted(ImageSlider* slider)
{
    if (slider == fSliderMaster)
        editParameter(ZamEQ2Plugin::paramMaster, true);
}

void ZamEQ2UI::imageSliderDragFinished(ImageSlider* slider)
{
    if (slider == fSliderMaster)
        editParameter(ZamEQ2Plugin::paramMaster, false);
}

void ZamEQ2UI::imageSliderValueChanged(ImageSlider* slider, float value)
{
    if (slider == fSliderMaster)
        setParameterValue(ZamEQ2Plugin::paramMaster, value);
}

void ZamEQ2UI::peq(double G0, double G, double GB, double w0, double Dw,
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

void ZamEQ2UI::lowshelfeq(double, double G, double, double w0, double, double q, double B[], double A[]) {
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

void ZamEQ2UI::highshelfeq(double, double G, double, double w0, double, double q, double B[], double A[])
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

void ZamEQ2UI::calceqcurve(float x[], float y[])
{
        float SR = getSampleRate();
        float p1 = 10000.;
        float p2 = 5000.;
        float c2 = log10(1.+SR);
        float c1 = (1.+p1/SR)/(EQPOINTS*(p2/SR)*(p2/SR));

        double dcgain = 1.f;

	double q1 = fKnobQ1->getValue();
        double qq1 = pow(2.0, 1.0/q1)/(pow(2.0, q1) - 1.0); //q from octave bw
	double boost1 = from_dB(fKnobGain1->getValue());
	double fc1 = fKnobFreq1->getValue() / SR;
        double w01 = fc1*2.f*M_PI;
        double bwgain1 = (sqrt(boost1));
        double bw1 = fc1 / qq1;

	double q2 = fKnobQ2->getValue();
        double qq2 = pow(2.0, 1.0/q2)/(pow(2.0, q2) - 1.0); //q from octave bw
	double boost2 = from_dB(fKnobGain2->getValue());
	double fc2 = fKnobFreq2->getValue() / SR;
        double w02 = fc2*2.f*M_PI;
        double bwgain2 = (sqrt(boost2));
        double bw2 = fc2 / qq2;

	double boostl = (fKnobGainL->getValue());
	double freql = fKnobFreqL->getValue();
        double All = sqrt(from_dB(boostl));
        double bwl = 2.f*M_PI*freql/ SR;
        double bwgaindbl = (All);

	double boosth = (fKnobGainH->getValue());
	double freqh = fKnobFreqH->getValue();
        double Ahh = sqrt(from_dB(boosth));
        double bwh = 2.f*M_PI*freqh/ SR;
        double bwgaindbh = (Ahh);

        for (uint32_t i = 0; i < EQPOINTS; ++i) {
                x[i] = 1.5*log10(1.+i+c1)/c2;

                std::complex<double> H;
		double theta = -(i+0.005)*M_PI/EQPOINTS*20./(SR/1000.);
                std::complex<double> expiw = std::polar(1.0, theta);
                std::complex<double> exp2iw = std::polar(1.0, 2.0*theta);
                double freqH; //phaseH;

		peq(dcgain,boost1,bwgain1,w01,bw1,&a0x,&a1x,&a2x,&b0x,&b1x,&b2x,&gainx);
		peq(dcgain,boost2,bwgain2,w02,bw2,&a0y,&a1y,&a2y,&b0y,&b1y,&b2y,&gainy);
		lowshelfeq(0.f,boostl,bwgaindbl,2.f*M_PI*freql/SR,bwl,0.707f,Bl,Al);
		highshelfeq(0.f,boosth,bwgaindbh,2.f*M_PI*freqh/SR,bwh,0.707f,Bh,Ah);

                H = (1. + Al[1]*expiw + Al[2]*exp2iw)/(Bl[0] + Bl[1]*expiw + Bl[2]*exp2iw);
                H += (1. + a1x*expiw + a2x*exp2iw)/(b0x + b1x*expiw + b2x*exp2iw);
                H += (1. + a1y*expiw + a2y*exp2iw)/(b0y + b1y*expiw + b2y*exp2iw);
                H += (1. + Ah[1]*expiw + Ah[2]*exp2iw)/(Bh[0] + Bh[1]*expiw + Bh[2]*exp2iw);

                freqH = to_dB(std::abs(H) / 4.);
		if (freqH < -100.) freqH = -100.;
		if (freqH > 100.) freqH = 100.;
                y[i] = (freqH / 5.)-(fSliderMaster->getValue())/24.f+0.5;
		x[i] = fCanvasArea.getX() + x[i]*fCanvasArea.getWidth();
		y[i] = fCanvasArea.getY() + y[i]*fCanvasArea.getHeight();
        }
}


void ZamEQ2UI::onDisplay()
{
    const GraphicsContext& context(getGraphicsContext());
    fImgBackground.draw(context);

    calceqcurve(eqx, eqy);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glLineWidth(2);
    int i;
    glColor4f(1.f, 1.f, 0.235f, 1.0f);
    for (i = 1; i < EQPOINTS; ++i) {
        glBegin(GL_LINES);
            if (eqy[i-1] < fCanvasArea.getY() + fCanvasArea.getHeight()
			&& eqy[i] < fCanvasArea.getY() + fCanvasArea.getHeight()
			&& eqy[i-1] > fCanvasArea.getY()
			&& eqy[i] > fCanvasArea.getY()) {
                glVertex2i(eqx[i-1], eqy[i-1]);
                glVertex2i(eqx[i], eqy[i]);
	    }
        glEnd();
    }
    // reset color
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamEQ2UI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
