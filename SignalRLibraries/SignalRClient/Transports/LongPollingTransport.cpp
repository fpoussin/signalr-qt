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

#include "LongPollingTransport.h"
#include "Helper/Helper.h"
#include <QsLog.h>

LongPollingTransport::LongPollingTransport(HttpClient* httpClient, Connection *con) :HttpBasedTransport(httpClient, con)
{

}


LongPollingTransport::~LongPollingTransport(void)
{
}

void LongPollingTransport::start(QString)
{
    connect(_httpClient, SIGNAL(getRequestCompleted(QString,SignalException*)), this, SLOT(onPollHttpResponse(QString,SignalException*)));

    _url = _connection->getUrl() + "/connect";
    _url += TransportHelper::getReceiveQueryString(_connection, _connection->onSending(), getTransportType());

    Q_EMIT transportStarted(0);

    _started = true;
    _httpClient->get(_url);
}

void LongPollingTransport::abort()
{
    _httpClient->abort();
}

void LongPollingTransport::stop()
{
    _httpClient->abort();
}

const QString &LongPollingTransport::getTransportType()
{
    static QString type  ="longPolling";
    return type;
}

void LongPollingTransport::onPollHttpResponse(const QString& httpResponse, SignalException *ex)
{ 
    bool timedOut = false, disconnected = false;
    SignalException *error = 0;
    if(ex)
       error = (SignalException*)ex;

    if(!error)
    {
        TransportHelper::processMessages(_connection, httpResponse, &timedOut, &disconnected);
    }
    else
    {
        if(_started)
        {
            Q_EMIT transportStarted(error);
        }
        else
        {
            if(error->getType() == SignalException::ConnectionRefusedError)
            {
                _connection->changeState(Connection::Reconnecting, Connection::Connected);
            }

            if(_connection->ensureReconnecting())
            {
                QLOG_DEBUG() << "LongPollingTranpsort: lost connection...try to reconnect";
                Helper::wait(2);
                //_transport->run();
            }
            else if(_connection->getAutoReconnect())
            {
                QLOG_DEBUG() << "LongPollingTranpsort: (autoconnect=true) lost connection...try to reconnect";
                Helper::wait(2);
                //pollInfo->transport->run();
            }
            else
            {
                _connection->onError(*error);
            }
        }

        delete error;
    }

    if(disconnected)
    {
        _connection->stop();
    }
    else
    {
        _httpClient->get(_url);
    }

}
