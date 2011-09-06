#include "qxtscreen_p.h"
/****************************************************************************
** Copyright (c) 2006 - 2011, the LibQxt project.
** See the Qxt AUTHORS file for a list of authors and copyright holders.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the LibQxt project nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** <http://libqxt.org>  <foundation@libqxt.org>
*****************************************************************************/


#ifdef HAVE_XRANDR
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#endif // HAVE_XRANDR

void QxtScreenPrivate::init_sys()
{
#ifdef HAVE_XRANDR
    Display* display = XOpenDisplay(NULL);
    Window root = RootWindow(display, screen);
    XRRScreenConfiguration* config = XRRGetScreenInfo(display, root);

    // available resolutions & rates
    if (availResos.isEmpty() || availRates.isEmpty())
    {
        availResos.clear();
        availRates.clear();

        int sizeCount = 0;
        XRRScreenSize* sizes = XRRSizes(display, 0, &sizeCount);
        for (int i = 0; i < sizeCount; ++i)
        {
            QSize reso(sizes[i].width, sizes[i].height);
            if (!availResos.contains(reso))
                availResos += reso;

            int rateCount = 0;
            short* rates = XRRConfigRates(config, i, &rateCount);
            for (int j = 0; j < rateCount; ++j)
                availRates.insertMulti(reso, rates[j]);
        }
    }

    // current resolution & rate
    if (!currReso.isValid() || currRate < 0)
    {
        Rotation rotation;
        SizeID sizeId = XRRConfigCurrentConfiguration(config, &rotation);
        currReso = availResos.at(sizeId);
        currRate = XRRConfigCurrentRate(config);
    }

    XRRFreeScreenConfigInfo(config);
    XCloseDisplay(display);
#endif // HAVE_XRANDR
}

bool QxtScreenPrivate::set(const QSize& reso, int rate, int depth)
{
    bool result = false;
    Q_UNUSED(reso);
    Q_UNUSED(rate);
    Q_UNUSED(depth);
#ifdef HAVE_XRANDR
    Display* display = XOpenDisplay(NULL);
    Window root = RootWindow(display, screen);
    XRRScreenConfiguration* config = XRRGetScreenInfo(display, root);

    int sizeIndex = 0;
    if (reso.isValid())
        sizeIndex = availResos.indexOf(reso);
    else
        sizeIndex = availResos.indexOf(currReso);
    Q_ASSERT(sizeIndex != -1);

    if (rate == -1)
        result = XRRSetScreenConfig(display, config, root, sizeIndex, RR_Rotate_0, CurrentTime);
    else
        result = XRRSetScreenConfigAndRate(display, config, root, sizeIndex, RR_Rotate_0, rate, CurrentTime);

    XRRFreeScreenConfigInfo(config);
    XCloseDisplay(display);
#endif // HAVE_XRANDR
    return result;
}
