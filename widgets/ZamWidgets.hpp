/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2016 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

// This is a custom knob widget for DPF with numeric label for zam-plugins

#ifndef DGL_ZAM_WIDGETS_HPP_INCLUDED
#define DGL_ZAM_WIDGETS_HPP_INCLUDED

#include "../dpf/dgl/Image.hpp"
#include "../dpf/dgl/Widget.hpp"
#include "../dpf/dgl/Window.hpp"
#include "../dpf/dgl/NanoVG.hpp"
#include "../dpf/dgl/src/Common.hpp"
#include "../dpf/dgl/src/WidgetPrivateData.hpp"

START_NAMESPACE_DGL

// -----------------------------------------------------------------------

class ZamKnob : public Widget,
                public NanoVG
{
public:
    enum Orientation {
        Horizontal,
        Vertical
    };

    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void imageKnobDragStarted(ZamKnob* imageKnob) = 0;
        virtual void imageKnobDragFinished(ZamKnob* imageKnob) = 0;
        virtual void imageKnobValueChanged(ZamKnob* imageKnob, float value) = 0;
    };

    explicit ZamKnob(Window& parent, const Image& image, Orientation orientation = Vertical) noexcept;
    explicit ZamKnob(Widget* widget, const Image& image, Orientation orientation = Vertical) noexcept;
    explicit ZamKnob(const ZamKnob& imageKnob);
    ZamKnob& operator=(const ZamKnob& imageKnob);
    ~ZamKnob() override;

    float getValue() const noexcept;

    void setDefault(float def) noexcept;
    void setRange(float min, float max) noexcept;
    void setStep(float step) noexcept;
    void setScrollStep(float step) noexcept;
    void setLabel(bool label) noexcept;
    void setValue(float value, bool sendCallback = false) noexcept;
    void setUsingLogScale(bool yesNo) noexcept;

    void setCallback(Callback* callback) noexcept;
    void setOrientation(Orientation orientation) noexcept;
    void setRotationAngle(int angle);

    void setImageLayerCount(uint count) noexcept;

protected:
     void onDisplay() override;
     bool onMouse(const MouseEvent&) override;
     bool onMotion(const MotionEvent&) override;
     bool onScroll(const ScrollEvent&) override;

private:
    Image fImage;
    bool  fLabel;
    float fMinimum;
    float fMaximum;
    float fScrollStep;
    float fStep;
    float fValue;
    float fValueDef;
    float fValueTmp;
    bool  fUsingDefault;
    bool  fUsingLog;
    Orientation fOrientation;

    int  fRotationAngle;
    bool fDragging;
    bool fRightClicked;
    int  fLastX;
    int  fLastY;

    Callback* fCallback;

    bool fIsImgVertical;
    uint fImgLayerWidth;
    uint fImgLayerHeight;
    uint fImgLayerCount;
    bool fIsReady;
    GLuint fTextureId;

    float _logscale(float value) const;
    float _invlogscale(float value) const;
    void labelDisplay(void);

    DISTRHO_LEAK_DETECTOR(ZamKnob)
};

// -----------------------------------------------------------------------

ZamKnob::ZamKnob(Window& parent, const Image& image, Orientation orientation) noexcept
    : Widget(parent),
      NanoVG(CREATE_ANTIALIAS),
      fImage(image),
      fLabel(false),
      fMinimum(0.0f),
      fMaximum(1.0f),
      fScrollStep(0.0f),
      fStep(0.0f),
      fValue(0.5f),
      fValueDef(fValue),
      fValueTmp(fValue),
      fUsingDefault(false),
      fUsingLog(false),
      fOrientation(orientation),
      fRotationAngle(0),
      fDragging(false),
      fRightClicked(false),
      fLastX(0),
      fLastY(0),
      fCallback(nullptr),
      fIsImgVertical(image.getHeight() > image.getWidth()),
      fImgLayerWidth(fIsImgVertical ? image.getWidth() : image.getHeight()),
      fImgLayerHeight(fImgLayerWidth),
      fImgLayerCount(fIsImgVertical ? image.getHeight()/fImgLayerHeight : image.getWidth()/fImgLayerWidth),
      fIsReady(false),
      fTextureId(0)
{
    glGenTextures(1, &fTextureId);
    setSize(fImgLayerWidth, fImgLayerHeight);
    NanoVG::loadSharedResources();
}

ZamKnob::ZamKnob(Widget* widget, const Image& image, Orientation orientation) noexcept
    : Widget(widget->getParentWindow()),
      NanoVG(CREATE_ANTIALIAS),
      fImage(image),
      fLabel(false),
      fMinimum(0.0f),
      fMaximum(1.0f),
      fScrollStep(0.0f),
      fStep(0.0f),
      fValue(0.5f),
      fValueDef(fValue),
      fValueTmp(fValue),
      fUsingDefault(false),
      fUsingLog(false),
      fOrientation(orientation),
      fRotationAngle(0),
      fDragging(false),
      fLastX(0),
      fLastY(0),
      fCallback(nullptr),
      fIsImgVertical(image.getHeight() > image.getWidth()),
      fImgLayerWidth(fIsImgVertical ? image.getWidth() : image.getHeight()),
      fImgLayerHeight(fImgLayerWidth),
      fImgLayerCount(fIsImgVertical ? image.getHeight()/fImgLayerHeight : image.getWidth()/fImgLayerWidth),
      fIsReady(false),
      fTextureId(0)
{
    glGenTextures(1, &fTextureId);
    setSize(fImgLayerWidth, fImgLayerHeight);
    NanoVG::loadSharedResources();
}

ZamKnob::ZamKnob(const ZamKnob& imageKnob)
    : Widget(imageKnob.getParentWindow()),
      NanoVG(CREATE_ANTIALIAS),
      fImage(imageKnob.fImage),
      fLabel(false),
      fMinimum(imageKnob.fMinimum),
      fMaximum(imageKnob.fMaximum),
      fScrollStep(imageKnob.fScrollStep),
      fStep(imageKnob.fStep),
      fValue(imageKnob.fValue),
      fValueDef(imageKnob.fValueDef),
      fValueTmp(fValue),
      fUsingDefault(imageKnob.fUsingDefault),
      fUsingLog(imageKnob.fUsingLog),
      fOrientation(imageKnob.fOrientation),
      fRotationAngle(imageKnob.fRotationAngle),
      fDragging(false),
      fRightClicked(false),
      fLastX(0),
      fLastY(0),
      fCallback(imageKnob.fCallback),
      fIsImgVertical(imageKnob.fIsImgVertical),
      fImgLayerWidth(imageKnob.fImgLayerWidth),
      fImgLayerHeight(imageKnob.fImgLayerHeight),
      fImgLayerCount(imageKnob.fImgLayerCount),
      fIsReady(false),
      fTextureId(0)
{
    glGenTextures(1, &fTextureId);
    setSize(fImgLayerWidth, fImgLayerHeight);
    NanoVG::loadSharedResources();
}

ZamKnob& ZamKnob::operator=(const ZamKnob& imageKnob)
{
    fImage    = imageKnob.fImage;
    fMinimum  = imageKnob.fMinimum;
    fMaximum  = imageKnob.fMaximum;
    fScrollStep = imageKnob.fScrollStep;
    fStep     = imageKnob.fStep;
    fValue    = imageKnob.fValue;
    fValueDef = imageKnob.fValueDef;
    fValueTmp = fValue;
    fUsingDefault  = imageKnob.fUsingDefault;
    fUsingLog      = imageKnob.fUsingLog;
    fOrientation   = imageKnob.fOrientation;
    fRotationAngle = imageKnob.fRotationAngle;
    fDragging      = false;
    fRightClicked  = false;
    fLastX    = 0;
    fLastY    = 0;
    fCallback = imageKnob.fCallback;
    fIsImgVertical  = imageKnob.fIsImgVertical;
    fImgLayerWidth  = imageKnob.fImgLayerWidth;
    fImgLayerHeight = imageKnob.fImgLayerHeight;
    fImgLayerCount  = imageKnob.fImgLayerCount;
    fIsReady  = false;

    if (fTextureId != 0)
    {
        glDeleteTextures(1, &fTextureId);
        fTextureId = 0;
    }

    glGenTextures(1, &fTextureId);
    setSize(fImgLayerWidth, fImgLayerHeight);

    return *this;
}

ZamKnob::~ZamKnob()
{
    if (fTextureId != 0)
    {
        glDeleteTextures(1, &fTextureId);
        fTextureId = 0;
    }
}

float ZamKnob::getValue() const noexcept
{
    return fValue;
}

// NOTE: value is assumed to be scaled if using log
void ZamKnob::setDefault(float value) noexcept
{
    fValueDef = value;
    fUsingDefault = true;
}

void ZamKnob::setRange(float min, float max) noexcept
{
    DISTRHO_SAFE_ASSERT_RETURN(max > min,);

    if (fValue < min)
    {
        fValue = min;
        repaint();

        if (fCallback != nullptr)
        {
            try {
                fCallback->imageKnobValueChanged(this, fValue);
            } DISTRHO_SAFE_EXCEPTION("ZamKnob::setRange < min");
        }
    }
    else if (fValue > max)
    {
        fValue = max;
        repaint();

        if (fCallback != nullptr)
        {
            try {
                fCallback->imageKnobValueChanged(this, fValue);
            } DISTRHO_SAFE_EXCEPTION("ZamKnob::setRange > max");
        }
    }

    fMinimum = min;
    fMaximum = max;
}

void ZamKnob::setScrollStep(float step) noexcept
{
    fScrollStep = step;
}

void ZamKnob::setLabel(bool label) noexcept
{
    fLabel = label;
}

void ZamKnob::setStep(float step) noexcept
{
    fStep = step;
}

// NOTE: value is assumed to be scaled if using log
void ZamKnob::setValue(float value, bool sendCallback) noexcept
{
    if (d_isEqual(fValue, value))
        return;

    fValue = value;

    if (d_isZero(fStep))
        fValueTmp = value;

    if (fRotationAngle == 0)
        fIsReady = false;

    repaint();

    if (sendCallback && fCallback != nullptr)
    {
        try {
            fCallback->imageKnobValueChanged(this, fValue);
        } DISTRHO_SAFE_EXCEPTION("ZamKnob::setValue");
    }
}

void ZamKnob::setUsingLogScale(bool yesNo) noexcept
{
    fUsingLog = yesNo;
}

void ZamKnob::setCallback(Callback* callback) noexcept
{
    fCallback = callback;
}

void ZamKnob::setOrientation(Orientation orientation) noexcept
{
    if (fOrientation == orientation)
        return;

    fOrientation = orientation;
}

void ZamKnob::setRotationAngle(int angle)
{
    if (fRotationAngle == angle)
        return;

    fRotationAngle = angle;
    fIsReady = false;
}

void ZamKnob::setImageLayerCount(uint count) noexcept
{
    DISTRHO_SAFE_ASSERT_RETURN(count > 1,);

    fImgLayerCount = count;

    if (fIsImgVertical)
        fImgLayerHeight = fImage.getHeight()/count;
    else
        fImgLayerWidth = fImage.getWidth()/count;

    setSize(fImgLayerWidth, fImgLayerHeight);
}

void ZamKnob::labelDisplay()
{
    char txt[16];
    float ww = (float)getParentWindow().getWidth();
    float wh = (float)getParentWindow().getHeight();
    float w = (float)fImage.getWidth();
    float h = (float)fImage.getHeight();

    NanoVG::beginFrame(ww, wh);
    if (fValue > 1000.f) {
        snprintf(txt, sizeof(txt), "%.0f", fValue);
    } else {
        snprintf(txt, sizeof(txt), "%.1f", fValue);
    }
    FontId fid = findFont(NANOVG_DEJAVU_SANS_TTF);
    fontFaceId(fid);
    fontSize(14.f);
    fillColor(255, 255, 255, 255);
    strokeColor(255, 255, 255, 255);
    textAlign(ALIGN_CENTER | ALIGN_MIDDLE);
    textLineHeight(14.f);
    text(w/2., h/2., txt, NULL);
    NanoVG::endFrame();
}

void ZamKnob::onDisplay()
{
    const float normValue = ((fUsingLog ? _invlogscale(fValue) : fValue) - fMinimum) / (fMaximum - fMinimum);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fTextureId);

    if (! fIsReady)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        static const float trans[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, trans);

        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        uint imageDataOffset = 0;

        if (fRotationAngle == 0)
        {
            DISTRHO_SAFE_ASSERT_RETURN(fImgLayerCount > 0,);
            DISTRHO_SAFE_ASSERT_RETURN(normValue >= 0.0f,);

            const uint& v1(fIsImgVertical ? fImgLayerWidth : fImgLayerHeight);
            const uint& v2(fIsImgVertical ? fImgLayerHeight : fImgLayerWidth);

            const uint layerDataSize   = v1 * v2 * ((fImage.getFormat() == GL_BGRA || fImage.getFormat() == GL_RGBA) ? 4 : 3);
            imageDataOffset = layerDataSize * uint(normValue * float(fImgLayerCount-1));
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     static_cast<GLsizei>(getWidth()), static_cast<GLsizei>(getHeight()), 0,
                     fImage.getFormat(), fImage.getType(), fImage.getRawData() + imageDataOffset);

        fIsReady = true;
    }

    const int w = static_cast<int>(getWidth());
    const int h = static_cast<int>(getHeight());

    if (fRotationAngle != 0)
    {
        glPushMatrix();

        const int w2 = w/2;
        const int h2 = h/2;

        glTranslatef(static_cast<float>(w2), static_cast<float>(h2), 0.0f);
        glRotatef(normValue*static_cast<float>(fRotationAngle), 0.0f, 0.0f, 1.0f);

        Rectangle<int>(-w2, -h2, w, h).draw();

        glPopMatrix();
    }
    else
    {
        Rectangle<int>(0, 0, w, h).draw();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    if (fLabel)
        labelDisplay();
}

bool ZamKnob::onMouse(const MouseEvent& ev)
{
    if (ev.button > 3 || ev.button < 1)
        return false;
    
    if (ev.button == 3)
        fRightClicked = true;
    else fRightClicked = false;

    if (ev.press)
    {
        if (! contains(ev.pos))
            return false;

        if ((ev.mod & kModifierShift) != 0 && fUsingDefault)
        {
            setValue(fValueDef, true);
            fValueTmp = fValue;
            return true;
        }

        if ((ev.button == 2) && fUsingDefault)
        {
            setValue(fValueDef, true);
            fValueTmp = fValue;
            return true;
        }

        fDragging = true;
        fLastX = ev.pos.getX();
        fLastY = ev.pos.getY();

        if (fCallback != nullptr)
            fCallback->imageKnobDragStarted(this);

        return true;
    }
    else if (fDragging)
    {
        if (fCallback != nullptr)
            fCallback->imageKnobDragFinished(this);

        fDragging = false;
        return true;
    }

    return false;
}

bool ZamKnob::onMotion(const MotionEvent& ev)
{
    if (! fDragging)
        return false;

    bool doVal = false;
    float d, value = 0.0f;

    if (fOrientation == ZamKnob::Horizontal)
    {
        if (const int movX = ev.pos.getX() - fLastX)
        {
            d     = (ev.mod & kModifierControl) ? 2000.0f : 200.0f;
            if (fRightClicked) d = 2000.0f;
            if (fRightClicked && (ev.mod & kModifierControl)) d = 20000.0f;
            value = (fUsingLog ? _invlogscale(fValueTmp) : fValueTmp) + (float(fMaximum - fMinimum) / d * float(movX));
            doVal = true;
        }
    }
    else if (fOrientation == ZamKnob::Vertical)
    {
        if (const int movY = fLastY - ev.pos.getY())
        {
            d     = (ev.mod & kModifierControl) ? 2000.0f : 200.0f;
            if (fRightClicked) d = 2000.0f;
            if (fRightClicked && (ev.mod & kModifierControl)) d = 20000.0f;
            value = (fUsingLog ? _invlogscale(fValueTmp) : fValueTmp) + (float(fMaximum - fMinimum) / d * float(movY));
            doVal = true;
        }
    }

    if (! doVal)
        return false;

    if (fUsingLog)
        value = _logscale(value);

    if (value < fMinimum)
    {
        fValueTmp = value = fMinimum;
    }
    else if (value > fMaximum)
    {
        fValueTmp = value = fMaximum;
    }
    else if (d_isNotZero(fStep))
    {
        fValueTmp = value;
        const float rest = std::fmod(value, fStep);
        value = value - rest + (rest > fStep/2.0f ? fStep : 0.0f);
    }

    setValue(value, true);

    fLastX = ev.pos.getX();
    fLastY = ev.pos.getY();

    return true;
}

bool ZamKnob::onScroll(const ScrollEvent& ev)
{
    if (! contains(ev.pos))
        return false;

    float value, d;

    if (d_isZero(fScrollStep)) {
        d = (ev.mod & kModifierControl) ? 2000.0f : 200.0f;
        value = (fUsingLog ? _invlogscale(fValueTmp) : fValueTmp) + (float(fMaximum - fMinimum) / d * 10.f * ev.delta.getY());
    } else {
        d = (ev.mod & kModifierControl) ? fScrollStep * 0.1f : fScrollStep;
        value = (fUsingLog ? _invlogscale(fValueTmp + d * ev.delta.getY())
                           : fValueTmp + d * ev.delta.getY());
    }

    if (fUsingLog)
        value = _logscale(value);

    if (value < fMinimum)
    {
        fValueTmp = value = fMinimum;
    }
    else if (value > fMaximum)
    {
        fValueTmp = value = fMaximum;
    }
    else if (d_isNotZero(fStep))
    {
        fValueTmp = value;
        const float rest = std::fmod(value, fStep);
        value = value - rest + (rest > fStep/2.0f ? fStep : 0.0f);
    }

    setValue(value, true);
    return true;
}

// -----------------------------------------------------------------------

float ZamKnob::_logscale(float value) const
{
    const float b = std::log(fMaximum/fMinimum)/(fMaximum-fMinimum);
    const float a = fMaximum/std::exp(fMaximum*b);
    if (value < fMinimum) value = fMinimum;
    if (value > fMaximum) value = fMaximum;
    return a * std::exp(b*value);
}

float ZamKnob::_invlogscale(float value) const
{
    const float b = std::log(fMaximum/fMinimum)/(fMaximum-fMinimum);
    const float a = fMaximum/std::exp(fMaximum*b);
    if (value < fMinimum) value = fMinimum;
    if (value > fMaximum) value = fMaximum;
    return std::log(value/a)/b;
}

// -----------------------------------------------------------------------

class ZamSwitch : public Widget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void imageSwitchClicked(ZamSwitch* imageButton, bool down) = 0;
    };

    explicit ZamSwitch(Window& parent, const Image& imageNormal, const Image& imageDown) noexcept;
    explicit ZamSwitch(Widget* widget, const Image& imageNormal, const Image& imageDown) noexcept;
    explicit ZamSwitch(const ZamSwitch& imageSwitch) noexcept;
    ZamSwitch& operator=(const ZamSwitch& imageSwitch) noexcept;

    bool isDown() const noexcept;
    void setDown(bool down) noexcept;

    void setCallback(Callback* callback) noexcept;

protected:
     void onDisplay() override;
     bool onMouse(const MouseEvent&) override;

private:
    Image fImageNormal;
    Image fImageDown;
    bool  fIsDown;

    Callback* fCallback;

    DISTRHO_LEAK_DETECTOR(ZamSwitch)
};

ZamSwitch::ZamSwitch(Window& parent, const Image& imageNormal, const Image& imageDown) noexcept
    : Widget(parent),
      fImageNormal(imageNormal),
      fImageDown(imageDown),
      fIsDown(false),
      fCallback(nullptr)
{
    DISTRHO_SAFE_ASSERT(fImageNormal.getSize() == fImageDown.getSize());

    setSize(fImageNormal.getSize());
}

ZamSwitch::ZamSwitch(Widget* widget, const Image& imageNormal, const Image& imageDown) noexcept
    : Widget(widget->getParentWindow()),
      fImageNormal(imageNormal),
      fImageDown(imageDown),
      fIsDown(false),
      fCallback(nullptr)
{
    DISTRHO_SAFE_ASSERT(fImageNormal.getSize() == fImageDown.getSize());

    setSize(fImageNormal.getSize());
}

ZamSwitch::ZamSwitch(const ZamSwitch& imageSwitch) noexcept
    : Widget(imageSwitch.getParentWindow()),
      fImageNormal(imageSwitch.fImageNormal),
      fImageDown(imageSwitch.fImageDown),
      fIsDown(imageSwitch.fIsDown),
      fCallback(imageSwitch.fCallback)
{
    DISTRHO_SAFE_ASSERT(fImageNormal.getSize() == fImageDown.getSize());

    setSize(fImageNormal.getSize());
}

ZamSwitch& ZamSwitch::operator=(const ZamSwitch& imageSwitch) noexcept
{
    fImageNormal = imageSwitch.fImageNormal;
    fImageDown   = imageSwitch.fImageDown;
    fIsDown      = imageSwitch.fIsDown;
    fCallback    = imageSwitch.fCallback;

    DISTRHO_SAFE_ASSERT(fImageNormal.getSize() == fImageDown.getSize());

    setSize(fImageNormal.getSize());

    return *this;
}

bool ZamSwitch::isDown() const noexcept
{
    return fIsDown;
}

void ZamSwitch::setDown(bool down) noexcept
{
    fIsDown = down;
}

void ZamSwitch::setCallback(Callback* callback) noexcept
{
    fCallback = callback;
}

void ZamSwitch::onDisplay()
{
    if (fIsDown)
        fImageDown.draw();
    else
        fImageNormal.draw();
}

bool ZamSwitch::onMouse(const MouseEvent& ev)
{
    if (ev.press && contains(ev.pos))
    {
        fIsDown = true;
        repaint();

        if (fCallback != nullptr)
            fCallback->imageSwitchClicked(this, true);

        return true;
    }

    return false;
}

END_NAMESPACE_DGL

#endif
