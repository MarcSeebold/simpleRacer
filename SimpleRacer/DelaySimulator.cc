#include "DelaySimulator.hh"
#include <QTimer>
#include "GameLogic.hh"

DelaySimulator::DelaySimulator(SharedGameLogic _client, SharedGameLogic _server)
  : mGameLogicClient(std::forward<SharedGameLogic>(_client)), mGameLogicServer(std::forward<SharedGameLogic>(_server))
{
}

DelaySimulator::~DelaySimulator()
{
   for (_& a : mDelayedActions)
      delete a;
   mDelayedActions.clear();
}

void DelaySimulator::update()
{
   // clean up delayed actions
   for (_ it = mDelayedActions.begin(); it != mDelayedActions.end();)
   {
      if (!(*it)->isActive())
      {
         _ obj = *it;
         it = mDelayedActions.erase(it);
         delete obj;
      }
      else
      {
         ++it;
      }
   }
}

void DelaySimulator::csSendInput(InputController::KeyStatus _keys)
{
   SR_ASSERT(0 && "TODO");
}

void DelaySimulator::scSendCar(PlayerID _player, QVector2D _position, QVector2D _velocity)
{
   SR_ASSERT(0 && "TODO");
}

void DelaySimulator::scSendCoins(std::vector<QVector2D> _coins)
{
   SR_ASSERT(0 && "TODO");
}

void DelaySimulator::scSendScore(PlayerID _player, int _score)
{
   SR_ASSERT(0 && "TODO");
}

void DelaySimulator::pushDelayedAction(std::function<void()> _function, DelayedActionType _type)
{
   _ delayed = new QTimer();
   mDelayedActions.push_back(delayed);
   delayed->setSingleShot(true);
   connect(delayed, &QTimer::timeout, [this, _function]()
           {
      _function();
   });
   float delay = (_type == DelayedActionType::CLIENT_TO_SERVER)? mDelayClientToServer : mDelayServerToClient;
   delayed->start(delay * 1000); // secondsToMilliseconds
}
