#ifndef SOCKETINIT_H
#define SOCKETINIT_H
#include <QTcpSocket>
#include <QHostAddress>

#include <QObject>

class SocketInit : public QObject
{
    Q_OBJECT
public:
    SocketInit();
};

#endif // SOCKETINIT_H
