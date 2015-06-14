#include "StatisticsEngine.hh"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "Settings.hh"

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
   const _ & stat = mGameStats[mCurrGameRound] = std::make_shared<GameStat>();
   // write current game settings to stat obj
   stat->settings = new QJsonObject;
   Settings::the()->write(*(stat->settings));
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

void StatisticsEngine::saveToFile()
{
   QDir dir("./stats/");
   if (!dir.exists())
      dir.mkpath("."); // ensure path exists
   QFile h("./stats/" + QString::number(common::getCurrentTimestamp()));
   _ ok = h.open(QIODevice::ReadWrite | QIODevice::Text);
   SR_ASSERT(ok && "failed to open file");
   QJsonArray jStats;
   // use json for saving stats
   for (_ const &s : mGameStats)
   {
      QJsonObject jStat;
      jStat["round"] = s.first;
      s.second->write(jStat);
      jStats.append(jStat);
   }
   QJsonDocument jDoc(jStats);
   h.write(jDoc.toJson());
   h.close();
}

void StatisticsEngine::GameStat::Collision::write(QJsonObject &_json)
{
   // helper function for enum stringification
   _ colTypeToString = [](PhysicsObject::Type _type) -> QString
   {
      switch (_type)
      {
      case PhysicsObject::Type::BOUNDARY:
         return "BOUNDARY";
         break;
      case PhysicsObject::Type::CAR:
         return "CAR";
         break;
      case PhysicsObject::Type::COIN:
         return "COIN";
         break;
      case PhysicsObject::Type::MUD:
         return "MUD";
         break;
      case PhysicsObject::Type::OTHER:
         return "OTHER";
         break;
      default:
         SR_ASSERT(0 && "unhandled case");
         return "INVALID";
      }
   };
   // serialize
   _json["objA"] = colTypeToString(objAType);
   _json["objB"] = colTypeToString(objBType);
   _json["latActive"] = latencyActive;
   _json["latTriggered"] = triggeredLatency;
   _json["timestamp"] = QString::number(timestamp);
   _json["player"] = (int)involvedPlayer;
}


void StatisticsEngine::GameStat::write(QJsonObject &_json)
{
   if (!settings)
   {
      SR_ASSERT(0 && "Settings not set!");
      return;
   }
   _json["Settings"] = *settings;
   _json["p1Muds"] = p1Muds;
   _json["p2Muds"] = p2Muds;
   _json["p1Coins"] = p1Coins;
   _json["p2Coins"] = p2Coins;
   QJsonArray jCollisions;
   for (_ const &c : collisions)
   {
      QJsonObject jObj;
      c->write(jObj);
      jCollisions.append(jObj);
   }
   _json["collision"] = jCollisions;
}

StatisticsEngine::StatisticsEngine()
{
}

StatisticsEngine::GameStat::Collision::Collision(
    PhysicsObject::Type _typeA, PhysicsObject::Type _typeB, bool _latencyActive, bool _triggeredLatency, int64_t _timestamp, PlayerID _player)
  : objAType(_typeA),
    objBType(_typeB),
    latencyActive(_latencyActive),
    triggeredLatency(_triggeredLatency),
    timestamp(_timestamp),
    involvedPlayer(_player)
{
}
