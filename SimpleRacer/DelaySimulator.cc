#include "DelaySimulator.hh"
#include <QTimer>
#include "GameLogic.hh"
#include "Settings.hh"

DelaySimulator::DelaySimulator(SharedGameLogic _client, SharedGameLogic _server)
  : mGameLogicClient(std::forward<SharedGameLogic>(_client)), mGameLogicServer(std::forward<SharedGameLogic>(_server))
{
}

DelaySimulator::~DelaySimulator()
{
}

void DelaySimulator::update()
{
   mDelayManager.update();
}

void DelaySimulator::csSendInput(InputController::KeyStatus _keys)
{
   mDelayManager.pushDelayedAction(
       [this, _keys]()
       {
          mGameLogicServer->mKeyStatus = _keys;
       },
       DelayedActions::DelayedActionType::CLIENT_TO_SERVER);
}

void DelaySimulator::scSendCar(PlayerID _player, QVector2D _position, QVector2D _velocity)
{
   mDelayManager.pushDelayedAction(
       [this, _player, _position, _velocity]()
       {
          mGameLogicClient->setCarPositionVelocity(_player, _position, _velocity);
       },
       DelayedActions::DelayedActionType::SERVER_TO_CLIENT);
}

void DelaySimulator::scSendCoins(std::vector<QVector2D> _coins)
{
   mDelayManager.pushDelayedAction(
       [this, _coins]()
       {
          mGameLogicClient->setCoins(_coins);
       },
   DelayedActions::DelayedActionType::SERVER_TO_CLIENT);
}

void DelaySimulator::scSendMuds(std::vector<QVector2D> _muds)
{
   mDelayManager.pushDelayedAction(
       [this, _muds]()
       {
          mGameLogicClient->setMuds(_muds);
       },
   DelayedActions::DelayedActionType::SERVER_TO_CLIENT);
}

void DelaySimulator::scSendScore(PlayerID _player, int _score)
{
   mDelayManager.pushDelayedAction(
       [this, _player, _score]()
       {
          mGameLogicClient->mScore[int(_player)] = _score;
       },
   DelayedActions::DelayedActionType::SERVER_TO_CLIENT);
}

void DelaySimulator::scSpawnCoin(const QVector2D &_pos)
{
   mDelayManager.pushDelayedAction(
       [this, _pos]()
       {
          mGameLogicClient->clientSpawnCoin(_pos);
       },
   DelayedActions::DelayedActionType::SERVER_TO_CLIENT);
}

void DelaySimulator::scSpawnMud(const QVector2D &_pos)
{
   mDelayManager.pushDelayedAction(
       [this, _pos]()
       {
          mGameLogicClient->clientSpawnMud(_pos);
       },
   DelayedActions::DelayedActionType::SERVER_TO_CLIENT);
}
