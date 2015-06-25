#pragma once
#include <QVector2D>
#include "Common.hh"
#include "InputController.hh"
#include <functional>

SHARED(class, GameLogic);

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
   void scSendMuds(std::vector<QVector2D> _muds);
   void scSendScore(PlayerID _player, int _score);
   void scSpawnCoin(const QVector2D &_pos);
   void scSpawnMud(const QVector2D &_pos);

private:
   SharedGameLogic mGameLogicClient;
   SharedGameLogic mGameLogicServer;
   DelayedActions mDelayManager;
};
