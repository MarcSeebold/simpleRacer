#pragma once
#include "PhysicsObject.hh"
#include "Common.hh"

/**
 * @brief The StatisticsEngine class is a singleton for collecting game statistics
 */
class StatisticsEngine
{
public:
   using gameRound = int;

   enum class EventType : char
   {
      P1MUD,
      P2MUD,
      P1COIN,
      P2COIN
   };

private:
   struct GameStat
   {
      // player specific
      int p1Muds = 0;
      int p2Muds = 0;
      int p1Coins = 0;
      int p2Coins = 0;
      // critical situations
      struct Collision
      {
         Collision(PhysicsObject::Type _typeA, PhysicsObject::Type _typeB, bool _latencyActive, bool _triggeredLatency, int64_t _timestamp, PlayerID _player);
         PhysicsObject::Type objAType = PhysicsObject::Type::OTHER;
         PhysicsObject::Type objBType = PhysicsObject::Type::OTHER;
         bool latencyActive = false;
         bool triggeredLatency = false;
         int64_t timestamp;
         PlayerID involvedPlayer = PlayerID::P1;
      };
      SHARED(struct, Collision);
      std::vector<SharedCollision> collisions;
   };
   SHARED(struct, GameStat);

public:
   /// Singleton "the"
   static StatisticsEngine *the();

   /// A new game round has begun
   void tellNewGameRound();
   /// A collision occured
   void tellCollision(PhysicsObject::Type _typeA, PhysicsObject::Type _typeB, bool _latencyActive, bool _triggeredLatency, PlayerID _involvedPlayer = PlayerID::P1);
   /// Something happend
   void tellEvent(EventType _type);

private:
   StatisticsEngine();

private:
   static StatisticsEngine *sInstance;
   std::map<gameRound, SharedGameStat> mGameStats;
   gameRound mCurrGameRound = 0;
};
