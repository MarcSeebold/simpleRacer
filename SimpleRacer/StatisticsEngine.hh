#pragma once
#include "PhysicsObject.hh"
#include "Common.hh"

class QJsonObject;

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
      /// Writes this GameStat object to JSON
      void write(QJsonObject &_json);

      // player specific
      int p1Muds = 0;
      int p2Muds = 0;
      int p1Coins = 0;
      int p2Coins = 0;

      // QoE questionnaire results
      QString surveyResults;

      /// Condition loaded for this game
      /// -1 = No condition loaded / testplay
      int condition = -1;

      /// Game Settings for this round
      QJsonObject *settings=nullptr;

      // critical situations
      struct Collision
      {
         Collision(PhysicsObject::Type _typeA, PhysicsObject::Type _typeB, bool _latencyActive, bool _triggeredLatency, int64_t _timestamp, PlayerID _player);
         /// Writes this Collision object to JSON
         void write(QJsonObject &_json);

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
   void tellCollision(PhysicsObject::Type _typeA,
                      PhysicsObject::Type _typeB,
                      bool _latencyActive,
                      bool _triggeredLatency,
                      PlayerID _involvedPlayer = PlayerID::P1);
   /// Something happend
   void tellEvent(EventType _type);
   /// Tell survey results. Will overwrite existing data!
   void tellInGameSurvey(QString _data);
   /// Tell the results of the core survey
   void tellCoreSurvey(QString _data);
   /// Tell the results of the post-game survey
   void tellPostGameSurvey(QString _data);
   // Save generated stats to disk
   void saveToFile();

private:
   /// Singleton: private c'tor
   StatisticsEngine();
   /// Singleton: No copy c'tor
   StatisticsEngine(StatisticsEngine const&) = delete;
   /// Singleton: No copy assignment
   StatisticsEngine& operator=(StatisticsEngine const&) = delete;

private:
   static StatisticsEngine *sInstance;
   std::map<gameRound, SharedGameStat> mGameStats;
   gameRound mCurrGameRound = 0;
   QString mPreGameSurveyResults;
   QString mPostGameSurveyResults;
};
