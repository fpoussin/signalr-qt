/*
 *  Copyright (c) 2013, p3root - Patrik Pfaffenbauer (patrik.pfaffenbauer@p3.co.at)
 *  All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. All advertising materials mentioning features or use of this software
 *      must display the following acknowledgement:
 *      This product includes software developed by the p3root - Patrik Pfaffenbauer (patrik.pfaffenbauer@p3.co.at).
 *   4. Neither the name of the p3root - Patrik Pfaffenbauer nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY p3root - Patrik Pfaffenbauer ''AS IS'' AND ANY
 *   EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL p3root - Patrik Pfaffenbauer BE LIABLE FOR ANY
 *   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "KeepAliveData.h"

const static double _keepAliveWarnAt = 2.0 / 3.0;

KeepAliveData::KeepAliveData(double timeout)
{
    _timeout = timeout;
    _timeoutWarning = timeout * _keepAliveWarnAt;
    _checkInterval = (timeout - _timeoutWarning) / 3;
}

KeepAliveData::KeepAliveData(QDateTime lastKeepAlive, double timeout, double timeoutWarning, double checkInterval)
{
    _lastKeepAlive = lastKeepAlive;
    _timeout = timeout;
    _timeoutWarning = timeoutWarning;
    _checkInterval = checkInterval;
}

void KeepAliveData::setLastKeepAlive(QDateTime dt)
{
    _lastKeepAlive = dt;
}

const QDateTime &KeepAliveData::getLastKeepAlive()
{
    return _lastKeepAlive;
}

const double &KeepAliveData::getTimeout()
{
    return _timeout;
}

const double &KeepAliveData::getTimeoutWarning()
{
    return _timeoutWarning;
}


const double &KeepAliveData::getCheckInterval()
{
    return _checkInterval;
}