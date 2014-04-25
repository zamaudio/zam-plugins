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
    Image knobImage(ZamSynthArtwork::knobData, ZamSynthArtwork::knobWidth, ZamSynthArtwork::knobHeight);
    Image smoothrImage(ZamSynthArtwork::smoothrData, ZamSynthArtwork::smoothrWidth, ZamSynthArtwork::smoothrHeight);
    Image smoothyImage(ZamSynthArtwork::smoothyData, ZamSynthArtwork::smoothyWidth, ZamSynthArtwork::smoothyHeight);

    // knob 

    fKnobGain = new ImageKnob(this, knobImage);
    fKnobGain->setPos(284, 240);
    fKnobGain->setRange(-30.f, 30.0f);
    fKnobGain->setDefault(0.0f);
    fKnobGain->setRotationAngle(240);
    fKnobGain->setCallback(this);

    // button
    fButtonSmooth = new ImageButton(this, smoothrImage, smoothrImage, smoothyImage);
    fButtonSmooth->setPos(265, 55);
    fButtonSmooth->setCallback(this);

    fCanvasArea.setPos(10,10);
    fCanvasArea.setSize(AREAHEIGHT,AREAHEIGHT);
    for (int i = 0; i < AREAHEIGHT; i++) {
        wave_y[i] = -(AREAHEIGHT*(sin(2.*i*M_PI/AREAHEIGHT)-1.0))/2.;
    }
}

ZamSynthUI::~ZamSynthUI()
{
	delete fKnobGain;
	delete fButtonSmooth;
}

void ZamSynthUI::d_stateChanged(const char*, const char*)
{

}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamSynthUI::d_parameterChanged(uint32_t index, float value)
{
	switch (index)
	{
	case ZamSynthPlugin::paramGain:
		fKnobGain->setValue(value);
		break;
	}
}

void ZamSynthUI::d_programChanged(uint32_t index)
{
    if (index != 0)
        return;

    fKnobGain->setDefault(0.0f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamSynthUI::imageKnobDragStarted(ImageKnob* knob)
{
    if (knob == fKnobGain)
        d_editParameter(ZamSynthPlugin::paramGain, true);
}

void ZamSynthUI::imageKnobDragFinished(ImageKnob* knob)
{
    if (knob == fKnobGain)
        d_editParameter(ZamSynthPlugin::paramGain, false);
}

void ZamSynthUI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    if (knob == fKnobGain)
        d_setParameterValue(ZamSynthPlugin::paramGain, value);
}

void ZamSynthUI::imageButtonClicked(ImageButton* button, int)
{
	float wavesmooth[AREAHEIGHT];
	float xs[AREAHEIGHT];
	int i;
	for (i = 0; i < AREAHEIGHT; i++) {
		xs[i] = i;
	}
	gaussiansmooth(wavesmooth, xs, wave_y, AREAHEIGHT, 4);
	memcpy(wave_y, wavesmooth, AREAHEIGHT*sizeof(float));
	
	char tmp[4*AREAHEIGHT+1] = {0};
	for(i = 0; i < AREAHEIGHT; i++) {
		char wavestr[5] = {0};
		snprintf(wavestr, sizeof(wavestr), "%03d ", (int) (fCanvasArea.getHeight()-wave_y[i]));
		strcat(tmp, wavestr);
	}

	d_setState("waveform", tmp);
}

void ZamSynthUI::gaussiansmooth(float* smoothed, float* xs, float* ys, int n, int radius)
{
	int i,j;
	float numer;
	float denom;
	float kernel;
	for (i = 0; i < n; i++) {
		numer = 0.f;
		denom = 0.f;
		for (j = 0; j < n; j++) {
			kernel = expf(-(i - xs[j])*(i - xs[j]) / (2. * radius));
			numer += kernel * ys[j];
			denom += kernel;
		}
		smoothed[i] = numer / denom;
	}
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

    if (wave_y[x-10] != (y-10)) {
	char tmp[4*AREAHEIGHT+1] = {0};
	int i;
	for(i = 0; i < AREAHEIGHT; i++) {
		char wavestr[5] = {0};
		snprintf(wavestr, sizeof(wavestr), "%03d ", (int) (fCanvasArea.getHeight()-wave_y[i]));
		strcat(tmp, wavestr);
	}

        wave_y[x-10] = y-10;
        d_setState("waveform",tmp);
        repaint();
    }

    return true;
}


void ZamSynthUI::onDisplay()
{
	fImgBackground.draw();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glLineWidth(2);
    int i;
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
