#include "NetworkEngine.hh"
#include "Settings.hh"
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkSession>
#include <QDebug>
#include "SimpleRacer.hh"

NetworkEngine::NetworkEngine()
{
}

NetworkEngine::~NetworkEngine()
{
   mTcpServer = nullptr;
   mNetworkSession = nullptr;
   mClient = nullptr;
}

bool NetworkEngine::listen()
{
   mTcpServer = UniqueQTcpServer(new QTcpServer(this));
   _ port = Settings::the()->getNetworkPort();
   // IPv4 only, port specified in settings
   if (!mTcpServer->listen(QHostAddress::AnyIPv4, port))
      return false;
   connect(mTcpServer.get(), &QTcpServer::newConnection, this, &NetworkEngine::newConnection);
   return true;
}

bool NetworkEngine::send(const QByteArray &_data)
{
   return (mClient->write(_data) != -1);
}

void NetworkEngine::newConnection()
{
   mClient = mTcpServer->nextPendingConnection();
   connect(mClient, &QTcpSocket::disconnected, this, &NetworkEngine::onDisconnect);
   connect(mClient, &QTcpSocket::readyRead, this, &NetworkEngine::onData);
}

void NetworkEngine::onData()
{
   while(mClient->bytesAvailable())
   {
      _ data = mClient->readAll();
      bool ok = true;
      _ cmd = data.toShort(&ok);
      if (!ok)
      {
         qDebug() << "Unknown command: " << data;
         send("NACK");
         return;
      }
      switch ((NetworkCommand)cmd) {
      case NetworkCommand::START_GAME:
         SimpleRacer::the()->startGame();
         send("ACK");
         break;
      default:
         send("NACK");
         break;
      }
   }
}

void NetworkEngine::onDisconnect()
{
   // asynch delete by Qt
   mClient->deleteLater();
   mClient = nullptr;
}
