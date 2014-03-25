#include "ConnectionMessage.h"

namespace P3 { namespace SignalR { namespace Server {

ConnectionMessage::ConnectionMessage(QList<QString> signal, QVariant value, QList<QString> excludedSignals)
{
    _signals = signal;
    _value = value;
    _excludedSignals = excludedSignals;
}

ConnectionMessage::ConnectionMessage(QString signal, QVariant value, QList<QString> excludedSignal)
{
    _signal = signal;
    _value = value;
    _excludedSignals = excludedSignal;
}

}}}
