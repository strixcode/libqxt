#include "qxtfilelock.h"
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

#include "qxtfilelock_p.h"
#include <windows.h>
#include <io.h>

bool QxtFileLock::unlock()
{
    if (file() && file()->isOpen() && isActive())
    {
        HANDLE w32FileHandle;
        OVERLAPPED ov1;

        w32FileHandle = (HANDLE)_get_osfhandle(file()->handle());
        if (w32FileHandle == INVALID_HANDLE_VALUE)
            return false;

        memset(&ov1, 0, sizeof(ov1));
        ov1.Offset =  qxt_d().offset;

        if (UnlockFileEx(w32FileHandle, 0, qxt_d().length, 0, &ov1))
        {
            qxt_d().isLocked = false;
            return true;
        }
    }
    return false;
}

bool QxtFileLock::lock()
{
    if (file() && file()->isOpen() && !isActive())
    {
        HANDLE w32FileHandle;
        OVERLAPPED ov1;
        DWORD dwflags;

        w32FileHandle = (HANDLE)_get_osfhandle(file()->handle());
        if (w32FileHandle == INVALID_HANDLE_VALUE)
            return false;

        switch (qxt_d().mode)
        {
        case    ReadLock:
            dwflags = LOCKFILE_FAIL_IMMEDIATELY;
            break;

        case    ReadLockWait:
            dwflags = 0;
            break;

        case    WriteLock:
            dwflags = LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY;
            break;

        case    WriteLockWait:
            dwflags = LOCKFILE_EXCLUSIVE_LOCK;
            break;

        default:
            return (false);
        }

        memset(&ov1, 0, sizeof(ov1));
        ov1.Offset =  qxt_d().offset;

        if (LockFileEx(w32FileHandle, dwflags, 0,  qxt_d().length, 0, &ov1))
        {
            qxt_d().isLocked = true;
            return true;
        }
    }
    return false;
}
