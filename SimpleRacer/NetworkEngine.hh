#pragma once
#include "Common.hh"
#include <QObject>
#include <QByteArray>

SHARED(class, QTcpServer);
SHARED(class, QNetworkSession);
class QTcpSocket;

enum class NetworkCommand : short
{
   INVALID = 0,
   START_GAME = 1,
   OPEN_SMALL_SURVEY = 2,
   SIZE
};

class NetworkEngine : public QObject
{
   Q_OBJECT

public:
   NetworkEngine();
   ~NetworkEngine();

   /// Start a server. IPv4 only. Port given by settings.
   bool listen();
   /// Send data to the connected client. Returns false on error or if no client is connected.
   bool send(const QByteArray &_data);

private slots:
   void newConnection();
   void onData();
   void onDisconnect();

private:
   UniqueQTcpServer mTcpServer;
   UniqueQNetworkSession mNetworkSession;
   QTcpSocket *mClient = nullptr; ///< We do not own this ptr.
};
