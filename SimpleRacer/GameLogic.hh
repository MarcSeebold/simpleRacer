#pragma once
#include <memory>
#include <QObject>
#include <QVector2D>
#include "Common.hh"
#include "InputController.hh"

SHARED(class, b2World);
SHARED(class, Boundary);
SHARED(class, Car);
SHARED(class, Coin);
SHARED(class, Mud);
SHARED(class, ContactListener);

/**
 * @brief Game concept: 1 on 1 racing game.
 *        3 Lanes. Goal: Collect coins, avoid rocks.
 *        End of game after X Minutes.
 *        Top left corner is x=0, y=0. Bottom right corner is 100,100.
 *
 *        |x=0                       |x=100
 *        v                          v
 *        --------------------------- <- y=0
 *        Lane 0
 *        ---------------------------
 *        Lane 1
 *        ---------------------------
 *        Lane 2
 *        --------------------------- <- y=100
 *
 *        We use a heartbeat system. I.e., we have a fixed timestep for the game logic simulation.
 *        Every input event is processed at the end of a simulation step.
 */
class GameLogic : public QObject, public std::enable_shared_from_this<GameLogic>
{
   Q_OBJECT

public:
   struct OldData;
   static const float sConversionFactor;
   static const float sCarHeight;
   static const float sCarWidth;
   static const float sCoinSize;
   static const float sMudSize;
   static const float sGameWidth;
   static const float sGameHeight;

   enum class Type : char
   {
      SERVER,
      CLIENT
   };

public:
   /// c'tor
   GameLogic(Type _type);
   /// c'tor
   ~GameLogic();

   /// Resets the game state
   void reset();

   bool isServer() const { return mType == Type::SERVER; }
   bool isClient() const { return mType == Type::CLIENT; }

   /// Enable/Disable cheats for AI.
   /// This allows the AI to drive faster than the human player.
   void setAICheating(bool _val);

   /// accelerates the player by one px/sec
   void accelerate(PlayerID _id);
   /// decelerate the player by one px/sec
   void decelerate(PlayerID _id);

   /// move player's car up by one px/sec
   void steerUp(PlayerID _id);
   /// move player's car down by one px/sec
   void steerDown(PlayerID _id);

   /// Returns the current position of the car
   QVector2D getCarCenterPosition(PlayerID _id);

   /// Returns the current linearvelocity of the car
   QVector2D getCarLinearVelocity(PlayerID _id);

   /// Get positions of all coins
   std::vector<QVector2D> getCoins();

   /// Get positions of all mud puddles
   std::vector<QVector2D> getMuds();

   int getScore(PlayerID _id);

   // Coin callbacks
   void setCoinSpawnCallback(void (*_func)(QVector2D)) { mCoinSpawnCallback = _func; }
   void setCoinCollectedCallback(void (*_func)(QVector2D)) { mCoinCollectedCallback = _func; }

   // Mud callbacks
   void setMudSpawnCallback(void (*_func)(QVector2D)) { mMudSpawnCallback = _func; }
   void setMudCollectedCallback(void (*_func)(QVector2D)) { mMudCollectedCallback = _func; }

   void setKeyStatus(const InputController::KeyStatus &_status) { mKeyStatus = _status; }

public: // client only: setter for server data
   /// Sets the position of _player's car in the physics engine.
   void setCarPositionVelocity(PlayerID _player, const QVector2D &_pos, const QVector2D &_velo);

public slots:
   /// Simulate one time step
   void update(const float &_timestep);

private:
   /// Spawn new coin (client only)
   void clientSpawnCoin(const QVector2D &_pos);

   /// Spawn new mud puddle (client only)
   void clientSpawnMud(const QVector2D &_pos);

   /// Replace all coins by _coins
   void setCoins(const std::vector<QVector2D> &_coins);

   /// Replace all mud puddles by _muds
   void setMuds(const std::vector<QVector2D> &_muds);

   /// Spawns a coin
   void spawnCoin();

   /// Spawns a mud puddle
   void spawnMud();

   /// Called when a mud puddle has been collected
   void callbackCarMud(Car *_car, Mud *_mud);

   /// Called when a coin has been collected
   void callbackCarCoin(Car *_car, Coin *_coin);

   /// Called on a Car/Car collision
   void callbackCarCar(Car *_carA, Car *_carB);

   /// Called on a Car/Boundary collision
   void callbackCarBoundary(Car *_car, Boundary *_boundary);

   /// Called whenever a critical situation occurs
   /// @return True iff artificial latency is triggered by this call
   bool criticalSituationOccured();

private:
   struct AIInput
   {
      void reset();
      int deltaX[2]; ///< Horizontal Delta
      int deltaY[2]; ///< Vertical Delta
   };
   SHARED(struct, AIInput);

private:
   bool mAICheatingEnabled = false; ///< AI is allowed to cheat?
   Type mType;                      ///< Server or Client?
   UniqueAIInput mAIInput;          ///< actions applied to next game state
   Sharedb2World mPhysicsWorld;     ///< Box2D World
   Sharedb2World mPhysicsWorldOld;  ///< Box2D World. This is mPhysicsWorld, but x seconds in the past. x = latency.

   UniqueCar mCar1;    ///< Physics object for car 1
   UniqueCar mCar2;    ///< Physics object for car 2
   UniqueCar mCar1Old; ///< Physics object for car 1, but x seconds in the past. x = latency.
   UniqueCar mCar2Old; ///< Physics object for car 2, but x seconds in the past. x = latency.

   UniqueBoundary mStreetBoundaries[4]; ///< Physics objects for end of the street
   UniqueBoundary
       mStreetBoundariesOld[4]; ///< Physics objects for end of the street, but x seconds in the past. x = latency.

   UniqueContactListener mContactListener; ///< Box2D contact listener

   DelayedActions mDelayedLagDisabling;                 ///< Helper for delayed stuff
   DelayedActions mDelayedServerCarPosUpdate;           ///< Helper for synchronizing server lag compensation
   DelayedActions mDelayedSpawner;                      ///< Helper for spawning coins and mud puddles
   DelayedActions mDelayedAIInput;                      ///< Helper for delayed AI input
   std::vector<UniqueCoin> mCoins;                      ///< Coins in the world
   std::vector<UniqueMud> mMuds;                        ///< Mud puddles in the world
   std::vector<Coin *> mCoinsToRemove;                  ///< Coins that should be deleted
   std::vector<Mud *> mMudsToRemove;                    ///< Mud puddles that should be deleted
   int mScore[2];                                       ///< the score. 0 coins at beginning
   void (*mCoinSpawnCallback)(QVector2D) = nullptr;     ///< Callback for coin spawning
   void (*mCoinCollectedCallback)(QVector2D) = nullptr; ///< Callback for coin collecting
   void (*mMudSpawnCallback)(QVector2D) = nullptr;      ///< Callback for mud spawning
   void (*mMudCollectedCallback)(QVector2D) = nullptr;  ///< Callback for mud collecting
   InputController::KeyStatus mKeyStatus;               ///< Keys pressed (interesting for server only)

   friend class DelaySimulator;
};
