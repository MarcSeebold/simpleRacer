#include "StatisticsEngine.hh"

StatisticsEngine *StatisticsEngine::sInstance = nullptr;

StatisticsEngine *StatisticsEngine::the()
{
   if (!sInstance)
      sInstance = new StatisticsEngine();
   return sInstance;
}

void StatisticsEngine::tellNewGameRound()
{
   ++mCurrGameRound;
   SR_ASSERT(mGameStats.count(mCurrGameRound) == 0 && "GameStats already present");
   mGameStats[mCurrGameRound] = std::make_shared<GameStat>();
}

void StatisticsEngine::tellCollision(PhysicsObject::Type _typeA, PhysicsObject::Type _typeB, bool _latencyActive, bool _triggeredLatency, PlayerID _involvedPlayer)
{
   SR_ASSERT(mGameStats.count(mCurrGameRound) == 1 && "No GameStat object. Forgot to call tellNewGameRound?");
   _ collision = std::make_shared<GameStat::Collision>(_typeA, _typeB, _latencyActive, _triggeredLatency,
                                                       common::getCurrentTimestamp(), _involvedPlayer);
   mGameStats[mCurrGameRound]->collisions.push_back(std::move(collision));
}

void StatisticsEngine::tellEvent(StatisticsEngine::EventType _type)
{
   SR_ASSERT(mGameStats.count(mCurrGameRound) == 1 && "No GameStat object. Forgot to call tellNewGameRound?");
   switch (_type)
   {
   case EventType::P1COIN:
      mGameStats[mCurrGameRound]->p1Coins++;
      break;
   case EventType::P2COIN:
      mGameStats[mCurrGameRound]->p2Coins++;
      break;
   case EventType::P1MUD:
      mGameStats[mCurrGameRound]->p1Muds++;
      break;
   case EventType::P2MUD:
      mGameStats[mCurrGameRound]->p2Muds++;
      break;
   default:
      SR_ASSERT(0 && "unhandled case");
   }
}

StatisticsEngine::StatisticsEngine()
{
}


StatisticsEngine::GameStat::Collision::Collision(PhysicsObject::Type _typeA, PhysicsObject::Type _typeB, bool _latencyActive, bool _triggeredLatency, int64_t _timestamp, PlayerID _player)
  : objAType(_typeA), objBType(_typeB), latencyActive(_latencyActive), triggeredLatency(_triggeredLatency), timestamp(_timestamp), involvedPlayer(_player)
{
}
