/*
 * ZaMultiComp  multiband compressor
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

#include "ZaMultiCompPlugin.hpp"
#include "ZaMultiCompUI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZaMultiCompUI::ZaMultiCompUI()
    : UI()
{
    setSize(ZaMultiCompArtwork::zamulticompWidth, ZaMultiCompArtwork::zamulticompHeight);

    // background
    fImgBackground = Image(ZaMultiCompArtwork::zamulticompData, ZaMultiCompArtwork::zamulticompWidth, ZaMultiCompArtwork::zamulticompHeight, GL_BGR);

    // led images
    fLedRedImg = Image(ZaMultiCompArtwork::ledredData, ZaMultiCompArtwork::ledredWidth, ZaMultiCompArtwork::ledredHeight);
    fLedYellowImg = Image(ZaMultiCompArtwork::ledyellowData, ZaMultiCompArtwork::ledyellowWidth, ZaMultiCompArtwork::ledyellowHeight);

    // led values
    fLedRedValue[0] = 0.0f;
    fLedRedValue[1] = 0.0f;
    fLedRedValue[2] = 0.0f;
    fLedYellowValueL = -45.0f;

    // knob
    Image knobImage(ZaMultiCompArtwork::knobData, ZaMultiCompArtwork::knobWidth, ZaMultiCompArtwork::knobHeight);

    int x = 250;
    int y = 291;
    // knob
    fKnobAttack1 = new ZamKnob(this, knobImage);
    fKnobAttack1->setAbsolutePos(26+x, 43+y);
    fKnobAttack1->setRange(0.1f, 200.0f);
    fKnobAttack1->setLabel(true);
    fKnobAttack1->setScrollStep(1.0f);
    fKnobAttack1->setUsingLogScale(true);
    fKnobAttack1->setDefault(10.0f);
    fKnobAttack1->setRotationAngle(240);
    fKnobAttack1->setCallback(this);

    fKnobAttack2 = new ZamKnob(this, knobImage);
    fKnobAttack2->setAbsolutePos(26+x, 43+y-50);
    fKnobAttack2->setRange(0.1f, 200.0f);
    fKnobAttack2->setLabel(true);
    fKnobAttack2->setScrollStep(1.0f);
    fKnobAttack2->setUsingLogScale(true);
    fKnobAttack2->setDefault(10.0f);
    fKnobAttack2->setRotationAngle(240);
    fKnobAttack2->setCallback(this);

    fKnobAttack3 = new ZamKnob(this, knobImage);
    fKnobAttack3->setAbsolutePos(26+x, 43+y-100);
    fKnobAttack3->setRange(0.1f, 200.0f);
    fKnobAttack3->setLabel(true);
    fKnobAttack3->setScrollStep(1.0f);
    fKnobAttack3->setUsingLogScale(true);
    fKnobAttack3->setDefault(10.0f);
    fKnobAttack3->setRotationAngle(240);
    fKnobAttack3->setCallback(this);

    fKnobRelease1 = new ZamKnob(this, knobImage);
    fKnobRelease1->setAbsolutePos(108+x, 43+y);
    fKnobRelease1->setRange(1.0f, 500.0f);
    fKnobRelease1->setLabel(true);
    fKnobRelease1->setScrollStep(1.0f);
    fKnobRelease1->setDefault(80.0f);
    fKnobRelease1->setRotationAngle(240);
    fKnobRelease1->setCallback(this);

    fKnobRelease2 = new ZamKnob(this, knobImage);
    fKnobRelease2->setAbsolutePos(108+x, 43+y-50);
    fKnobRelease2->setRange(1.0f, 500.0f);
    fKnobRelease2->setLabel(true);
    fKnobRelease2->setScrollStep(1.0f);
    fKnobRelease2->setDefault(80.0f);
    fKnobRelease2->setRotationAngle(240);
    fKnobRelease2->setCallback(this);

    fKnobRelease3 = new ZamKnob(this, knobImage);
    fKnobRelease3->setAbsolutePos(108+x, 43+y-100);
    fKnobRelease3->setRange(1.0f, 500.0f);
    fKnobRelease3->setLabel(true);
    fKnobRelease3->setScrollStep(1.0f);
    fKnobRelease3->setDefault(80.0f);
    fKnobRelease3->setRotationAngle(240);
    fKnobRelease3->setCallback(this);

    fKnobThresh3 = new ZamKnob(this, knobImage);
    fKnobThresh3->setAbsolutePos(94.5+31, 99+134);
    fKnobThresh3->setRange(-60.0f, 0.0f);
    fKnobThresh3->setLabel(true);
    fKnobThresh3->setScrollStep(1.0f);
    fKnobThresh3->setDefault(-16.0f);
    fKnobThresh3->setRotationAngle(240);
    fKnobThresh3->setCallback(this);

    fKnobThresh2 = new ZamKnob(this, knobImage);
    fKnobThresh2->setAbsolutePos(94.5+31, 150+134);
    fKnobThresh2->setRange(-60.0f, 0.0f);
    fKnobThresh2->setLabel(true);
    fKnobThresh2->setScrollStep(1.0f);
    fKnobThresh2->setDefault(-18.0f);
    fKnobThresh2->setRotationAngle(240);
    fKnobThresh2->setCallback(this);

    fKnobThresh1 = new ZamKnob(this, knobImage);
    fKnobThresh1->setAbsolutePos(94.5+31, 201+134);
    fKnobThresh1->setRange(-60.0f, 0.0f);
    fKnobThresh1->setLabel(true);
    fKnobThresh1->setScrollStep(1.0f);
    fKnobThresh1->setDefault(-20.0f);
    fKnobThresh1->setRotationAngle(240);
    fKnobThresh1->setCallback(this);

    fKnobRatio1 = new ZamKnob(this, knobImage);
    fKnobRatio1->setAbsolutePos(191.5+x, 43+y);
    fKnobRatio1->setRange(1.0f, 20.0f);
    fKnobRatio1->setLabel(true);
    fKnobRatio1->setScrollStep(1.0f);
    fKnobRatio1->setUsingLogScale(true);
    fKnobRatio1->setDefault(4.0f);
    fKnobRatio1->setRotationAngle(240);
    fKnobRatio1->setCallback(this);

    fKnobRatio2 = new ZamKnob(this, knobImage);
    fKnobRatio2->setAbsolutePos(191.5+x, 43+y-50);
    fKnobRatio2->setRange(1.0f, 20.0f);
    fKnobRatio2->setLabel(true);
    fKnobRatio2->setScrollStep(1.0f);
    fKnobRatio2->setUsingLogScale(true);
    fKnobRatio2->setDefault(4.0f);
    fKnobRatio2->setRotationAngle(240);
    fKnobRatio2->setCallback(this);

    fKnobRatio3 = new ZamKnob(this, knobImage);
    fKnobRatio3->setAbsolutePos(191.5+x, 43+y-100);
    fKnobRatio3->setRange(1.0f, 20.0f);
    fKnobRatio3->setLabel(true);
    fKnobRatio3->setScrollStep(1.0f);
    fKnobRatio3->setUsingLogScale(true);
    fKnobRatio3->setDefault(4.0f);
    fKnobRatio3->setRotationAngle(240);
    fKnobRatio3->setCallback(this);

    fKnobKnee1 = new ZamKnob(this, knobImage);
    fKnobKnee1->setAbsolutePos(273+x, 43+y);
    fKnobKnee1->setRange(0.0f, 8.0f);
    fKnobKnee1->setLabel(true);
    fKnobKnee1->setScrollStep(1.f);
    fKnobKnee1->setDefault(0.0f);
    fKnobKnee1->setRotationAngle(240);
    fKnobKnee1->setCallback(this);

    fKnobKnee2 = new ZamKnob(this, knobImage);
    fKnobKnee2->setAbsolutePos(273+x, 43+y-50);
    fKnobKnee2->setRange(0.0f, 8.0f);
    fKnobKnee2->setLabel(true);
    fKnobKnee2->setScrollStep(1.f);
    fKnobKnee2->setDefault(0.0f);
    fKnobKnee2->setRotationAngle(240);
    fKnobKnee2->setCallback(this);

    fKnobKnee3 = new ZamKnob(this, knobImage);
    fKnobKnee3->setAbsolutePos(273+x, 43+y-100);
    fKnobKnee3->setRange(0.0f, 8.0f);
    fKnobKnee3->setLabel(true);
    fKnobKnee3->setScrollStep(1.f);
    fKnobKnee3->setDefault(0.0f);
    fKnobKnee3->setRotationAngle(240);
    fKnobKnee3->setCallback(this);

    fKnobGlobalGain = new ZamKnob(this, knobImage);
    fKnobGlobalGain->setAbsolutePos(427-170, 41);
    fKnobGlobalGain->setRange(-30.0f, 30.0f);
    fKnobGlobalGain->setLabel(true);
    fKnobGlobalGain->setScrollStep(1.f);
    fKnobGlobalGain->setDefault(0.0f);
    fKnobGlobalGain->setRotationAngle(240);
    fKnobGlobalGain->setCallback(this);

    fKnobXover2 = new ZamKnob(this, knobImage);
    fKnobXover2->setAbsolutePos(22+333, 121-80);
    fKnobXover2->setRange(1400.f, 14000.f);
    fKnobXover2->setLabel(true);
    fKnobXover2->setScrollStep(10.f);
    fKnobXover2->setUsingLogScale(true);
    fKnobXover2->setDefault(1400.f);
    fKnobXover2->setRotationAngle(240);
    fKnobXover2->setCallback(this);

    fKnobXover1 = new ZamKnob(this, knobImage);
    fKnobXover1->setAbsolutePos(22+333, 175.5-80);
    fKnobXover1->setRange(20.0f, 1400.0f);
    fKnobXover1->setLabel(true);
    fKnobXover1->setScrollStep(10.f);
    fKnobXover1->setUsingLogScale(true);
    fKnobXover1->setDefault(160.0f);
    fKnobXover1->setRotationAngle(240);
    fKnobXover1->setCallback(this);

    fKnobMakeup3 = new ZamKnob(this, knobImage);
    fKnobMakeup3->setAbsolutePos(167.75+30, 99.5+134);
    fKnobMakeup3->setRange(0.0f, 30.0f);
    fKnobMakeup3->setLabel(true);
    fKnobMakeup3->setScrollStep(1.f);
    fKnobMakeup3->setDefault(0.0f);
    fKnobMakeup3->setRotationAngle(240);
    fKnobMakeup3->setCallback(this);

    fKnobMakeup2 = new ZamKnob(this, knobImage);
    fKnobMakeup2->setAbsolutePos(167.75+30, 150.25+134);
    fKnobMakeup2->setRange(0.0f, 30.0f);
    fKnobMakeup2->setLabel(true);
    fKnobMakeup2->setScrollStep(1.f);
    fKnobMakeup2->setDefault(0.0f);
    fKnobMakeup2->setRotationAngle(240);
    fKnobMakeup2->setCallback(this);

    fKnobMakeup1 = new ZamKnob(this, knobImage);
    fKnobMakeup1->setAbsolutePos(167.75+30, 201.4+134);
    fKnobMakeup1->setRange(0.0f, 30.0f);
    fKnobMakeup1->setLabel(true);
    fKnobMakeup1->setScrollStep(1.f);
    fKnobMakeup1->setDefault(0.0f);
    fKnobMakeup1->setRotationAngle(240);
    fKnobMakeup1->setCallback(this);

    Image toggleonImage(ZaMultiCompArtwork::toggleonData, ZaMultiCompArtwork::toggleonWidth, ZaMultiCompArtwork::toggleonHeight);
    Image toggleoffImage(ZaMultiCompArtwork::toggleoffData, ZaMultiCompArtwork::toggleoffWidth, ZaMultiCompArtwork::toggleoffHeight);
    Image toggleonhImage(ZaMultiCompArtwork::toggleonhorizData, ZaMultiCompArtwork::toggleonhorizWidth, ZaMultiCompArtwork::toggleonhorizHeight);
    Image toggleoffhImage(ZaMultiCompArtwork::toggleoffhorizData, ZaMultiCompArtwork::toggleoffhorizWidth, ZaMultiCompArtwork::toggleoffhorizHeight);

    Point<int> togglePosStart(247-230+4,108+130+5);

    fToggleBypass3 = new ImageSwitch(this, toggleoffImage, toggleonImage);
    fToggleBypass3->setAbsolutePos(togglePosStart);
    fToggleBypass3->setCallback(this);

    togglePosStart.setY(158+130+5);

    fToggleBypass2 = new ImageSwitch(this, toggleoffImage, toggleonImage);
    fToggleBypass2->setAbsolutePos(togglePosStart);
    fToggleBypass2->setCallback(this);

    togglePosStart.setY(209+130+5);

    fToggleBypass1 = new ImageSwitch(this, toggleoffImage, toggleonImage);
    fToggleBypass1->setAbsolutePos(togglePosStart);
    fToggleBypass1->setCallback(this);

    togglePosStart.setX(278-230+4);
    togglePosStart.setY(113+130+5);

    fToggleListen3 = new ImageSwitch(this, toggleoffhImage, toggleonhImage);
    fToggleListen3->setAbsolutePos(togglePosStart);
    fToggleListen3->setCallback(this);

    togglePosStart.setY(164+130+5);

    fToggleListen2 = new ImageSwitch(this, toggleoffhImage, toggleonhImage);
    fToggleListen2->setAbsolutePos(togglePosStart);
    fToggleListen2->setCallback(this);

    togglePosStart.setY(214+130+5);

    fToggleListen1 = new ImageSwitch(this, toggleoffhImage, toggleonhImage);
    fToggleListen1->setAbsolutePos(togglePosStart);
    fToggleListen1->setCallback(this);

    togglePosStart.setX(369);
    togglePosStart.setY(181);

    fCanvasArea.setPos(430, 42);
    fCanvasArea.setSize(145, 145);

    int i,k;

    for (k = 0; k < MAX_COMP; ++k) {
        for (i = 0; i < COMPOINTS; ++i) {
                compx[k][i] = fCanvasArea.getX();
		compy[k][i] = fCanvasArea.getY() + fCanvasArea.getHeight();
	}
	dotx[k] = 0.f;
	doty[k] = 0.f;
    }

    // set default values
    programLoaded(0);
}

void ZaMultiCompUI::compcurve(float in, int k, float *outx, float* outy) {
        float knee = fKnee[k];
        float ratio = fRatio[k];
        float makeup = fMakeup[k] + fMaster;
        float thresdb = fThresh[k];
        float width=6.f*knee+0.01;
        float xg, yg;

        yg = 0.f;
        xg = (in==0.f) ? -160.f : to_dB(fabs(in));
        xg = sanitize_denormal(xg);

        if (2.f*(xg-thresdb)<-width) {
                yg = xg;
        } else if (2.f*fabs(xg-thresdb)<=width) {
                yg = xg + (1.f/ratio-1.f)*(xg-thresdb+width/2.f)*(xg-thresdb+width/2.f)/(2.f*width);
        } else if (2.f*(xg-thresdb)>width) {
                yg = thresdb + (xg-thresdb)/ratio;
        }
        yg = sanitize_denormal(yg);

        *outx = (to_dB(in) + 1.) / 55. + 1.;
        *outy = !fBypass[k] ? (to_dB(in) + fMaster + 1.) / 55. + 1. : (yg + makeup + 1.) / 55. + 1.;
	//printf("x = %f  y = %f\n",*outx,*outy);
}

void ZaMultiCompUI::compdot(float in, int k, float *outx, float* outy) {
        float makeup = fMakeup[k] + fMaster;
        float xg, yg;

        yg = 0.f;
        xg = (in==0.f) ? -160.f : to_dB(fabs(in));
        xg = sanitize_denormal(xg);
	yg = xg - fLedRedValue[k];
        yg = sanitize_denormal(yg);

        *outx = (to_dB(in) + 1.) / 55. + 1.;
        *outy = !fBypass[k] ? (to_dB(in) + fMaster + 1.) / 55. + 1. : (yg + makeup + 1.) / 55. + 1.;
}

void ZaMultiCompUI::calc_compcurves() {
        float max_x = 1.f;
        float min_x = 0.f;
        float x2;
	for (int k = 0; k < MAX_COMP; ++k) {
	        for (int i = 0; i < COMPOINTS; ++i) {
	                x2 = (max_x - min_x) / COMPOINTS * i + min_x;
	                compcurve(x2, k, &compx[k][i], &compy[k][i]);
                        compx[k][i] = fCanvasArea.getX() + compx[k][i]*fCanvasArea.getWidth();
                        compy[k][i] = fCanvasArea.getY() + (1.-compy[k][i])*fCanvasArea.getHeight();
		}
		 //dot follows curve:
	        compdot((max_x - min_x)*from_dB(outlevel[k]), k, &dotx[k], &doty[k]);
	        dotx[k] = fCanvasArea.getX() + dotx[k]*fCanvasArea.getWidth();
	        doty[k] = fCanvasArea.getY() + (1. - doty[k])*fCanvasArea.getHeight();
	}
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZaMultiCompUI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZaMultiCompPlugin::paramAttack1:
        fKnobAttack1->setValue(value);
        break;
    case ZaMultiCompPlugin::paramAttack2:
        fKnobAttack2->setValue(value);
        break;
    case ZaMultiCompPlugin::paramAttack3:
        fKnobAttack3->setValue(value);
        break;
    case ZaMultiCompPlugin::paramRelease1:
        fKnobRelease1->setValue(value);
        break;
    case ZaMultiCompPlugin::paramRelease2:
        fKnobRelease2->setValue(value);
        break;
    case ZaMultiCompPlugin::paramRelease3:
        fKnobRelease3->setValue(value);
        break;
    case ZaMultiCompPlugin::paramThresh1:
        fKnobThresh1->setValue(value);
        fThresh[0] = value;
        break;
    case ZaMultiCompPlugin::paramThresh2:
        fKnobThresh2->setValue(value);
        fThresh[1] = value;
        break;
    case ZaMultiCompPlugin::paramThresh3:
        fKnobThresh3->setValue(value);
        fThresh[2] = value;
        break;
    case ZaMultiCompPlugin::paramRatio1:
        fKnobRatio1->setValue(value);
        fRatio[0] = value;
        break;
    case ZaMultiCompPlugin::paramRatio2:
        fKnobRatio2->setValue(value);
        fRatio[1] = value;
        break;
    case ZaMultiCompPlugin::paramRatio3:
        fKnobRatio3->setValue(value);
        fRatio[2] = value;
        break;
    case ZaMultiCompPlugin::paramKnee1:
        fKnobKnee1->setValue(value);
        fKnee[0] = value;
        break;
    case ZaMultiCompPlugin::paramKnee2:
        fKnobKnee2->setValue(value);
        fKnee[1] = value;
        break;
    case ZaMultiCompPlugin::paramKnee3:
        fKnobKnee3->setValue(value);
        fKnee[2] = value;
        break;
    case ZaMultiCompPlugin::paramGlobalGain:
        fKnobGlobalGain->setValue(value);
        fMaster = value;
        break;
    case ZaMultiCompPlugin::paramXover1:
        fKnobXover1->setValue(value);
        break;
    case ZaMultiCompPlugin::paramXover2:
        fKnobXover2->setValue(value);
        break;
    case ZaMultiCompPlugin::paramGainR1:
        if (fLedRedValue[0] != value)
        {
            fLedRedValue[0] = value;
            repaint();
        }
        break;
    case ZaMultiCompPlugin::paramGainR2:
        if (fLedRedValue[1] != value)
        {
            fLedRedValue[1] = value;
            repaint();
        }
        break;
    case ZaMultiCompPlugin::paramGainR3:
        if (fLedRedValue[2] != value)
        {
            fLedRedValue[2] = value;
            repaint();
        }
        break;
    case ZaMultiCompPlugin::paramOutputLevel:
        if (fLedYellowValueL != value)
        {
            fLedYellowValueL = value;
            repaint();
        }
        break;
    case ZaMultiCompPlugin::paramOutputLevelLow:
        if (outlevel[0] != value) {
		outlevel[0] = value;
		repaint();
	}
	break;
    case ZaMultiCompPlugin::paramOutputLevelMed:
        if (outlevel[1] != value) {
		outlevel[1] = value;
		repaint();
	}
	break;
    case ZaMultiCompPlugin::paramOutputLevelHigh:
        if (outlevel[2] != value) {
		outlevel[2] = value;
		repaint();
	}
	break;
    case ZaMultiCompPlugin::paramMakeup1:
        fKnobMakeup1->setValue(value);
        if (fMakeup[0] != value)
        {
            fMakeup[0] = value;
            repaint();
        }
        break;
    case ZaMultiCompPlugin::paramMakeup2:
        fKnobMakeup2->setValue(value);
        if (fMakeup[1] != value)
        {
            fMakeup[1] = value;
            repaint();
        }
        break;
    case ZaMultiCompPlugin::paramMakeup3:
        fKnobMakeup3->setValue(value);
        if (fMakeup[2] != value)
        {
            fMakeup[2] = value;
            repaint();
        }
        break;
    case ZaMultiCompPlugin::paramToggle1:
        fToggleBypass1->setDown(value > 0.5);
        if (fBypass[0] != value)
        {
            fBypass[0] = value;
            repaint();
        }
        break;
    case ZaMultiCompPlugin::paramToggle2:
        fToggleBypass2->setDown(value > 0.5);
        if (fBypass[1] != value)
        {
            fBypass[1] = value;
            repaint();
        }
        break;
    case ZaMultiCompPlugin::paramToggle3:
        fToggleBypass3->setDown(value > 0.5);
        if (fBypass[2] != value)
        {
            fBypass[2] = value;
            repaint();
        }
        break;
    case ZaMultiCompPlugin::paramListen1:
        fToggleListen1->setDown(value > 0.5);
        if (fListen[0] != value)
        {
            fListen[0] = value;
            repaint();
        }
        break;
    case ZaMultiCompPlugin::paramListen2:
        fToggleListen2->setDown(value > 0.5);
        if (fListen[1] != value)
        {
            fListen[1] = value;
            repaint();
        }
        break;
    case ZaMultiCompPlugin::paramListen3:
        fToggleListen3->setDown(value > 0.5);
        if (fListen[2] != value)
        {
            fListen[2] = value;
            repaint();
        }
        break;
    }
}

void ZaMultiCompUI::programLoaded(uint32_t index)
{
	switch(index) {
	case 0:
		fKnobAttack1->setValue(10.0f);
		fKnobRelease1->setValue(80.0f);
		fKnobKnee1->setValue(0.0f);
		fKnobRatio1->setValue(4.0f);
		fKnobAttack2->setValue(10.0f);
		fKnobRelease2->setValue(80.0f);
		fKnobKnee2->setValue(0.0f);
		fKnobRatio2->setValue(4.0f);
		fKnobAttack3->setValue(10.0f);
		fKnobRelease3->setValue(80.0f);
		fKnobKnee3->setValue(0.0f);
		fKnobRatio3->setValue(4.0f);
		fKnobThresh1->setValue(-20.0f);
		fKnobThresh2->setValue(-18.0f);
		fKnobThresh3->setValue(-16.0f);
		fKnobMakeup1->setValue(0.0f);
		fKnobMakeup2->setValue(0.0f);
		fKnobMakeup3->setValue(0.0f);
		fKnobXover1->setValue(160.0f);
		fKnobXover2->setValue(1400.0f);
		fToggleBypass1->setDown(false);
		fToggleBypass2->setDown(false);
		fToggleBypass3->setDown(false);
		fToggleListen1->setDown(false);
		fToggleListen2->setDown(false);
		fToggleListen3->setDown(false);
		fKnobGlobalGain->setValue(0.0f);
		break;
	case 1:
		fKnobAttack1->setValue(10.0f);
		fKnobRelease1->setValue(200.0f);
		fKnobKnee1->setValue(3.0f);
		fKnobRatio1->setValue(5.0f);
		fKnobAttack2->setValue(10.0f);
		fKnobRelease2->setValue(200.0f);
		fKnobKnee2->setValue(3.0f);
		fKnobRatio2->setValue(5.0f);
		fKnobAttack3->setValue(10.0f);
		fKnobRelease3->setValue(200.0f);
		fKnobKnee3->setValue(3.0f);
		fKnobRatio3->setValue(5.0f);
		fKnobThresh1->setValue(-20.0f);
		fKnobThresh2->setValue(-18.0f);
		fKnobThresh3->setValue(-16.0f);
		fKnobMakeup1->setValue(9.0f);
		fKnobMakeup2->setValue(6.0f);
		fKnobMakeup3->setValue(6.0f);
		fKnobXover1->setValue(160.0f);
		fKnobXover2->setValue(1400.0f);
		fToggleBypass1->setDown(true);
		fToggleBypass2->setDown(true);
		fToggleBypass3->setDown(true);
		fToggleListen1->setDown(false);
		fToggleListen2->setDown(false);
		fToggleListen3->setDown(false);
		fKnobGlobalGain->setValue(0.0f);
		break;
	}
}

void ZaMultiCompUI::stateChanged(const char*, const char*)
{
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZaMultiCompUI::imageKnobDragStarted(ZamKnob* knob)
{
    if (knob == fKnobAttack1)
        editParameter(ZaMultiCompPlugin::paramAttack1, true);
    else if (knob == fKnobAttack2)
        editParameter(ZaMultiCompPlugin::paramAttack2, true);
    else if (knob == fKnobAttack3)
        editParameter(ZaMultiCompPlugin::paramAttack3, true);
    else if (knob == fKnobRelease1)
        editParameter(ZaMultiCompPlugin::paramRelease1, true);
    else if (knob == fKnobRelease2)
        editParameter(ZaMultiCompPlugin::paramRelease2, true);
    else if (knob == fKnobRelease3)
        editParameter(ZaMultiCompPlugin::paramRelease3, true);
    else if (knob == fKnobThresh1)
        editParameter(ZaMultiCompPlugin::paramThresh1, true);
    else if (knob == fKnobThresh2)
        editParameter(ZaMultiCompPlugin::paramThresh2, true);
    else if (knob == fKnobThresh3)
        editParameter(ZaMultiCompPlugin::paramThresh3, true);
    else if (knob == fKnobRatio1)
        editParameter(ZaMultiCompPlugin::paramRatio1, true);
    else if (knob == fKnobRatio2)
        editParameter(ZaMultiCompPlugin::paramRatio2, true);
    else if (knob == fKnobRatio3)
        editParameter(ZaMultiCompPlugin::paramRatio3, true);
    else if (knob == fKnobKnee1)
        editParameter(ZaMultiCompPlugin::paramKnee1, true);
    else if (knob == fKnobKnee2)
        editParameter(ZaMultiCompPlugin::paramKnee2, true);
    else if (knob == fKnobKnee3)
        editParameter(ZaMultiCompPlugin::paramKnee3, true);
    else if (knob == fKnobGlobalGain)
        editParameter(ZaMultiCompPlugin::paramGlobalGain, true);
    else if (knob == fKnobMakeup1)
        editParameter(ZaMultiCompPlugin::paramMakeup1, true);
    else if (knob == fKnobMakeup2)
        editParameter(ZaMultiCompPlugin::paramMakeup2, true);
    else if (knob == fKnobMakeup3)
        editParameter(ZaMultiCompPlugin::paramMakeup3, true);
    else if (knob == fKnobXover1)
        editParameter(ZaMultiCompPlugin::paramXover1, true);
    else if (knob == fKnobXover2)
        editParameter(ZaMultiCompPlugin::paramXover2, true);
}

void ZaMultiCompUI::imageKnobDragFinished(ZamKnob* knob)
{
    if (knob == fKnobAttack1)
        editParameter(ZaMultiCompPlugin::paramAttack1, false);
    else if (knob == fKnobAttack2)
        editParameter(ZaMultiCompPlugin::paramAttack2, false);
    else if (knob == fKnobAttack3)
        editParameter(ZaMultiCompPlugin::paramAttack3, false);
    else if (knob == fKnobRelease1)
        editParameter(ZaMultiCompPlugin::paramRelease1, false);
    else if (knob == fKnobRelease2)
        editParameter(ZaMultiCompPlugin::paramRelease2, false);
    else if (knob == fKnobRelease3)
        editParameter(ZaMultiCompPlugin::paramRelease3, false);
    else if (knob == fKnobThresh1)
        editParameter(ZaMultiCompPlugin::paramThresh1, false);
    else if (knob == fKnobThresh2)
        editParameter(ZaMultiCompPlugin::paramThresh2, false);
    else if (knob == fKnobThresh3)
        editParameter(ZaMultiCompPlugin::paramThresh3, false);
    else if (knob == fKnobRatio1)
        editParameter(ZaMultiCompPlugin::paramRatio1, false);
    else if (knob == fKnobRatio2)
        editParameter(ZaMultiCompPlugin::paramRatio2, false);
    else if (knob == fKnobRatio3)
        editParameter(ZaMultiCompPlugin::paramRatio3, false);
    else if (knob == fKnobKnee1)
        editParameter(ZaMultiCompPlugin::paramKnee1, false);
    else if (knob == fKnobKnee2)
        editParameter(ZaMultiCompPlugin::paramKnee2, false);
    else if (knob == fKnobKnee3)
        editParameter(ZaMultiCompPlugin::paramKnee3, false);
    else if (knob == fKnobGlobalGain)
        editParameter(ZaMultiCompPlugin::paramGlobalGain, false);
    else if (knob == fKnobMakeup1)
        editParameter(ZaMultiCompPlugin::paramMakeup1, false);
    else if (knob == fKnobMakeup2)
        editParameter(ZaMultiCompPlugin::paramMakeup2, false);
    else if (knob == fKnobMakeup3)
        editParameter(ZaMultiCompPlugin::paramMakeup3, false);
    else if (knob == fKnobXover1)
        editParameter(ZaMultiCompPlugin::paramXover1, false);
    else if (knob == fKnobXover2)
        editParameter(ZaMultiCompPlugin::paramXover2, false);
}

void ZaMultiCompUI::imageKnobValueChanged(ZamKnob* knob, float value)
{
    if (knob == fKnobAttack1)
        setParameterValue(ZaMultiCompPlugin::paramAttack1, value);
    else if (knob == fKnobAttack2)
        setParameterValue(ZaMultiCompPlugin::paramAttack2, value);
    else if (knob == fKnobAttack3)
        setParameterValue(ZaMultiCompPlugin::paramAttack3, value);
    else if (knob == fKnobRelease1)
        setParameterValue(ZaMultiCompPlugin::paramRelease1, value);
    else if (knob == fKnobRelease2)
        setParameterValue(ZaMultiCompPlugin::paramRelease2, value);
    else if (knob == fKnobRelease3)
        setParameterValue(ZaMultiCompPlugin::paramRelease3, value);
    else if (knob == fKnobThresh1) {
        setParameterValue(ZaMultiCompPlugin::paramThresh1, value);
	fThresh[0] = value;
    }
    else if (knob == fKnobThresh2) {
        setParameterValue(ZaMultiCompPlugin::paramThresh2, value);
	fThresh[1] = value;
    }
    else if (knob == fKnobThresh3) {
        setParameterValue(ZaMultiCompPlugin::paramThresh3, value);
	fThresh[2] = value;
    }
    else if (knob == fKnobRatio1) {
        setParameterValue(ZaMultiCompPlugin::paramRatio1, value);
        fRatio[0] = value;
    }
    else if (knob == fKnobRatio2) {
        setParameterValue(ZaMultiCompPlugin::paramRatio2, value);
        fRatio[1] = value;
    }
    else if (knob == fKnobRatio3) {
        setParameterValue(ZaMultiCompPlugin::paramRatio3, value);
        fRatio[2] = value;
    }
    else if (knob == fKnobKnee1) {
        setParameterValue(ZaMultiCompPlugin::paramKnee1, value);
	fKnee[0] = value;
    }
    else if (knob == fKnobKnee2) {
        setParameterValue(ZaMultiCompPlugin::paramKnee2, value);
	fKnee[1] = value;
    }
    else if (knob == fKnobKnee3) {
        setParameterValue(ZaMultiCompPlugin::paramKnee3, value);
	fKnee[2] = value;
    }
    else if (knob == fKnobGlobalGain) {
        setParameterValue(ZaMultiCompPlugin::paramGlobalGain, value);
        fMaster = value;
    }
    else if (knob == fKnobMakeup1) {
        setParameterValue(ZaMultiCompPlugin::paramMakeup1, value);
        fMakeup[0] = value;
    }
    else if (knob == fKnobMakeup2) {
        setParameterValue(ZaMultiCompPlugin::paramMakeup2, value);
        fMakeup[1] = value;
    }
    else if (knob == fKnobMakeup3) {
        setParameterValue(ZaMultiCompPlugin::paramMakeup3, value);
        fMakeup[2] = value;
    }
    else if (knob == fKnobXover1)
        setParameterValue(ZaMultiCompPlugin::paramXover1, value);
    else if (knob == fKnobXover2)
        setParameterValue(ZaMultiCompPlugin::paramXover2, value);
}

void ZaMultiCompUI::imageSwitchClicked(ImageSwitch* toggle, bool down)
{
    float v = down ? 1.f : 0.f;
    if (toggle == fToggleBypass1) {
        setParameterValue(ZaMultiCompPlugin::paramToggle1, v);
        fBypass[0] = v;
    }
    else if (toggle == fToggleBypass2) {
        setParameterValue(ZaMultiCompPlugin::paramToggle2, v);
        fBypass[1] = v;
    }
    else if (toggle == fToggleBypass3) {
        setParameterValue(ZaMultiCompPlugin::paramToggle3, v);
        fBypass[2] = v;
    }
    else if (toggle == fToggleListen1) {
        setParameterValue(ZaMultiCompPlugin::paramListen1, v);
        fListen[0] = v;
    }
    else if (toggle == fToggleListen2) {
        setParameterValue(ZaMultiCompPlugin::paramListen2, v);
        fListen[1] = v;
    }
    else if (toggle == fToggleListen3) {
        setParameterValue(ZaMultiCompPlugin::paramListen3, v);
        fListen[2] = v;
    }
}

void ZaMultiCompUI::onDisplay()
{
    fImgBackground.draw();

    setState("stateMeterReset", "");

    // draw leds
    static const float sLedSpacing  = 15.3f;
    static const int   sLedInitialX = 35.5;

    static const int sYellowLedStaticYL = 254.5-70+3;
    static const int sRedLed1StaticY    = 215-70+3;
    static const int sRedLed2StaticY    = 164-70+4;
    static const int sRedLed3StaticY    = 113-70+5;

    int numRedLeds1;
    int numRedLeds2;
    int numRedLeds3;
    int numYellowLedsL;

	if (fLedRedValue[0] >= 40.f)
		numRedLeds1 = 12;
	else if (fLedRedValue[0] >= 30.f)
		numRedLeds1 = 11;
	else if (fLedRedValue[0] >= 20.f)
		numRedLeds1 = 10;
	else if (fLedRedValue[0] >= 15.f)
		numRedLeds1 = 9;
	else if (fLedRedValue[0] >= 10.f)
		numRedLeds1 = 8;
	else if (fLedRedValue[0] >= 8.f)
		numRedLeds1 = 7;
	else if (fLedRedValue[0] >= 6.f)
		numRedLeds1 = 6;
	else if (fLedRedValue[0] >= 5.f)
		numRedLeds1 = 5;
	else if (fLedRedValue[0] >= 4.f)
		numRedLeds1 = 4;
	else if (fLedRedValue[0] >= 3.f)
		numRedLeds1 = 3;
	else if (fLedRedValue[0] >= 2.f)
		numRedLeds1 = 2;
	else if (fLedRedValue[0] >= 1.f)
		numRedLeds1 = 1;
	else numRedLeds1 = 0;

	if (fLedRedValue[1] >= 40.f)
		numRedLeds2 = 12;
	else if (fLedRedValue[1] >= 30.f)
		numRedLeds2 = 11;
	else if (fLedRedValue[1] >= 20.f)
		numRedLeds2 = 10;
	else if (fLedRedValue[1] >= 15.f)
		numRedLeds2 = 9;
	else if (fLedRedValue[1] >= 10.f)
		numRedLeds2 = 8;
	else if (fLedRedValue[1] >= 8.f)
		numRedLeds2 = 7;
	else if (fLedRedValue[1] >= 6.f)
		numRedLeds2 = 6;
	else if (fLedRedValue[1] >= 5.f)
		numRedLeds2 = 5;
	else if (fLedRedValue[1] >= 4.f)
		numRedLeds2 = 4;
	else if (fLedRedValue[1] >= 3.f)
		numRedLeds2 = 3;
	else if (fLedRedValue[1] >= 2.f)
		numRedLeds2 = 2;
	else if (fLedRedValue[1] >= 1.f)
		numRedLeds2 = 1;
	else numRedLeds2 = 0;

	if (fLedRedValue[2] >= 40.f)
		numRedLeds3 = 12;
	else if (fLedRedValue[2] >= 30.f)
		numRedLeds3 = 11;
	else if (fLedRedValue[2] >= 20.f)
		numRedLeds3 = 10;
	else if (fLedRedValue[2] >= 15.f)
		numRedLeds3 = 9;
	else if (fLedRedValue[2] >= 10.f)
		numRedLeds3 = 8;
	else if (fLedRedValue[2] >= 8.f)
		numRedLeds3 = 7;
	else if (fLedRedValue[2] >= 6.f)
		numRedLeds3 = 6;
	else if (fLedRedValue[2] >= 5.f)
		numRedLeds3 = 5;
	else if (fLedRedValue[2] >= 4.f)
		numRedLeds3 = 4;
	else if (fLedRedValue[2] >= 3.f)
		numRedLeds3 = 3;
	else if (fLedRedValue[2] >= 2.f)
		numRedLeds3 = 2;
	else if (fLedRedValue[2] >= 1.f)
		numRedLeds3 = 1;
	else numRedLeds3 = 0;

    for (int i=numRedLeds1; i>0; --i)
        fLedRedImg.drawAt(sLedInitialX + (12 - i)*sLedSpacing, sRedLed1StaticY);

    for (int i=numRedLeds2; i>0; --i)
        fLedRedImg.drawAt(sLedInitialX + (12 - i)*sLedSpacing, sRedLed2StaticY);

    for (int i=numRedLeds3; i>0; --i)
        fLedRedImg.drawAt(sLedInitialX + (12 - i)*sLedSpacing, sRedLed3StaticY);

	if (fLedYellowValueL >= 20.f)
		numYellowLedsL = 19;
	else if (fLedYellowValueL >= 10.f)
		numYellowLedsL = 18;
	else if (fLedYellowValueL >= 8.f)
		numYellowLedsL = 17;
	else if (fLedYellowValueL >= 4.f)
		numYellowLedsL = 16;
	else if (fLedYellowValueL >= 2.f)
		numYellowLedsL = 15;
	else if (fLedYellowValueL >= 1.f)
		numYellowLedsL = 14;
	else if (fLedYellowValueL >= 0.f)
		numYellowLedsL = 13;
	else if (fLedYellowValueL >= -1.f)
		numYellowLedsL = 12;
	else if (fLedYellowValueL >= -2.f)
		numYellowLedsL = 11;
	else if (fLedYellowValueL >= -3.f)
		numYellowLedsL = 10;
	else if (fLedYellowValueL >= -4.f)
		numYellowLedsL = 9;
	else if (fLedYellowValueL >= -5.f)
		numYellowLedsL = 8;
	else if (fLedYellowValueL >= -6.f)
		numYellowLedsL = 7;
	else if (fLedYellowValueL >= -8.f)
		numYellowLedsL = 6;
	else if (fLedYellowValueL >= -10.f)
		numYellowLedsL = 5;
	else if (fLedYellowValueL >= -15.f)
		numYellowLedsL = 4;
	else if (fLedYellowValueL >= -20.f)
		numYellowLedsL = 3;
	else if (fLedYellowValueL >= -30.f)
		numYellowLedsL = 2;
	else if (fLedYellowValueL >= -40.f)
		numYellowLedsL = 1;
	else numYellowLedsL = 0;

	if (numYellowLedsL > 12) {
		for (int i=12; i<numYellowLedsL; ++i)
			fLedRedImg.drawAt(sLedInitialX + i*sLedSpacing, sYellowLedStaticYL);
		for (int i=0; i<12; ++i)
			fLedYellowImg.drawAt(sLedInitialX + i*sLedSpacing, sYellowLedStaticYL);
	} else {
		for (int i=0; i<numYellowLedsL; ++i)
			fLedYellowImg.drawAt(sLedInitialX + i*sLedSpacing, sYellowLedStaticYL);
	}

/*
// TESTING - remove later
// this paints the 'fCanvasArea' so we can clearly see its bounds
{
const int x = fCanvasArea.getX();
const int y = fCanvasArea.getY();
const int w = fCanvasArea.getWidth();
const int h = fCanvasArea.getHeight();

glColor4f(0.0f, 1.0f, 0.0f, 0.9f);

glBegin(GL_QUADS);
glTexCoord2f(0.0f, 0.0f);
glVertex2i(x, y);

glTexCoord2f(1.0f, 0.0f);
glVertex2i(x+w, y);

glTexCoord2f(1.0f, 1.0f);
glVertex2i(x+w, y+h);

glTexCoord2f(0.0f, 1.0f);
glVertex2i(x, y+h);
glEnd();

// reset color
glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
*/

    calc_compcurves();

    //draw comp curves
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(8.0);
    glEnable(GL_POINT_SPRITE);

    glLineWidth(2);
    int i,k;
    int flip[3] = {1,1,1};
    if (fListen[0]) {
    	flip[1] = 0;
    	flip[2] = 0;
    }
    if (fListen[1]) {
    	flip[0] = 0;
    	flip[2] = 0;
    }
    if (fListen[2]) {
    	flip[0] = 0;
    	flip[1] = 0;
    }
    for (k = 0; k < MAX_COMP; ++k) {
        glColor4f((k==0)?1.f:0.235f, (k==1)?1.f:0.235f, (k==2)?1.f:0.235f, 1.0f);
	if (flip[k]) {
	for (i = 2; i < COMPOINTS; ++i) {
            glBegin(GL_LINES);
                if (compy[k][i-1] < fCanvasArea.getY() + fCanvasArea.getHeight()
			&& compy[k][i-1] > fCanvasArea.getY()
                	&& compy[k][i] < fCanvasArea.getY() + fCanvasArea.getHeight()
			&& compy[k][i] > fCanvasArea.getY()) {
                    glVertex2i(compx[k][i-1], compy[k][i-1]);
                    glVertex2i(compx[k][i], compy[k][i]);
                }
	    glEnd();
        }

	glBegin(GL_POINTS);
		if (doty[k] < fCanvasArea.getY() + fCanvasArea.getHeight()
				&& doty[k] > fCanvasArea.getY()
				&& dotx[k] < fCanvasArea.getX() + fCanvasArea.getWidth()
				&& dotx[k] > fCanvasArea.getX()) {
			glVertex2i(dotx[k], doty[k]);
		}
	glEnd();
	}
    }
    // reset color
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZaMultiCompUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
