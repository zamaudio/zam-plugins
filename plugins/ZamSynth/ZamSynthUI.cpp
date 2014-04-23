/*
 * ZamSynth polyphonic synthesiser 
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

#include "ZamSynthUI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamSynthUI::ZamSynthUI()
    : UI()
{
    // background
    fImgBackground = Image(ZamSynthArtwork::zamsynthData, ZamSynthArtwork::zamsynthWidth, ZamSynthArtwork::zamsynthHeight, GL_BGR);

    // knob
    //Image knobImage(ZamSynthArtwork::knobData, ZamSynthArtwork::knobWidth, ZamSynthArtwork::knobHeight);

    // knob 
/*
    fKnobAttack = new ImageKnob(this, knobImage);
    fKnobAttack->setPos(24, 45);
    fKnobAttack->setRange(0.1f, 200.0f);
    fKnobAttack->setStep(0.1f);
    fKnobAttack->setLogScale(true);
    fKnobAttack->setDefault(10.0f);
    fKnobAttack->setRotationAngle(240);
    fKnobAttack->setCallback(this);
*/
    fCanvasArea.setPos(10,10);
    fCanvasArea.setSize(AREAHEIGHT,AREAHEIGHT);
}

ZamSynthUI::~ZamSynthUI()
{
	//delete fKnobAttack;
}

void ZamSynthUI::d_stateChanged(const char*, const char*)
{

}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamSynthUI::d_parameterChanged(uint32_t index, float value)
{
//    switch (index)
//    {
//    case ZamSynthPlugin::paramAttack:
//       fKnobAttack->setValue(value);
//        break;
//    }
}

void ZamSynthUI::d_programChanged(uint32_t index)
{
    if (index != 0)
        return;

    /* Default values
    fKnobAttack->setDefault(10.0f);
    fKnobRelease->setDefault(80.0f);
    fKnobThresh->setDefault(0.0f);
    fKnobRatio->setDefault(4.0f);
    fKnobKnee->setDefault(0.0f);
    fKnobMakeup->setDefault(0.0f);
    */
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamSynthUI::imageKnobDragStarted(ImageKnob* knob)
{
//    if (knob == fKnobAttack)
//        d_editParameter(ZamSynthPlugin::paramAttack, true);
}

void ZamSynthUI::imageKnobDragFinished(ImageKnob* knob)
{
//    if (knob == fKnobAttack)
//        d_editParameter(ZamSynthPlugin::paramAttack, false);
}

void ZamSynthUI::imageKnobValueChanged(ImageKnob* knob, float value)
{
//    if (knob == fKnobAttack)
//        d_setParameterValue(ZamSynthPlugin::paramAttack, value);
}

bool ZamSynthUI::onMouse(int button, bool press, int x, int y)
{
    if (button != 1)
        return false;

    if (press)
    {
        if (! fCanvasArea.contains(x, y))
            return false;

        fDragging = true;
        fDragValid = true;
        return true;
    }
    else if (fDragging)
    {
        fDragging = false;
        return true;
    }

    return false;
}

bool ZamSynthUI::onMotion(int x, int y)
{
    if (! fDragging)
        return false;
    if (! fDragValid)
    {
        fDragValid = true;
    }

    if (x > fCanvasArea.getWidth()+10)
    	x = fCanvasArea.getWidth()+10;

    if (y > fCanvasArea.getHeight()+10)
    	y = fCanvasArea.getHeight()+10;

    if (x < 10) x = 10;
    if (y < 10) y = 10;

    wave_y[x-10] = y-10;
    repaint();

    return true;
}


void ZamSynthUI::onDisplay()
{
	fImgBackground.draw();

	char tmp[4*AREAHEIGHT+1] = {0};
	int i;
	for(i = 0; i < AREAHEIGHT; i++) {
		char wavestr[5] = {0};
		snprintf(wavestr, sizeof(wavestr), "%03d ", (int) wave_y[i]);
		strcat(tmp, wavestr);
		//printf("wavestr=%4s",wavestr);
	}

	d_setState("",tmp);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glLineWidth(2);
        glColor4f(0.235f, 1.f, 0.235f, 1.0f);
        for (i = 2; i < AREAHEIGHT; ++i) {
            glBegin(GL_LINES);
                    glVertex2i(i-1+fCanvasArea.getX(), wave_y[i-1]+fCanvasArea.getY());
                    glVertex2i(i+fCanvasArea.getX(), wave_y[i]+fCanvasArea.getY());
            glEnd();
        }
    // reset color
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamSynthUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
