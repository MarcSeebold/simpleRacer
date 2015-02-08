#include "NetworkEngine.hh"
#if 0
#include <QUdpSocket>
#include <cassert>
#include <QVector2D>
#include <iostream> // debug

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
   mIsServer = false;
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

bool NetworkEngine::sendHandshake()
{
   if (mIsServer)
      return false; // client only func

   assert(!mConn1.address.isNull() && "Address is null.");
   // say "hello"
   sendData(DataType::HANDSHAKE, QByteArray(), mConn1);
   mConn1.state = ConnState::HANDSHAKING;

   return true;
}

bool NetworkEngine::sendPlayerPosition(const PlayerID &_player, const QVector2D &_pos)
{
   // Data: PlayerID | posX | posY
   QByteArray data;
   const float posX = _pos.x();
   const float posY = _pos.y();
   data.append((char *)_player, sizeof(int));
   data.append((char *)&posX, sizeof(float));
   data.append((char *)&posY, sizeof(float));

   bool ok = sendData(DataType::PLAYER_POS, data, mConn1);
   if (mIsServer)
      ok &= sendData(DataType::PLAYER_POS, data, mConn2);
   return ok;
}

void NetworkEngine::onHandshake(const QHostAddress &_sender)
{
    assert(mConn1.address.isNull() || mConn2.address.isNull() && "already got 2 clients.");
    if (mConn1.address.isNull())
    {
        mConn1.address = _sender;
        mConn1.state = ConnState::CONNECTED;
    }
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
    // 0: type
    DataType type = (DataType)_data.at(0);
    // 1-8: id
    char idStr[8];
    for (int i=0; i<8; ++i)
        idStr[i] = _data.at(i+1);
    uint64_t id = *((uint64_t*)idStr);
    // 9+: data
    QByteArray data = _data.right(_data.size() - (8+1));

    std::cerr << "type: "<< (int)type << "\nID:" << id << "\data:" << QString(_data).toStdString() << std::endl;
}

bool NetworkEngine::sendData(const DataType &_type, const QByteArray &_data, const NetworkEngine::Connection &_receiver)
{
   QByteArray data;
   data.reserve(_data.size() + sizeof _type + sizeof(int));
   char *idStr = (char *)(_receiver.currentID);
   data.append(idStr, sizeof _receiver.currentID);
   data.append((char)_type);
   auto res = mUDPSocket->writeDatagram(data, _receiver.address, _receiver.port);
   return (res != -1);
}
#endif
