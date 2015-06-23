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
   OPEN_SURVEY_PREGAME = 2,
   OPEN_SURVEY_INGAME = 3,
   OPEN_SURVEY_POSTGAME = 4,
   LOAD_CONDITION_1 = 5,
   LOAD_CONDITION_2 = 6,
   LOAD_CONDITION_3 = 7,
   LOAD_CONDITION_4 = 8,
   LOAD_CONDITION_5 = 9,
   LOAD_CONDITION_6 = 10,
   LOAD_CONDITION_7 = 11,
   LOAD_CONDITION_8 = 12,
   LOAD_CONDITION_9 = 13,
   LOAD_CONDITION_10 = 14,
   NOP = 15,
   START_TESTPLAY = 16,
   STOP_TESTPLAY = 17,
   WAITING = 18,
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
