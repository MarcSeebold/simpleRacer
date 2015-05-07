#pragma once
#include <QVector2D>
#include "Common.hh"
#include "InputController.hh"
#include <functional>

SHARED(class, GameLogic);

/**
 * @todo Server-Side Lag-Kompensation
 *       "Pakete" zwischen Server und Client peridisch senden (zB 20Hz)
 */
class DelaySimulator
{
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
   SharedGameLogic mGameLogicClient;
   SharedGameLogic mGameLogicServer;
   DelayedActions mDelayManager;
};
