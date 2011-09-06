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

#include <qt_windows.h>

void QxtScreenPrivate::init_sys()
{
    DISPLAY_DEVICE displayDevice;
    ::ZeroMemory(&displayDevice, sizeof(displayDevice));
    displayDevice.cb = sizeof(displayDevice);

    if (::EnumDisplayDevices(NULL, screen, &displayDevice, 0))
    {
        DEVMODE devMode;
        ::ZeroMemory(&devMode, sizeof(devMode));
        devMode.dmSize = sizeof(devMode);

        // current resolution & rate
        if (!currReso.isValid() || currRate < 0 || currDepth < 0)
        {
            if (::EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode))
            {
                currReso = QSize(devMode.dmPelsWidth, devMode.dmPelsHeight);
                currRate = devMode.dmDisplayFrequency;
                currDepth = devMode.dmBitsPerPel;
            }
        }

        // available resolutions & rates
        if (availResos.isEmpty() || availRates.isEmpty() || availDepths.isEmpty())
        {
            availResos.clear();
            availRates.clear();
            availDepths.clear();

            ::ZeroMemory(&devMode, sizeof(devMode));
            devMode.dmSize = sizeof(devMode);

            DWORD i = 0;
            while (::EnumDisplaySettings(displayDevice.DeviceName, i++, &devMode))
            {
                const QSize reso(devMode.dmPelsWidth, devMode.dmPelsHeight);
                if (!availResos.contains(reso))
                    availResos += reso;
                if (!availRates.contains(reso, devMode.dmDisplayFrequency))
                    availRates.insertMulti(reso, devMode.dmDisplayFrequency);
                if (!availDepths.contains(reso, devMode.dmBitsPerPel))
                    availDepths.insertMulti(reso, devMode.dmBitsPerPel);

                ::ZeroMemory(&devMode, sizeof(devMode));
                devMode.dmSize = sizeof(devMode);
            }
        }
    }
}

bool QxtScreenPrivate::set(const QSize& reso, int rate, int depth)
{
    bool result = false;

    DISPLAY_DEVICE displayDevice;
    ::ZeroMemory(&displayDevice, sizeof(displayDevice));
    displayDevice.cb = sizeof(displayDevice);

    if (::EnumDisplayDevices(NULL, screen, &displayDevice, 0))
    {
        DEVMODE devMode;
        ::ZeroMemory(&devMode, sizeof(devMode));
        devMode.dmSize = sizeof(devMode);

        if (reso.isValid())
        {
            devMode.dmPelsWidth = reso.width();
            devMode.dmPelsHeight = reso.height();
            devMode.dmFields |= DM_PELSWIDTH | DM_PELSHEIGHT;
        }

        if (rate != -1)
        {
            devMode.dmDisplayFrequency = rate;
            devMode.dmFields |= DM_DISPLAYFREQUENCY;
        }

        if (depth != -1)
        {
            devMode.dmBitsPerPel = depth;
            devMode.dmFields |= DM_BITSPERPEL;
        }

        result = ::ChangeDisplaySettingsEx(displayDevice.DeviceName, &devMode, NULL, 0, NULL) == DISP_CHANGE_SUCCESSFUL;
    }

    return result;
}
