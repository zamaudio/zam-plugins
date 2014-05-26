/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2014 Filipe Coelho <falktx@falktx.com>
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

#ifndef DGL_IMAGE_BUTTON_HPP_INCLUDED
#define DGL_IMAGE_BUTTON_HPP_INCLUDED

#include "Image.hpp"
#include "Widget.hpp"

START_NAMESPACE_DGL

// -----------------------------------------------------------------------

class ImageButton : public Widget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void imageButtonClicked(ImageButton* imageButton, int button) = 0;
    };

    explicit ImageButton(Window& parent, const Image& image) noexcept;
    explicit ImageButton(Window& parent, const Image& imageNormal, const Image& imageHover, const Image& imageDown) noexcept;
    explicit ImageButton(Widget* widget, const Image& image) noexcept;
    explicit ImageButton(Widget* widget, const Image& imageNormal, const Image& imageHover, const Image& imageDown) noexcept;
    explicit ImageButton(const ImageButton& imageButton) noexcept;

    void setCallback(Callback* callback) noexcept;

protected:
     void onDisplay() override;
     bool onMouse(const MouseEvent&) override;
     bool onMotion(const MotionEvent&) override;

private:
    Image  fImageNormal;
    Image  fImageHover;
    Image  fImageDown;
    Image* fCurImage;
    int    fCurButton;

    Callback* fCallback;

    DISTRHO_LEAK_DETECTOR(ImageButton)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DGL

#endif // DGL_IMAGE_BUTTON_HPP_INCLUDED
