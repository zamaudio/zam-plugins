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

    // toggle image
    Image sliderImage(ZamEQ2Artwork::togglesliderData, ZamEQ2Artwork::togglesliderWidth, ZamEQ2Artwork::togglesliderHeight);

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
    fKnobQ1->setRange(0.1f, 6.0f);
    fKnobQ1->setStep(0.1f);
    fKnobQ1->setValue(1.0f);
    fKnobQ1->setRotationAngle(240);
    fKnobQ1->setCallback(this);

    fKnobFreq1 = new ImageKnob(this, knobImage);
    fKnobFreq1->setPos(23, 144);
    fKnobFreq1->setRange(20.f, 14000.0f);
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
    fKnobQ2->setRange(0.1f, 6.0f);
    fKnobQ2->setStep(0.1f);
    fKnobQ2->setValue(1.0f);
    fKnobQ2->setRotationAngle(240);
    fKnobQ2->setCallback(this);

    fKnobFreq2 = new ImageKnob(this, knobImage);
    fKnobFreq2->setPos(499, 144);
    fKnobFreq2->setRange(20.f, 14000.0f);
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
    fKnobFreqL->setRange(20.f, 14000.0f);
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
    fKnobFreqH->setRange(20.f, 14000.0f);
    fKnobFreqH->setStep(1.f);
    fKnobFreqH->setLogScale(true);
    fKnobFreqH->setValue(500.0f);
    fKnobFreqH->setRotationAngle(240);
    fKnobFreqH->setCallback(this);

    Point<int> masterPosStart(211,204);
    Point<int> masterPosEnd(288,204);

    fSliderMaster = new ImageSlider(this, sliderImage);
    fSliderMaster->setStartPos(masterPosStart);
    fSliderMaster->setEndPos(masterPosEnd);
    fSliderMaster->setRange(-12.f,12.f);
    fSliderMaster->setValue(0.f);
    fSliderMaster->setCallback(this);
    
    fCanvasArea.setPos(165,10);
    fCanvasArea.setSize(305,180);
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
    delete fSliderMaster;
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
    case ZamEQ2Plugin::paramMaster:
    	fSliderMaster->setValue(value);
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
    fSliderMaster->setValue(0.f);
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

void ZamEQ2UI::imageSliderDragStarted(ImageSlider* slider)
{
    if (slider == fSliderMaster)
        d_editParameter(ZamEQ2Plugin::paramMaster, true);
}

void ZamEQ2UI::imageSliderDragFinished(ImageSlider* slider)
{
    if (slider == fSliderMaster)
        d_editParameter(ZamEQ2Plugin::paramMaster, false);
}

void ZamEQ2UI::imageSliderValueChanged(ImageSlider* slider, float value)
{
    if (slider == fSliderMaster)
        d_setParameterValue(ZamEQ2Plugin::paramMaster, value);
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
        W2 = sqrt(G11 / G00) * tan(w0/2.)*tan(w0/2.);
        Dww = (1.+ sqrt(F00 / F11) * W2) * tan(Dw/2.);
        C = F11 * Dww*Dww - 2. * W2 * (F01 - sqrt(F00 * F11));
        D = 2. * W2 * (G01 - sqrt(G00 * G11));
        A = sqrt((C + D) / F);
        B = sqrt((G*G * C + GB*GB * D) / F);
        *gn = G1;
        *b0 = (G1 + G0*W2 + B) / (1. + W2 + A);
        *b1 = -2.*(G1 - G0*W2) / (1. + W2 + A);
        *b2 = (G1 - B + G0*W2) / (1. + W2 + A);
        *a0 = 1.;
        *a1 = -2.*(1. - W2) / (1. + W2 + A);
        *a2 = (1. + W2 - A) / (1. + W2 + A);

        *b1 = sanitize_denormal(*b1);
        *b2 = sanitize_denormal(*b2);
        *a0 = sanitize_denormal(*a0);
        *a1 = sanitize_denormal(*a1);
        *a2 = sanitize_denormal(*a2);
        *gn = sanitize_denormal(*gn);
        if (!std::isnormal(*b0)) { *b0 = 1.; }
}


void ZamEQ2UI::lowshelfeq(double G0, double G, double GB, double w0, double Dw, double q, double B[], double A[]) {
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

void ZamEQ2UI::highshelfeq(double G0, double G, double GB, double w0, double Dw, double q,  double B[], double A[]) {
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
        float SR = d_getSampleRate();
        float p1 = 10000.;
        float p2 = 5000.;
        float c2 = log10(1.+SR);
        float c1 = (1.+p1/SR)/(EQPOINTS*(p2/SR)*(p2/SR));

        for (uint32_t i = 0; i < EQPOINTS; ++i) {
                x[i] = 1.5*log10(1.+i+c1)/c2;

                double L,M,N,O,P,Q,R;
                std::complex<double> H;
		double theta = -(i+0.005)*M_PI/EQPOINTS*20./(SR/1000.);
                std::complex<double> expiw = std::polar(1.0, theta);
                std::complex<double> exp2iw = std::polar(1.0, 2.0*theta);
                double freqH; //phaseH;
                double dcgain = 1.f;

                double qq1 = pow(2.0, 1.0/fKnobQ1->getValue())/(pow(2.0, fKnobQ1->getValue()) - 1.0); //q from octave bw
                double boost1 = from_dB(fKnobGain1->getValue());
                double fc1 = fKnobFreq1->getValue() / SR;
                double w01 = fc1 * 2. * M_PI;
                double bwgain1 = sqrt(boost1)*sqrt(dcgain);
                double bw1 = fc1 / qq1;

                peq(1.0, (boost1), (bwgain1), w01, bw1, &P, &Q, &R, &M, &N, &O, &L);
                H = (M + N*expiw + O*exp2iw)/(P + Q*expiw + R*exp2iw);

                qq1 = pow(2.0, 1.0/fKnobQ2->getValue())/(pow(2.0, fKnobQ2->getValue()) - 1.0); //q from octave bw
                boost1 = from_dB(fKnobGain2->getValue());
                fc1 = fKnobFreq2->getValue() / SR;
                w01 = fc1 * 2. * M_PI;
                bwgain1 = sqrt(boost1)*sqrt(dcgain);
                bw1 = fc1 / qq1;

                peq(1.0, (boost1), (bwgain1), w01, bw1, &P, &Q, &R, &M, &N, &O, &L);
                H += (M + N*expiw + O*exp2iw)/(P + Q*expiw + R*exp2iw);
                double boostl = from_dB(fKnobGainL->getValue());
                double All = sqrt(boostl);
                double bwl = 2.f*M_PI*fKnobFreqL->getValue()/ SR;
                double bwgaindbl = to_dB(All);

                double boosth = from_dB(fKnobGainH->getValue());
                double Ahh = sqrt(boosth);
                double bwh = 2.f*M_PI*fKnobFreqH->getValue()/ SR;
                double bwgaindbh = to_dB(Ahh);

                double Al[3], Ah[3], Bl[3], Bh[3];

                lowshelfeq(0.f,(fKnobGainL->getValue()),(bwgaindbl),bwl,bwl,1.0,Bl,Al);
                M = Bl[0]; N = Bl[1]; O = Bl[2];
                P = Al[0]; Q = Al[1]; R = Al[2];
                H += (M + N*expiw + O*exp2iw)/(P + Q*expiw + R*exp2iw);

                highshelfeq(0.f,(fKnobGainH->getValue()),bwgaindbh,bwh,bwh,1.0,Bh,Ah);
                M = Bh[0]; N = Bh[1]; O = Bh[2];
                P = Ah[0]; Q = Ah[1]; R = Ah[2];
                H += (M + N*expiw + O*exp2iw)/(P + Q*expiw + R*exp2iw);

                freqH = std::abs(H);
                //phaseH = carg(H);

                //y[i] = (to_dB(freqH)-12.04)/12. + 0.5;
                y[i] = (to_dB(freqH/4.)/10.)-(fSliderMaster->getValue())/24.f+0.5;
		//printf("%.4f\n",y[i]);
		x[i] = fCanvasArea.getX() + x[i]*fCanvasArea.getWidth();
		y[i] = fCanvasArea.getY() + (1.-y[i])*fCanvasArea.getHeight();
        }
}


void ZamEQ2UI::onDisplay()
{
    fImgBackground.draw();

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
            glVertex2i(eqx[i-1], eqy[i-1]);
            glVertex2i(eqx[i], eqy[i]);
            //printf("x = %f  y = %f\n",compx[k][i],compy[k][i]);
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
