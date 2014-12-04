#pragma once
#include "Common.hh"
#include <QObject>
#include <QHostAddress>

SHARED(class, QUdpSocket);

namespace simpleRacer
{
class NetworkEngine : public QObject
{
   Q_OBJECT

public:
   NetworkEngine();
   ~NetworkEngine();

   bool startServer(int _port = 7755);
   bool connectTo(const QHostAddress &_target, int _port = 7755);
   bool stop();

private slots:
    void readData();

private:
    bool init(int _port = 7755);
    void processData(const QHostAddress &_sender, const QByteArray &_data);

private:
   bool mIsServer = false;
   UniqueQUdpSocket mUDPSocket;

private:
   struct SRConnection
   {

   };
};
} // namespace
