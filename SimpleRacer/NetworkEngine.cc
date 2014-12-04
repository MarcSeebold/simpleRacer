#include "NetworkEngine.hh"

#include <QUdpSocket>
#include <cassert>

using namespace simpleRacer;

NetworkEngine::NetworkEngine()
{
}

NetworkEngine::~NetworkEngine()
{
}

bool NetworkEngine::startServer(int _port)
{
   bool ok = init(_port);
   if (!ok)
       return false;
   mIsServer = true;
   return true;
}

bool NetworkEngine::connectTo(const QHostAddress &_target, int _port)
{
    bool ok = init(_port);
    if (!ok)
        return false;
   init(_port);
   mIsServer= false;
   return true;
}

bool NetworkEngine::init(int _port)
{
   static bool inited = false;
   if (inited)
      return true;

   inited = true;

   mUDPSocket = UniqueQUdpSocket(new QUdpSocket(this));
   bool ok = mUDPSocket->bind(QHostAddress::AnyIPv4, _port);
   connect(mUDPSocket.get(), SIGNAL(readyRead()), this, SLOT(readData()));
   return ok;
}

bool NetworkEngine::stop()
{
   mUDPSocket->close();
   return true;
}

void NetworkEngine::readData()
{
   while (mUDPSocket->hasPendingDatagrams())
   {
      QByteArray datagram;
      datagram.resize(mUDPSocket->pendingDatagramSize());
      QHostAddress sender;
      quint16 senderPort;

      mUDPSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

      processData(sender, datagram);
   }
}

void NetworkEngine::processData(const QHostAddress &_sender, const QByteArray &_data)
{
}
