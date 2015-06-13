/*
 * ZamGEQ31X2 31 band graphic equaliser (stereo)
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

#include "ZamGEQ31X2UI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamGEQ31X2UI::ZamGEQ31X2UI()
    : UI()
{
    setSize(ZamGEQ31X2Artwork::zamgeq31x2Width, ZamGEQ31X2Artwork::zamgeq31x2Height);

    // background
    fImgBackground = Image(ZamGEQ31X2Artwork::zamgeq31x2Data, ZamGEQ31X2Artwork::zamgeq31x2Width, ZamGEQ31X2Artwork::zamgeq31x2Height, GL_BGR);

    // slider image
    Image sliderImage(ZamGEQ31X2Artwork::slidereqData, ZamGEQ31X2Artwork::slidereqWidth, ZamGEQ31X2Artwork::slidereqHeight);

    // toggle
    Image toggleoffImage(ZamGEQ31X2Artwork::toggleoffhorizData, ZamGEQ31X2Artwork::toggleoffhorizWidth, ZamGEQ31X2Artwork::toggleoffhorizHeight);

    Image toggleonImage(ZamGEQ31X2Artwork::toggleonhorizData, ZamGEQ31X2Artwork::toggleonhorizWidth, ZamGEQ31X2Artwork::toggleonhorizHeight);

    // master slider
    Point<int> masterPosStart(95,253);
    Point<int> masterPosEnd(172,253);
    fSliderMaster = new ImageSlider(this, sliderImage);
    fSliderMaster->setStartPos(masterPosStart);
    fSliderMaster->setEndPos(masterPosEnd);
    fSliderMaster->setRange(-12.f,0.f);
    fSliderMaster->setValue(0.f);
    fSliderMaster->setStep(3.f);
    fSliderMaster->setCallback(this);

    float i,j;
    i = 24.;
    j = 18.75;
    masterPosStart.setX(i);
    masterPosEnd.setX(j);
    masterPosStart.setY(38.);
    masterPosEnd.setY(115.);

    fSliderGain1L = new ImageSlider(this, sliderImage);
    fSliderGain1L->setStartPos(masterPosStart);
    fSliderGain1L->setEndPos(masterPosEnd);
    fSliderGain1L->setRange(-12.f,12.f);
    fSliderGain1L->setValue(0.f);
    fSliderGain1L->setCallback(this);
    fSliderGain1L->setInverted(true);

    masterPosStart.setX(i+j);
    masterPosEnd.setX(i+j);

    fSliderGain2L = new ImageSlider(this, sliderImage);
    fSliderGain2L->setStartPos(masterPosStart);
    fSliderGain2L->setEndPos(masterPosEnd);
    fSliderGain2L->setRange(-12.f,12.f);
    fSliderGain2L->setValue(0.f);
    fSliderGain2L->setCallback(this);
    fSliderGain2L->setInverted(true);

    masterPosStart.setX(i+2*j);
    masterPosEnd.setX(i+2*j);

    fSliderGain3L = new ImageSlider(this, sliderImage);
    fSliderGain3L->setStartPos(masterPosStart);
    fSliderGain3L->setEndPos(masterPosEnd);
    fSliderGain3L->setRange(-12.f,12.f);
    fSliderGain3L->setValue(0.f);
    fSliderGain3L->setCallback(this);
    fSliderGain3L->setInverted(true);

    masterPosStart.setX(i+3*j);
    masterPosEnd.setX(i+3*j);

    fSliderGain4L = new ImageSlider(this, sliderImage);
    fSliderGain4L->setStartPos(masterPosStart);
    fSliderGain4L->setEndPos(masterPosEnd);
    fSliderGain4L->setRange(-12.f,12.f);
    fSliderGain4L->setValue(0.f);
    fSliderGain4L->setCallback(this);
    fSliderGain4L->setInverted(true);

    masterPosStart.setX(i+4*j);
    masterPosEnd.setX(i+4*j);

    fSliderGain5L = new ImageSlider(this, sliderImage);
    fSliderGain5L->setStartPos(masterPosStart);
    fSliderGain5L->setEndPos(masterPosEnd);
    fSliderGain5L->setRange(-12.f,12.f);
    fSliderGain5L->setValue(0.f);
    fSliderGain5L->setCallback(this);
    fSliderGain5L->setInverted(true);

    masterPosStart.setX(i+5*j);
    masterPosEnd.setX(i+5*j);

    fSliderGain6L = new ImageSlider(this, sliderImage);
    fSliderGain6L->setStartPos(masterPosStart);
    fSliderGain6L->setEndPos(masterPosEnd);
    fSliderGain6L->setRange(-12.f,12.f);
    fSliderGain6L->setValue(0.f);
    fSliderGain6L->setCallback(this);
    fSliderGain6L->setInverted(true);

    masterPosStart.setX(i+6*j);
    masterPosEnd.setX(i+6*j);

    fSliderGain7L = new ImageSlider(this, sliderImage);
    fSliderGain7L->setStartPos(masterPosStart);
    fSliderGain7L->setEndPos(masterPosEnd);
    fSliderGain7L->setRange(-12.f,12.f);
    fSliderGain7L->setValue(0.f);
    fSliderGain7L->setCallback(this);
    fSliderGain7L->setInverted(true);

    masterPosStart.setX(i+7*j);
    masterPosEnd.setX(i+7*j);

    fSliderGain8L = new ImageSlider(this, sliderImage);
    fSliderGain8L->setStartPos(masterPosStart);
    fSliderGain8L->setEndPos(masterPosEnd);
    fSliderGain8L->setRange(-12.f,12.f);
    fSliderGain8L->setValue(0.f);
    fSliderGain8L->setCallback(this);
    fSliderGain8L->setInverted(true);

    masterPosStart.setX(i+8*j);
    masterPosEnd.setX(i+8*j);

    fSliderGain9L = new ImageSlider(this, sliderImage);
    fSliderGain9L->setStartPos(masterPosStart);
    fSliderGain9L->setEndPos(masterPosEnd);
    fSliderGain9L->setRange(-12.f,12.f);
    fSliderGain9L->setValue(0.f);
    fSliderGain9L->setCallback(this);
    fSliderGain9L->setInverted(true);

    masterPosStart.setX(i+9*j);
    masterPosEnd.setX(i+9*j);

    fSliderGain10L = new ImageSlider(this, sliderImage);
    fSliderGain10L->setStartPos(masterPosStart);
    fSliderGain10L->setEndPos(masterPosEnd);
    fSliderGain10L->setRange(-12.f,12.f);
    fSliderGain10L->setValue(0.f);
    fSliderGain10L->setCallback(this);
    fSliderGain10L->setInverted(true);

    masterPosStart.setX(i+10*j);
    masterPosEnd.setX(i+10*j);

    fSliderGain11L = new ImageSlider(this, sliderImage);
    fSliderGain11L->setStartPos(masterPosStart);
    fSliderGain11L->setEndPos(masterPosEnd);
    fSliderGain11L->setRange(-12.f,12.f);
    fSliderGain11L->setValue(0.f);
    fSliderGain11L->setCallback(this);
    fSliderGain11L->setInverted(true);

    masterPosStart.setX(i+11*j);
    masterPosEnd.setX(i+11*j);

    fSliderGain12L = new ImageSlider(this, sliderImage);
    fSliderGain12L->setStartPos(masterPosStart);
    fSliderGain12L->setEndPos(masterPosEnd);
    fSliderGain12L->setRange(-12.f,12.f);
    fSliderGain12L->setValue(0.f);
    fSliderGain12L->setCallback(this);
    fSliderGain12L->setInverted(true);

    masterPosStart.setX(i+12*j);
    masterPosEnd.setX(i+12*j);

    fSliderGain13L = new ImageSlider(this, sliderImage);
    fSliderGain13L->setStartPos(masterPosStart);
    fSliderGain13L->setEndPos(masterPosEnd);
    fSliderGain13L->setRange(-12.f,12.f);
    fSliderGain13L->setValue(0.f);
    fSliderGain13L->setCallback(this);
    fSliderGain13L->setInverted(true);

    masterPosStart.setX(i+13*j);
    masterPosEnd.setX(i+13*j);

    fSliderGain14L = new ImageSlider(this, sliderImage);
    fSliderGain14L->setStartPos(masterPosStart);
    fSliderGain14L->setEndPos(masterPosEnd);
    fSliderGain14L->setRange(-12.f,12.f);
    fSliderGain14L->setValue(0.f);
    fSliderGain14L->setCallback(this);
    fSliderGain14L->setInverted(true);

    masterPosStart.setX(i+14*j);
    masterPosEnd.setX(i+14*j);

    fSliderGain15L = new ImageSlider(this, sliderImage);
    fSliderGain15L->setStartPos(masterPosStart);
    fSliderGain15L->setEndPos(masterPosEnd);
    fSliderGain15L->setRange(-12.f,12.f);
    fSliderGain15L->setValue(0.f);
    fSliderGain15L->setCallback(this);
    fSliderGain15L->setInverted(true);

    masterPosStart.setX(i+15*j);
    masterPosEnd.setX(i+15*j);

    fSliderGain16L = new ImageSlider(this, sliderImage);
    fSliderGain16L->setStartPos(masterPosStart);
    fSliderGain16L->setEndPos(masterPosEnd);
    fSliderGain16L->setRange(-12.f,12.f);
    fSliderGain16L->setValue(0.f);
    fSliderGain16L->setCallback(this);
    fSliderGain16L->setInverted(true);

    masterPosStart.setX(i+16*j);
    masterPosEnd.setX(i+16*j);

    fSliderGain17L = new ImageSlider(this, sliderImage);
    fSliderGain17L->setStartPos(masterPosStart);
    fSliderGain17L->setEndPos(masterPosEnd);
    fSliderGain17L->setRange(-12.f,12.f);
    fSliderGain17L->setValue(0.f);
    fSliderGain17L->setCallback(this);
    fSliderGain17L->setInverted(true);

    masterPosStart.setX(i+17*j);
    masterPosEnd.setX(i+17*j);

    fSliderGain18L = new ImageSlider(this, sliderImage);
    fSliderGain18L->setStartPos(masterPosStart);
    fSliderGain18L->setEndPos(masterPosEnd);
    fSliderGain18L->setRange(-12.f,12.f);
    fSliderGain18L->setValue(0.f);
    fSliderGain18L->setCallback(this);
    fSliderGain18L->setInverted(true);

    masterPosStart.setX(i+18*j);
    masterPosEnd.setX(i+18*j);

    fSliderGain19L = new ImageSlider(this, sliderImage);
    fSliderGain19L->setStartPos(masterPosStart);
    fSliderGain19L->setEndPos(masterPosEnd);
    fSliderGain19L->setRange(-12.f,12.f);
    fSliderGain19L->setValue(0.f);
    fSliderGain19L->setCallback(this);
    fSliderGain19L->setInverted(true);

    masterPosStart.setX(i+19*j);
    masterPosEnd.setX(i+19*j);

    fSliderGain20L = new ImageSlider(this, sliderImage);
    fSliderGain20L->setStartPos(masterPosStart);
    fSliderGain20L->setEndPos(masterPosEnd);
    fSliderGain20L->setRange(-12.f,12.f);
    fSliderGain20L->setValue(0.f);
    fSliderGain20L->setCallback(this);
    fSliderGain20L->setInverted(true);

    masterPosStart.setX(i+20*j);
    masterPosEnd.setX(i+20*j);

    fSliderGain21L = new ImageSlider(this, sliderImage);
    fSliderGain21L->setStartPos(masterPosStart);
    fSliderGain21L->setEndPos(masterPosEnd);
    fSliderGain21L->setRange(-12.f,12.f);
    fSliderGain21L->setValue(0.f);
    fSliderGain21L->setCallback(this);
    fSliderGain21L->setInverted(true);

    masterPosStart.setX(i+21*j);
    masterPosEnd.setX(i+21*j);

    fSliderGain22L = new ImageSlider(this, sliderImage);
    fSliderGain22L->setStartPos(masterPosStart);
    fSliderGain22L->setEndPos(masterPosEnd);
    fSliderGain22L->setRange(-12.f,12.f);
    fSliderGain22L->setValue(0.f);
    fSliderGain22L->setCallback(this);
    fSliderGain22L->setInverted(true);

    masterPosStart.setX(i+22*j);
    masterPosEnd.setX(i+22*j);

    fSliderGain23L = new ImageSlider(this, sliderImage);
    fSliderGain23L->setStartPos(masterPosStart);
    fSliderGain23L->setEndPos(masterPosEnd);
    fSliderGain23L->setRange(-12.f,12.f);
    fSliderGain23L->setValue(0.f);
    fSliderGain23L->setCallback(this);
    fSliderGain23L->setInverted(true);

    masterPosStart.setX(i+23*j);
    masterPosEnd.setX(i+23*j);

    fSliderGain24L = new ImageSlider(this, sliderImage);
    fSliderGain24L->setStartPos(masterPosStart);
    fSliderGain24L->setEndPos(masterPosEnd);
    fSliderGain24L->setRange(-12.f,12.f);
    fSliderGain24L->setValue(0.f);
    fSliderGain24L->setCallback(this);
    fSliderGain24L->setInverted(true);

    masterPosStart.setX(i+24*j);
    masterPosEnd.setX(i+24*j);

    fSliderGain25L = new ImageSlider(this, sliderImage);
    fSliderGain25L->setStartPos(masterPosStart);
    fSliderGain25L->setEndPos(masterPosEnd);
    fSliderGain25L->setRange(-12.f,12.f);
    fSliderGain25L->setValue(0.f);
    fSliderGain25L->setCallback(this);
    fSliderGain25L->setInverted(true);

    masterPosStart.setX(i+25*j);
    masterPosEnd.setX(i+25*j);

    fSliderGain26L = new ImageSlider(this, sliderImage);
    fSliderGain26L->setStartPos(masterPosStart);
    fSliderGain26L->setEndPos(masterPosEnd);
    fSliderGain26L->setRange(-12.f,12.f);
    fSliderGain26L->setValue(0.f);
    fSliderGain26L->setCallback(this);
    fSliderGain26L->setInverted(true);

    masterPosStart.setX(i+26*j);
    masterPosEnd.setX(i+26*j);

    fSliderGain27L = new ImageSlider(this, sliderImage);
    fSliderGain27L->setStartPos(masterPosStart);
    fSliderGain27L->setEndPos(masterPosEnd);
    fSliderGain27L->setRange(-12.f,12.f);
    fSliderGain27L->setValue(0.f);
    fSliderGain27L->setCallback(this);
    fSliderGain27L->setInverted(true);

    masterPosStart.setX(i+27*j);
    masterPosEnd.setX(i+27*j);

    fSliderGain28L = new ImageSlider(this, sliderImage);
    fSliderGain28L->setStartPos(masterPosStart);
    fSliderGain28L->setEndPos(masterPosEnd);
    fSliderGain28L->setRange(-12.f,12.f);
    fSliderGain28L->setValue(0.f);
    fSliderGain28L->setCallback(this);
    fSliderGain28L->setInverted(true);

    masterPosStart.setX(i+28*j);
    masterPosEnd.setX(i+28*j);

    fSliderGain29L = new ImageSlider(this, sliderImage);
    fSliderGain29L->setStartPos(masterPosStart);
    fSliderGain29L->setEndPos(masterPosEnd);
    fSliderGain29L->setRange(-12.f,12.f);
    fSliderGain29L->setValue(0.f);
    fSliderGain29L->setCallback(this);
    fSliderGain29L->setInverted(true);

    masterPosStart.setX(i+29*j);
    masterPosEnd.setX(i+29*j);

    fSliderGain30L = new ImageSlider(this, sliderImage);
    fSliderGain30L->setStartPos(masterPosStart);
    fSliderGain30L->setEndPos(masterPosEnd);
    fSliderGain30L->setRange(-12.f,12.f);
    fSliderGain30L->setValue(0.f);
    fSliderGain30L->setCallback(this);
    fSliderGain30L->setInverted(true);

    masterPosStart.setX(i+30*j);
    masterPosEnd.setX(i+30*j);

    fSliderGain31L = new ImageSlider(this, sliderImage);
    fSliderGain31L->setStartPos(masterPosStart);
    fSliderGain31L->setEndPos(masterPosEnd);
    fSliderGain31L->setRange(-12.f,12.f);
    fSliderGain31L->setValue(0.f);
    fSliderGain31L->setCallback(this);
    fSliderGain31L->setInverted(true);

    masterPosStart.setX(24.);
    masterPosEnd.setX(24.);
    masterPosStart.setY(148.);
    masterPosEnd.setY(225.);

    fSliderGain1R = new ImageSlider(this, sliderImage);
    fSliderGain1R->setStartPos(masterPosStart);
    fSliderGain1R->setEndPos(masterPosEnd);
    fSliderGain1R->setRange(-12.f,12.f);
    fSliderGain1R->setValue(0.f);
    fSliderGain1R->setCallback(this);
    fSliderGain1R->setInverted(true);

    masterPosStart.setX(i+j);
    masterPosEnd.setX(i+j);

    fSliderGain2R = new ImageSlider(this, sliderImage);
    fSliderGain2R->setStartPos(masterPosStart);
    fSliderGain2R->setEndPos(masterPosEnd);
    fSliderGain2R->setRange(-12.f,12.f);
    fSliderGain2R->setValue(0.f);
    fSliderGain2R->setCallback(this);
    fSliderGain2R->setInverted(true);

    masterPosStart.setX(i+2*j);
    masterPosEnd.setX(i+2*j);

    fSliderGain3R = new ImageSlider(this, sliderImage);
    fSliderGain3R->setStartPos(masterPosStart);
    fSliderGain3R->setEndPos(masterPosEnd);
    fSliderGain3R->setRange(-12.f,12.f);
    fSliderGain3R->setValue(0.f);
    fSliderGain3R->setCallback(this);
    fSliderGain3R->setInverted(true);

    masterPosStart.setX(i+3*j);
    masterPosEnd.setX(i+3*j);

    fSliderGain4R = new ImageSlider(this, sliderImage);
    fSliderGain4R->setStartPos(masterPosStart);
    fSliderGain4R->setEndPos(masterPosEnd);
    fSliderGain4R->setRange(-12.f,12.f);
    fSliderGain4R->setValue(0.f);
    fSliderGain4R->setCallback(this);
    fSliderGain4R->setInverted(true);

    masterPosStart.setX(i+4*j);
    masterPosEnd.setX(i+4*j);

    fSliderGain5R = new ImageSlider(this, sliderImage);
    fSliderGain5R->setStartPos(masterPosStart);
    fSliderGain5R->setEndPos(masterPosEnd);
    fSliderGain5R->setRange(-12.f,12.f);
    fSliderGain5R->setValue(0.f);
    fSliderGain5R->setCallback(this);
    fSliderGain5R->setInverted(true);

    masterPosStart.setX(i+5*j);
    masterPosEnd.setX(i+5*j);

    fSliderGain6R = new ImageSlider(this, sliderImage);
    fSliderGain6R->setStartPos(masterPosStart);
    fSliderGain6R->setEndPos(masterPosEnd);
    fSliderGain6R->setRange(-12.f,12.f);
    fSliderGain6R->setValue(0.f);
    fSliderGain6R->setCallback(this);
    fSliderGain6R->setInverted(true);

    masterPosStart.setX(i+6*j);
    masterPosEnd.setX(i+6*j);

    fSliderGain7R = new ImageSlider(this, sliderImage);
    fSliderGain7R->setStartPos(masterPosStart);
    fSliderGain7R->setEndPos(masterPosEnd);
    fSliderGain7R->setRange(-12.f,12.f);
    fSliderGain7R->setValue(0.f);
    fSliderGain7R->setCallback(this);
    fSliderGain7R->setInverted(true);

    masterPosStart.setX(i+7*j);
    masterPosEnd.setX(i+7*j);

    fSliderGain8R = new ImageSlider(this, sliderImage);
    fSliderGain8R->setStartPos(masterPosStart);
    fSliderGain8R->setEndPos(masterPosEnd);
    fSliderGain8R->setRange(-12.f,12.f);
    fSliderGain8R->setValue(0.f);
    fSliderGain8R->setCallback(this);
    fSliderGain8R->setInverted(true);

    masterPosStart.setX(i+8*j);
    masterPosEnd.setX(i+8*j);

    fSliderGain9R = new ImageSlider(this, sliderImage);
    fSliderGain9R->setStartPos(masterPosStart);
    fSliderGain9R->setEndPos(masterPosEnd);
    fSliderGain9R->setRange(-12.f,12.f);
    fSliderGain9R->setValue(0.f);
    fSliderGain9R->setCallback(this);
    fSliderGain9R->setInverted(true);

    masterPosStart.setX(i+9*j);
    masterPosEnd.setX(i+9*j);

    fSliderGain10R = new ImageSlider(this, sliderImage);
    fSliderGain10R->setStartPos(masterPosStart);
    fSliderGain10R->setEndPos(masterPosEnd);
    fSliderGain10R->setRange(-12.f,12.f);
    fSliderGain10R->setValue(0.f);
    fSliderGain10R->setCallback(this);
    fSliderGain10R->setInverted(true);

    masterPosStart.setX(i+10*j);
    masterPosEnd.setX(i+10*j);

    fSliderGain11R = new ImageSlider(this, sliderImage);
    fSliderGain11R->setStartPos(masterPosStart);
    fSliderGain11R->setEndPos(masterPosEnd);
    fSliderGain11R->setRange(-12.f,12.f);
    fSliderGain11R->setValue(0.f);
    fSliderGain11R->setCallback(this);
    fSliderGain11R->setInverted(true);

    masterPosStart.setX(i+11*j);
    masterPosEnd.setX(i+11*j);

    fSliderGain12R = new ImageSlider(this, sliderImage);
    fSliderGain12R->setStartPos(masterPosStart);
    fSliderGain12R->setEndPos(masterPosEnd);
    fSliderGain12R->setRange(-12.f,12.f);
    fSliderGain12R->setValue(0.f);
    fSliderGain12R->setCallback(this);
    fSliderGain12R->setInverted(true);

    masterPosStart.setX(i+12*j);
    masterPosEnd.setX(i+12*j);

    fSliderGain13R = new ImageSlider(this, sliderImage);
    fSliderGain13R->setStartPos(masterPosStart);
    fSliderGain13R->setEndPos(masterPosEnd);
    fSliderGain13R->setRange(-12.f,12.f);
    fSliderGain13R->setValue(0.f);
    fSliderGain13R->setCallback(this);
    fSliderGain13R->setInverted(true);

    masterPosStart.setX(i+13*j);
    masterPosEnd.setX(i+13*j);

    fSliderGain14R = new ImageSlider(this, sliderImage);
    fSliderGain14R->setStartPos(masterPosStart);
    fSliderGain14R->setEndPos(masterPosEnd);
    fSliderGain14R->setRange(-12.f,12.f);
    fSliderGain14R->setValue(0.f);
    fSliderGain14R->setCallback(this);
    fSliderGain14R->setInverted(true);

    masterPosStart.setX(i+14*j);
    masterPosEnd.setX(i+14*j);

    fSliderGain15R = new ImageSlider(this, sliderImage);
    fSliderGain15R->setStartPos(masterPosStart);
    fSliderGain15R->setEndPos(masterPosEnd);
    fSliderGain15R->setRange(-12.f,12.f);
    fSliderGain15R->setValue(0.f);
    fSliderGain15R->setCallback(this);
    fSliderGain15R->setInverted(true);

    masterPosStart.setX(i+15*j);
    masterPosEnd.setX(i+15*j);

    fSliderGain16R = new ImageSlider(this, sliderImage);
    fSliderGain16R->setStartPos(masterPosStart);
    fSliderGain16R->setEndPos(masterPosEnd);
    fSliderGain16R->setRange(-12.f,12.f);
    fSliderGain16R->setValue(0.f);
    fSliderGain16R->setCallback(this);
    fSliderGain16R->setInverted(true);

    masterPosStart.setX(i+16*j);
    masterPosEnd.setX(i+16*j);

    fSliderGain17R = new ImageSlider(this, sliderImage);
    fSliderGain17R->setStartPos(masterPosStart);
    fSliderGain17R->setEndPos(masterPosEnd);
    fSliderGain17R->setRange(-12.f,12.f);
    fSliderGain17R->setValue(0.f);
    fSliderGain17R->setCallback(this);
    fSliderGain17R->setInverted(true);

    masterPosStart.setX(i+17*j);
    masterPosEnd.setX(i+17*j);

    fSliderGain18R = new ImageSlider(this, sliderImage);
    fSliderGain18R->setStartPos(masterPosStart);
    fSliderGain18R->setEndPos(masterPosEnd);
    fSliderGain18R->setRange(-12.f,12.f);
    fSliderGain18R->setValue(0.f);
    fSliderGain18R->setCallback(this);
    fSliderGain18R->setInverted(true);

    masterPosStart.setX(i+18*j);
    masterPosEnd.setX(i+18*j);

    fSliderGain19R = new ImageSlider(this, sliderImage);
    fSliderGain19R->setStartPos(masterPosStart);
    fSliderGain19R->setEndPos(masterPosEnd);
    fSliderGain19R->setRange(-12.f,12.f);
    fSliderGain19R->setValue(0.f);
    fSliderGain19R->setCallback(this);
    fSliderGain19R->setInverted(true);

    masterPosStart.setX(i+19*j);
    masterPosEnd.setX(i+19*j);

    fSliderGain20R = new ImageSlider(this, sliderImage);
    fSliderGain20R->setStartPos(masterPosStart);
    fSliderGain20R->setEndPos(masterPosEnd);
    fSliderGain20R->setRange(-12.f,12.f);
    fSliderGain20R->setValue(0.f);
    fSliderGain20R->setCallback(this);
    fSliderGain20R->setInverted(true);

    masterPosStart.setX(i+20*j);
    masterPosEnd.setX(i+20*j);

    fSliderGain21R = new ImageSlider(this, sliderImage);
    fSliderGain21R->setStartPos(masterPosStart);
    fSliderGain21R->setEndPos(masterPosEnd);
    fSliderGain21R->setRange(-12.f,12.f);
    fSliderGain21R->setValue(0.f);
    fSliderGain21R->setCallback(this);
    fSliderGain21R->setInverted(true);

    masterPosStart.setX(i+21*j);
    masterPosEnd.setX(i+21*j);

    fSliderGain22R = new ImageSlider(this, sliderImage);
    fSliderGain22R->setStartPos(masterPosStart);
    fSliderGain22R->setEndPos(masterPosEnd);
    fSliderGain22R->setRange(-12.f,12.f);
    fSliderGain22R->setValue(0.f);
    fSliderGain22R->setCallback(this);
    fSliderGain22R->setInverted(true);

    masterPosStart.setX(i+22*j);
    masterPosEnd.setX(i+22*j);

    fSliderGain23R = new ImageSlider(this, sliderImage);
    fSliderGain23R->setStartPos(masterPosStart);
    fSliderGain23R->setEndPos(masterPosEnd);
    fSliderGain23R->setRange(-12.f,12.f);
    fSliderGain23R->setValue(0.f);
    fSliderGain23R->setCallback(this);
    fSliderGain23R->setInverted(true);

    masterPosStart.setX(i+23*j);
    masterPosEnd.setX(i+23*j);

    fSliderGain24R = new ImageSlider(this, sliderImage);
    fSliderGain24R->setStartPos(masterPosStart);
    fSliderGain24R->setEndPos(masterPosEnd);
    fSliderGain24R->setRange(-12.f,12.f);
    fSliderGain24R->setValue(0.f);
    fSliderGain24R->setCallback(this);
    fSliderGain24R->setInverted(true);

    masterPosStart.setX(i+24*j);
    masterPosEnd.setX(i+24*j);

    fSliderGain25R = new ImageSlider(this, sliderImage);
    fSliderGain25R->setStartPos(masterPosStart);
    fSliderGain25R->setEndPos(masterPosEnd);
    fSliderGain25R->setRange(-12.f,12.f);
    fSliderGain25R->setValue(0.f);
    fSliderGain25R->setCallback(this);
    fSliderGain25R->setInverted(true);

    masterPosStart.setX(i+25*j);
    masterPosEnd.setX(i+25*j);

    fSliderGain26R = new ImageSlider(this, sliderImage);
    fSliderGain26R->setStartPos(masterPosStart);
    fSliderGain26R->setEndPos(masterPosEnd);
    fSliderGain26R->setRange(-12.f,12.f);
    fSliderGain26R->setValue(0.f);
    fSliderGain26R->setCallback(this);
    fSliderGain26R->setInverted(true);

    masterPosStart.setX(i+26*j);
    masterPosEnd.setX(i+26*j);

    fSliderGain27R = new ImageSlider(this, sliderImage);
    fSliderGain27R->setStartPos(masterPosStart);
    fSliderGain27R->setEndPos(masterPosEnd);
    fSliderGain27R->setRange(-12.f,12.f);
    fSliderGain27R->setValue(0.f);
    fSliderGain27R->setCallback(this);
    fSliderGain27R->setInverted(true);

    masterPosStart.setX(i+27*j);
    masterPosEnd.setX(i+27*j);

    fSliderGain28R = new ImageSlider(this, sliderImage);
    fSliderGain28R->setStartPos(masterPosStart);
    fSliderGain28R->setEndPos(masterPosEnd);
    fSliderGain28R->setRange(-12.f,12.f);
    fSliderGain28R->setValue(0.f);
    fSliderGain28R->setCallback(this);
    fSliderGain28R->setInverted(true);

    masterPosStart.setX(i+28*j);
    masterPosEnd.setX(i+28*j);

    fSliderGain29R = new ImageSlider(this, sliderImage);
    fSliderGain29R->setStartPos(masterPosStart);
    fSliderGain29R->setEndPos(masterPosEnd);
    fSliderGain29R->setRange(-12.f,12.f);
    fSliderGain29R->setValue(0.f);
    fSliderGain29R->setCallback(this);
    fSliderGain29R->setInverted(true);

    masterPosStart.setX(i+29*j);
    masterPosEnd.setX(i+29*j);

    fSliderGain30R = new ImageSlider(this, sliderImage);
    fSliderGain30R->setStartPos(masterPosStart);
    fSliderGain30R->setEndPos(masterPosEnd);
    fSliderGain30R->setRange(-12.f,12.f);
    fSliderGain30R->setValue(0.f);
    fSliderGain30R->setCallback(this);
    fSliderGain30R->setInverted(true);

    masterPosStart.setX(i+30*j);
    masterPosEnd.setX(i+30*j);

    fSliderGain31R = new ImageSlider(this, sliderImage);
    fSliderGain31R->setStartPos(masterPosStart);
    fSliderGain31R->setEndPos(masterPosEnd);
    fSliderGain31R->setRange(-12.f,12.f);
    fSliderGain31R->setValue(0.f);
    fSliderGain31R->setCallback(this);
    fSliderGain31R->setInverted(true);

    // set default values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamGEQ31X2UI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamGEQ31X2Plugin::paramMaster:
    	fSliderMaster->setValue(value);
	break;
    case ZamGEQ31X2Plugin::paramGain1L:
        fSliderGain1L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain2L:
        fSliderGain2L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain3L:
        fSliderGain3L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain4L:
        fSliderGain4L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain5L:
        fSliderGain5L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain6L:
        fSliderGain6L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain7L:
        fSliderGain7L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain8L:
        fSliderGain8L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain9L:
        fSliderGain9L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain10L:
        fSliderGain10L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain11L:
        fSliderGain11L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain12L:
        fSliderGain12L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain13L:
        fSliderGain13L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain14L:
        fSliderGain14L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain15L:
        fSliderGain15L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain16L:
        fSliderGain16L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain17L:
        fSliderGain17L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain18L:
        fSliderGain18L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain19L:
        fSliderGain19L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain20L:
        fSliderGain20L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain21L:
        fSliderGain21L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain22L:
        fSliderGain22L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain23L:
        fSliderGain23L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain24L:
        fSliderGain24L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain25L:
        fSliderGain25L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain26L:
        fSliderGain26L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain27L:
        fSliderGain27L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain28L:
        fSliderGain28L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain29L:
        fSliderGain29L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain30L:
        fSliderGain30L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain31L:
        fSliderGain31L->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain1R:
        fSliderGain1R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain2R:
        fSliderGain2R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain3R:
        fSliderGain3R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain4R:
        fSliderGain4R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain5R:
        fSliderGain5R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain6R:
        fSliderGain6R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain7R:
        fSliderGain7R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain8R:
        fSliderGain8R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain9R:
        fSliderGain9R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain10R:
        fSliderGain10R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain11R:
        fSliderGain11R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain12R:
        fSliderGain12R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain13R:
        fSliderGain13R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain14R:
        fSliderGain14R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain15R:
        fSliderGain15R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain16R:
        fSliderGain16R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain17R:
        fSliderGain17R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain18R:
        fSliderGain18R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain19R:
        fSliderGain19R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain20R:
        fSliderGain20R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain21R:
        fSliderGain21R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain22R:
        fSliderGain22R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain23R:
        fSliderGain23R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain24R:
        fSliderGain24R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain25R:
        fSliderGain25R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain26R:
        fSliderGain26R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain27R:
        fSliderGain27R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain28R:
        fSliderGain28R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain29R:
        fSliderGain29R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain30R:
        fSliderGain30R->setValue(value);
        break;
    case ZamGEQ31X2Plugin::paramGain31R:
        fSliderGain31R->setValue(value);
        break;
    }
}

void ZamGEQ31X2UI::programLoaded(uint32_t index)
{
    if (index != 0)
        return;

    // Default values

    fSliderMaster->setValue(0.f);
    fSliderGain1L->setValue(0.0f);
    fSliderGain2L->setValue(0.0f);
    fSliderGain3L->setValue(0.0f);
    fSliderGain4L->setValue(0.0f);
    fSliderGain5L->setValue(0.0f);
    fSliderGain6L->setValue(0.0f);
    fSliderGain7L->setValue(0.0f);
    fSliderGain8L->setValue(0.0f);
    fSliderGain9L->setValue(0.0f);
    fSliderGain10L->setValue(0.0f);
    fSliderGain11L->setValue(0.0f);
    fSliderGain12L->setValue(0.0f);
    fSliderGain13L->setValue(0.0f);
    fSliderGain14L->setValue(0.0f);
    fSliderGain15L->setValue(0.0f);
    fSliderGain16L->setValue(0.0f);
    fSliderGain17L->setValue(0.0f);
    fSliderGain18L->setValue(0.0f);
    fSliderGain19L->setValue(0.0f);
    fSliderGain20L->setValue(0.0f);
    fSliderGain21L->setValue(0.0f);
    fSliderGain22L->setValue(0.0f);
    fSliderGain23L->setValue(0.0f);
    fSliderGain24L->setValue(0.0f);
    fSliderGain25L->setValue(0.0f);
    fSliderGain26L->setValue(0.0f);
    fSliderGain27L->setValue(0.0f);
    fSliderGain28L->setValue(0.0f);
    fSliderGain29L->setValue(0.0f);
    fSliderGain30L->setValue(0.0f);
    fSliderGain31L->setValue(0.0f);
    fSliderGain1R->setValue(0.0f);
    fSliderGain2R->setValue(0.0f);
    fSliderGain3R->setValue(0.0f);
    fSliderGain4R->setValue(0.0f);
    fSliderGain5R->setValue(0.0f);
    fSliderGain6R->setValue(0.0f);
    fSliderGain7R->setValue(0.0f);
    fSliderGain8R->setValue(0.0f);
    fSliderGain9R->setValue(0.0f);
    fSliderGain10R->setValue(0.0f);
    fSliderGain11R->setValue(0.0f);
    fSliderGain12R->setValue(0.0f);
    fSliderGain13R->setValue(0.0f);
    fSliderGain14R->setValue(0.0f);
    fSliderGain15R->setValue(0.0f);
    fSliderGain16R->setValue(0.0f);
    fSliderGain17R->setValue(0.0f);
    fSliderGain18R->setValue(0.0f);
    fSliderGain19R->setValue(0.0f);
    fSliderGain20R->setValue(0.0f);
    fSliderGain21R->setValue(0.0f);
    fSliderGain22R->setValue(0.0f);
    fSliderGain23R->setValue(0.0f);
    fSliderGain24R->setValue(0.0f);
    fSliderGain25R->setValue(0.0f);
    fSliderGain26R->setValue(0.0f);
    fSliderGain27R->setValue(0.0f);
    fSliderGain28R->setValue(0.0f);
    fSliderGain29R->setValue(0.0f);
    fSliderGain30R->setValue(0.0f);
    fSliderGain31R->setValue(0.0f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamGEQ31X2UI::imageSliderDragStarted(ImageSlider* slider)
{
    if (slider == fSliderMaster)
        editParameter(ZamGEQ31X2Plugin::paramMaster, true);
    else if (slider == fSliderGain1L)
        editParameter(ZamGEQ31X2Plugin::paramGain1L, true);
    else if (slider == fSliderGain2L)
        editParameter(ZamGEQ31X2Plugin::paramGain2L, true);
    else if (slider == fSliderGain3L)
        editParameter(ZamGEQ31X2Plugin::paramGain3L, true);
    else if (slider == fSliderGain4L)
        editParameter(ZamGEQ31X2Plugin::paramGain4L, true);
    else if (slider == fSliderGain5L)
        editParameter(ZamGEQ31X2Plugin::paramGain5L, true);
    else if (slider == fSliderGain6L)
        editParameter(ZamGEQ31X2Plugin::paramGain6L, true);
    else if (slider == fSliderGain7L)
        editParameter(ZamGEQ31X2Plugin::paramGain7L, true);
    else if (slider == fSliderGain8L)
        editParameter(ZamGEQ31X2Plugin::paramGain8L, true);
    else if (slider == fSliderGain9L)
        editParameter(ZamGEQ31X2Plugin::paramGain9L, true);
    else if (slider == fSliderGain10L)
        editParameter(ZamGEQ31X2Plugin::paramGain10L, true);
    else if (slider == fSliderGain11L)
        editParameter(ZamGEQ31X2Plugin::paramGain11L, true);
    else if (slider == fSliderGain12L)
        editParameter(ZamGEQ31X2Plugin::paramGain12L, true);
    else if (slider == fSliderGain13L)
        editParameter(ZamGEQ31X2Plugin::paramGain13L, true);
    else if (slider == fSliderGain14L)
        editParameter(ZamGEQ31X2Plugin::paramGain14L, true);
    else if (slider == fSliderGain15L)
        editParameter(ZamGEQ31X2Plugin::paramGain15L, true);
    else if (slider == fSliderGain16L)
        editParameter(ZamGEQ31X2Plugin::paramGain16L, true);
    else if (slider == fSliderGain17L)
        editParameter(ZamGEQ31X2Plugin::paramGain17L, true);
    else if (slider == fSliderGain18L)
        editParameter(ZamGEQ31X2Plugin::paramGain18L, true);
    else if (slider == fSliderGain19L)
        editParameter(ZamGEQ31X2Plugin::paramGain19L, true);
    else if (slider == fSliderGain20L)
        editParameter(ZamGEQ31X2Plugin::paramGain20L, true);
    else if (slider == fSliderGain21L)
        editParameter(ZamGEQ31X2Plugin::paramGain21L, true);
    else if (slider == fSliderGain22L)
        editParameter(ZamGEQ31X2Plugin::paramGain22L, true);
    else if (slider == fSliderGain23L)
        editParameter(ZamGEQ31X2Plugin::paramGain23L, true);
    else if (slider == fSliderGain24L)
        editParameter(ZamGEQ31X2Plugin::paramGain24L, true);
    else if (slider == fSliderGain25L)
        editParameter(ZamGEQ31X2Plugin::paramGain25L, true);
    else if (slider == fSliderGain26L)
        editParameter(ZamGEQ31X2Plugin::paramGain26L, true);
    else if (slider == fSliderGain27L)
        editParameter(ZamGEQ31X2Plugin::paramGain27L, true);
    else if (slider == fSliderGain28L)
        editParameter(ZamGEQ31X2Plugin::paramGain28L, true);
    else if (slider == fSliderGain29L)
        editParameter(ZamGEQ31X2Plugin::paramGain29L, true);
    else if (slider == fSliderGain30L)
        editParameter(ZamGEQ31X2Plugin::paramGain30L, true);
    else if (slider == fSliderGain31L)
        editParameter(ZamGEQ31X2Plugin::paramGain31L, true);
    else if (slider == fSliderGain1R)
        editParameter(ZamGEQ31X2Plugin::paramGain1R, true);
    else if (slider == fSliderGain2R)
        editParameter(ZamGEQ31X2Plugin::paramGain2R, true);
    else if (slider == fSliderGain3R)
        editParameter(ZamGEQ31X2Plugin::paramGain3R, true);
    else if (slider == fSliderGain4R)
        editParameter(ZamGEQ31X2Plugin::paramGain4R, true);
    else if (slider == fSliderGain5R)
        editParameter(ZamGEQ31X2Plugin::paramGain5R, true);
    else if (slider == fSliderGain6R)
        editParameter(ZamGEQ31X2Plugin::paramGain6R, true);
    else if (slider == fSliderGain7R)
        editParameter(ZamGEQ31X2Plugin::paramGain7R, true);
    else if (slider == fSliderGain8R)
        editParameter(ZamGEQ31X2Plugin::paramGain8R, true);
    else if (slider == fSliderGain9R)
        editParameter(ZamGEQ31X2Plugin::paramGain9R, true);
    else if (slider == fSliderGain10R)
        editParameter(ZamGEQ31X2Plugin::paramGain10R, true);
    else if (slider == fSliderGain11R)
        editParameter(ZamGEQ31X2Plugin::paramGain11R, true);
    else if (slider == fSliderGain12R)
        editParameter(ZamGEQ31X2Plugin::paramGain12R, true);
    else if (slider == fSliderGain13R)
        editParameter(ZamGEQ31X2Plugin::paramGain13R, true);
    else if (slider == fSliderGain14R)
        editParameter(ZamGEQ31X2Plugin::paramGain14R, true);
    else if (slider == fSliderGain15R)
        editParameter(ZamGEQ31X2Plugin::paramGain15R, true);
    else if (slider == fSliderGain16R)
        editParameter(ZamGEQ31X2Plugin::paramGain16R, true);
    else if (slider == fSliderGain17R)
        editParameter(ZamGEQ31X2Plugin::paramGain17R, true);
    else if (slider == fSliderGain18R)
        editParameter(ZamGEQ31X2Plugin::paramGain18R, true);
    else if (slider == fSliderGain19R)
        editParameter(ZamGEQ31X2Plugin::paramGain19R, true);
    else if (slider == fSliderGain20R)
        editParameter(ZamGEQ31X2Plugin::paramGain20R, true);
    else if (slider == fSliderGain21R)
        editParameter(ZamGEQ31X2Plugin::paramGain21R, true);
    else if (slider == fSliderGain22R)
        editParameter(ZamGEQ31X2Plugin::paramGain22R, true);
    else if (slider == fSliderGain23R)
        editParameter(ZamGEQ31X2Plugin::paramGain23R, true);
    else if (slider == fSliderGain24R)
        editParameter(ZamGEQ31X2Plugin::paramGain24R, true);
    else if (slider == fSliderGain25R)
        editParameter(ZamGEQ31X2Plugin::paramGain25R, true);
    else if (slider == fSliderGain26R)
        editParameter(ZamGEQ31X2Plugin::paramGain26R, true);
    else if (slider == fSliderGain27R)
        editParameter(ZamGEQ31X2Plugin::paramGain27R, true);
    else if (slider == fSliderGain28R)
        editParameter(ZamGEQ31X2Plugin::paramGain28R, true);
    else if (slider == fSliderGain29R)
        editParameter(ZamGEQ31X2Plugin::paramGain29R, true);
    else if (slider == fSliderGain30R)
        editParameter(ZamGEQ31X2Plugin::paramGain30R, true);
    else if (slider == fSliderGain31R)
        editParameter(ZamGEQ31X2Plugin::paramGain31R, true);
}

void ZamGEQ31X2UI::imageSliderDragFinished(ImageSlider* slider)
{
    if (slider == fSliderMaster)
        editParameter(ZamGEQ31X2Plugin::paramMaster, false);
    else if (slider == fSliderGain1L)
        editParameter(ZamGEQ31X2Plugin::paramGain1L, false);
    else if (slider == fSliderGain2L)
        editParameter(ZamGEQ31X2Plugin::paramGain2L, false);
    else if (slider == fSliderGain3L)
        editParameter(ZamGEQ31X2Plugin::paramGain3L, false);
    else if (slider == fSliderGain4L)
        editParameter(ZamGEQ31X2Plugin::paramGain4L, false);
    else if (slider == fSliderGain5L)
        editParameter(ZamGEQ31X2Plugin::paramGain5L, false);
    else if (slider == fSliderGain6L)
        editParameter(ZamGEQ31X2Plugin::paramGain6L, false);
    else if (slider == fSliderGain7L)
        editParameter(ZamGEQ31X2Plugin::paramGain7L, false);
    else if (slider == fSliderGain8L)
        editParameter(ZamGEQ31X2Plugin::paramGain8L, false);
    else if (slider == fSliderGain9L)
        editParameter(ZamGEQ31X2Plugin::paramGain9L, false);
    else if (slider == fSliderGain10L)
        editParameter(ZamGEQ31X2Plugin::paramGain10L, false);
    else if (slider == fSliderGain11L)
        editParameter(ZamGEQ31X2Plugin::paramGain11L, false);
    else if (slider == fSliderGain12L)
        editParameter(ZamGEQ31X2Plugin::paramGain12L, false);
    else if (slider == fSliderGain13L)
        editParameter(ZamGEQ31X2Plugin::paramGain13L, false);
    else if (slider == fSliderGain14L)
        editParameter(ZamGEQ31X2Plugin::paramGain14L, false);
    else if (slider == fSliderGain15L)
        editParameter(ZamGEQ31X2Plugin::paramGain15L, false);
    else if (slider == fSliderGain16L)
        editParameter(ZamGEQ31X2Plugin::paramGain16L, false);
    else if (slider == fSliderGain17L)
        editParameter(ZamGEQ31X2Plugin::paramGain17L, false);
    else if (slider == fSliderGain18L)
        editParameter(ZamGEQ31X2Plugin::paramGain18L, false);
    else if (slider == fSliderGain19L)
        editParameter(ZamGEQ31X2Plugin::paramGain19L, false);
    else if (slider == fSliderGain20L)
        editParameter(ZamGEQ31X2Plugin::paramGain20L, false);
    else if (slider == fSliderGain21L)
        editParameter(ZamGEQ31X2Plugin::paramGain21L, false);
    else if (slider == fSliderGain22L)
        editParameter(ZamGEQ31X2Plugin::paramGain22L, false);
    else if (slider == fSliderGain23L)
        editParameter(ZamGEQ31X2Plugin::paramGain23L, false);
    else if (slider == fSliderGain24L)
        editParameter(ZamGEQ31X2Plugin::paramGain24L, false);
    else if (slider == fSliderGain25L)
        editParameter(ZamGEQ31X2Plugin::paramGain25L, false);
    else if (slider == fSliderGain26L)
        editParameter(ZamGEQ31X2Plugin::paramGain26L, false);
    else if (slider == fSliderGain27L)
        editParameter(ZamGEQ31X2Plugin::paramGain27L, false);
    else if (slider == fSliderGain28L)
        editParameter(ZamGEQ31X2Plugin::paramGain28L, false);
    else if (slider == fSliderGain29L)
        editParameter(ZamGEQ31X2Plugin::paramGain29L, false);
    else if (slider == fSliderGain30L)
        editParameter(ZamGEQ31X2Plugin::paramGain30L, false);
    else if (slider == fSliderGain31L)
        editParameter(ZamGEQ31X2Plugin::paramGain31L, false);
    else if (slider == fSliderGain1R)
        editParameter(ZamGEQ31X2Plugin::paramGain1R, false);
    else if (slider == fSliderGain2R)
        editParameter(ZamGEQ31X2Plugin::paramGain2R, false);
    else if (slider == fSliderGain3R)
        editParameter(ZamGEQ31X2Plugin::paramGain3R, false);
    else if (slider == fSliderGain4R)
        editParameter(ZamGEQ31X2Plugin::paramGain4R, false);
    else if (slider == fSliderGain5R)
        editParameter(ZamGEQ31X2Plugin::paramGain5R, false);
    else if (slider == fSliderGain6R)
        editParameter(ZamGEQ31X2Plugin::paramGain6R, false);
    else if (slider == fSliderGain7R)
        editParameter(ZamGEQ31X2Plugin::paramGain7R, false);
    else if (slider == fSliderGain8R)
        editParameter(ZamGEQ31X2Plugin::paramGain8R, false);
    else if (slider == fSliderGain9R)
        editParameter(ZamGEQ31X2Plugin::paramGain9R, false);
    else if (slider == fSliderGain10R)
        editParameter(ZamGEQ31X2Plugin::paramGain10R, false);
    else if (slider == fSliderGain11R)
        editParameter(ZamGEQ31X2Plugin::paramGain11R, false);
    else if (slider == fSliderGain12R)
        editParameter(ZamGEQ31X2Plugin::paramGain12R, false);
    else if (slider == fSliderGain13R)
        editParameter(ZamGEQ31X2Plugin::paramGain13R, false);
    else if (slider == fSliderGain14R)
        editParameter(ZamGEQ31X2Plugin::paramGain14R, false);
    else if (slider == fSliderGain15R)
        editParameter(ZamGEQ31X2Plugin::paramGain15R, false);
    else if (slider == fSliderGain16R)
        editParameter(ZamGEQ31X2Plugin::paramGain16R, false);
    else if (slider == fSliderGain17R)
        editParameter(ZamGEQ31X2Plugin::paramGain17R, false);
    else if (slider == fSliderGain18R)
        editParameter(ZamGEQ31X2Plugin::paramGain18R, false);
    else if (slider == fSliderGain19R)
        editParameter(ZamGEQ31X2Plugin::paramGain19R, false);
    else if (slider == fSliderGain20R)
        editParameter(ZamGEQ31X2Plugin::paramGain20R, false);
    else if (slider == fSliderGain21R)
        editParameter(ZamGEQ31X2Plugin::paramGain21R, false);
    else if (slider == fSliderGain22R)
        editParameter(ZamGEQ31X2Plugin::paramGain22R, false);
    else if (slider == fSliderGain23R)
        editParameter(ZamGEQ31X2Plugin::paramGain23R, false);
    else if (slider == fSliderGain24R)
        editParameter(ZamGEQ31X2Plugin::paramGain24R, false);
    else if (slider == fSliderGain25R)
        editParameter(ZamGEQ31X2Plugin::paramGain25R, false);
    else if (slider == fSliderGain26R)
        editParameter(ZamGEQ31X2Plugin::paramGain26R, false);
    else if (slider == fSliderGain27R)
        editParameter(ZamGEQ31X2Plugin::paramGain27R, false);
    else if (slider == fSliderGain28R)
        editParameter(ZamGEQ31X2Plugin::paramGain28R, false);
    else if (slider == fSliderGain29R)
        editParameter(ZamGEQ31X2Plugin::paramGain29R, false);
    else if (slider == fSliderGain30R)
        editParameter(ZamGEQ31X2Plugin::paramGain30R, false);
    else if (slider == fSliderGain31R)
        editParameter(ZamGEQ31X2Plugin::paramGain31R, false);
}

void ZamGEQ31X2UI::imageSliderValueChanged(ImageSlider* slider, float value)
{
    if (slider == fSliderMaster)
        setParameterValue(ZamGEQ31X2Plugin::paramMaster, value);
    else if (slider == fSliderGain1L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain1L, value);
    else if (slider == fSliderGain2L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain2L, value);
    else if (slider == fSliderGain3L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain3L, value);
    else if (slider == fSliderGain4L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain4L, value);
    else if (slider == fSliderGain5L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain5L, value);
    else if (slider == fSliderGain6L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain6L, value);
    else if (slider == fSliderGain7L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain7L, value);
    else if (slider == fSliderGain8L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain8L, value);
    else if (slider == fSliderGain9L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain9L, value);
    else if (slider == fSliderGain10L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain10L, value);
    else if (slider == fSliderGain11L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain11L, value);
    else if (slider == fSliderGain12L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain12L, value);
    else if (slider == fSliderGain13L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain13L, value);
    else if (slider == fSliderGain14L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain14L, value);
    else if (slider == fSliderGain15L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain15L, value);
    else if (slider == fSliderGain16L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain16L, value);
    else if (slider == fSliderGain17L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain17L, value);
    else if (slider == fSliderGain18L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain18L, value);
    else if (slider == fSliderGain19L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain19L, value);
    else if (slider == fSliderGain20L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain20L, value);
    else if (slider == fSliderGain21L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain21L, value);
    else if (slider == fSliderGain22L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain22L, value);
    else if (slider == fSliderGain23L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain23L, value);
    else if (slider == fSliderGain24L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain24L, value);
    else if (slider == fSliderGain25L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain25L, value);
    else if (slider == fSliderGain26L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain26L, value);
    else if (slider == fSliderGain27L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain27L, value);
    else if (slider == fSliderGain28L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain28L, value);
    else if (slider == fSliderGain29L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain29L, value);
    else if (slider == fSliderGain30L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain30L, value);
    else if (slider == fSliderGain31L)
        setParameterValue(ZamGEQ31X2Plugin::paramGain31L, value);
    else if (slider == fSliderGain1R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain1R, value);
    else if (slider == fSliderGain2R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain2R, value);
    else if (slider == fSliderGain3R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain3R, value);
    else if (slider == fSliderGain4R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain4R, value);
    else if (slider == fSliderGain5R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain5R, value);
    else if (slider == fSliderGain6R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain6R, value);
    else if (slider == fSliderGain7R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain7R, value);
    else if (slider == fSliderGain8R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain8R, value);
    else if (slider == fSliderGain9R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain9R, value);
    else if (slider == fSliderGain10R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain10R, value);
    else if (slider == fSliderGain11R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain11R, value);
    else if (slider == fSliderGain12R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain12R, value);
    else if (slider == fSliderGain13R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain13R, value);
    else if (slider == fSliderGain14R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain14R, value);
    else if (slider == fSliderGain15R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain15R, value);
    else if (slider == fSliderGain16R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain16R, value);
    else if (slider == fSliderGain17R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain17R, value);
    else if (slider == fSliderGain18R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain18R, value);
    else if (slider == fSliderGain19R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain19R, value);
    else if (slider == fSliderGain20R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain20R, value);
    else if (slider == fSliderGain21R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain21R, value);
    else if (slider == fSliderGain22R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain22R, value);
    else if (slider == fSliderGain23R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain23R, value);
    else if (slider == fSliderGain24R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain24R, value);
    else if (slider == fSliderGain25R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain25R, value);
    else if (slider == fSliderGain26R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain26R, value);
    else if (slider == fSliderGain27R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain27R, value);
    else if (slider == fSliderGain28R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain28R, value);
    else if (slider == fSliderGain29R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain29R, value);
    else if (slider == fSliderGain30R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain30R, value);
    else if (slider == fSliderGain31R)
        setParameterValue(ZamGEQ31X2Plugin::paramGain31R, value);
}

void ZamGEQ31X2UI::onDisplay()
{
    fImgBackground.draw();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamGEQ31X2UI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
