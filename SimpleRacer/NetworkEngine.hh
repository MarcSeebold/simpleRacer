#pragma once
#if 0
#include "Common.hh"
#include <QObject>
#include <QHostAddress>

#define SR_NETWORK_DEFAULT_PORT 7755

SHARED(class, QUdpSocket);

/**
 * Packets: | ID | Type | Data |
 */

namespace simpleRacer
{
class NetworkEngine : public QObject
{
   Q_OBJECT
private:
   enum class ConnState : char
   {
      INVALID = 0,
      DISCONNECTED,
      HANDSHAKING,
      CONNECTED
   };
   struct Connection
   {
      Connection()
      {
         currentID = 0;
         lastReceivedID = 0;
         port = SR_NETWORK_DEFAULT_PORT;
         state = ConnState::INVALID;
      }
      QHostAddress address;
      uint64_t currentID;
      uint64_t lastReceivedID;
      int port;
      ConnState state;
   };
   enum class DataType : char
   {
      INVALID = 0,

      HANDSHAKE,

      PLAYER_POS,
      PLAYER_KEY,

      SPAWN_COIN,
      REMOVE_COIN
   };

public:
   NetworkEngine();
   ~NetworkEngine();

   bool startServer(int _port = SR_NETWORK_DEFAULT_PORT);
   bool connectTo(const QHostAddress &_target, int _port = SR_NETWORK_DEFAULT_PORT);
   bool stop();

public: // sending
   // client only
   bool sendHandshake();

   // server only

   // both
   bool sendPlayerPosition(const PlayerID &_player, const QVector2D &_pos);

private: // receiving
   // client only

   // server only
   void onHandshake(const QHostAddress &_sender);

   // both

private slots:
   void readData();

private:
   bool init(int _port = SR_NETWORK_DEFAULT_PORT);
   void processData(const QHostAddress &_sender, const QByteArray &_data);
   /// Automatically increases the ID
   bool sendData(const DataType &_type, const QByteArray &_data, const Connection &_receiver);

private:
   uint64_t mPacketID;
   bool mIsServer = false;
   UniqueQUdpSocket mUDPSocket;
   Connection mConn1; ///< If Server: Player 1, if client: Server
   Connection mConn2; ///< Only as server: Player 2
};
} // namespace
#endif
