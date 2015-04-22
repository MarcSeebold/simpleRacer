#pragma once
#include <QVector2D>
#include "Common.hh"
#include "InputController.hh"
#include <functional>
#include <QObject>

SHARED(class, GameLogic);

/**
 * @todo Server-Side Lag-Kompensation
 *       "Pakete" zwischen Server und Client peridisch senden (zB 20Hz)
 */
class DelaySimulator : public QObject
{
   Q_OBJECT

public:
   DelaySimulator(SharedGameLogic _client, SharedGameLogic _server);
   ~DelaySimulator();

public:
   void update();

   // Client -> Server
   void csSendInput(InputController::KeyStatus _keys);

   // Server -> Client
   void scSendCar(PlayerID _player, QVector2D _position, QVector2D _velocity);
   void scSendCoins(std::vector<QVector2D> _coins);
   void scSendScore(PlayerID _player, int _score);

private:
   enum class DelayedActionType : char
   {
      SERVER_TO_CLIENT,
      CLIENT_TO_SERVER
   };
   void pushDelayedAction(std::function<void()> _function, DelayedActionType _type);

private:
   float mDelayClientToServer = 0.5f; //< in seconds
   float mDelayServerToClient = 0.5f; //< in seconds
   std::vector<QTimer *> mDelayedActions;
   SharedGameLogic mGameLogicClient;
   SharedGameLogic mGameLogicServer;
};
