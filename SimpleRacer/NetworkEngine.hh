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

   START_GAME,

   SET_SURVEY_LANG_ENG,
   SET_SURVEY_LANG_GER,

   OPEN_SURVEY_PREGAME,
   OPEN_SURVEY_INGAME,
   OPEN_SURVEY_POSTGAME,

   LOAD_CONDITION_1,
   LOAD_CONDITION_2,
   LOAD_CONDITION_3,
   LOAD_CONDITION_4,
   LOAD_CONDITION_5,
   LOAD_CONDITION_6,
   LOAD_CONDITION_7,
   LOAD_CONDITION_8,
   LOAD_CONDITION_9,
   LOAD_CONDITION_10,

   START_TESTPLAY,
   STOP_TESTPLAY,

   GET_STATE,

   STATE_WAITING,
   STATE_PLAYING,
   STATE_SURVEY,

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
