#pragma once
#include <memory>
#include <QObject>
#include <QVector2D>
#include "Common.hh"

SHARED(class, b2World);

namespace simpleRacer
{
SHARED(class, PhysicsObject);
SHARED(class, Car);
SHARED(class, Coin);
SHARED(class, PhysicsContactListener);
SHARED(class, ArtificialRacer);

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
   static const float sGameWidth;
   static const float sGameHeight;

public:
   /// c'tor
   GameLogic();
   /// c'tor
   ~GameLogic();

   /// Resets the game state
   void reset();

   /// accelerates the player by one px/sec
   void accelerate(PlayerID _id);
   /// decelerate the player by one px/sec
   void decelerate(PlayerID _id);

   /// move player's car up by one px/sec
   void steerUp(PlayerID _id);
   /// move player's car down by one px/sec
   void steerDown(PlayerID _id);

   /// Returns the current position of the car
   /// @param _timestamp Use this variable for getting positions in the past. -1 = latest
   QVector2D getCarCenterPosition(PlayerID _id, const qint64 &_timestamp = -1);

   /// Returns the current linearvelocity of the car
   /// @param _timestamp Use this variable for getting positions in the past. -1 = latest
   QVector2D getCarLinearVelocity(PlayerID _id, const qint64 &_timestamp = -1);

   /// Get positions of all coins
   /// @param _timestamp Use this variable for getting positions in the past. -1 = latest
   std::vector<QVector2D> getCoins(const qint64 &_timestamp = -1);

   int getScore(PlayerID _id);

public slots:
   /// Simulate one time step
   void update(const float &_timestep = 1 / 60.f);

private:
   /// Spawns a coin at a random position
   void spawnCoin();

   void coinCallback(Car *_car, Coin *_coin);

public: // TODO: made this public for Testing class. TODO: find out why it does not work with friending it
   /// Returns the car data that is closest to the given timestamp
   OldData getOldDataClosest(const std::vector<OldData>& _data, const qint64 &_timestamp);

private:
   struct UserInput
   {
      void reset();
      int deltaX[2]; ///< Horizontal Delta
      int deltaY[2]; ///< Vertical Delta
   };
   SHARED(struct, UserInput);

private:
   UniqueUserInput mUserInput;                    ///< actions applied to next game state
   Sharedb2World mPhysicsWorld;                   ///< Box2D World
   UniqueCar mCar1;                               ///< Physics object for car 1
   UniqueCar mCar2;                               ///< Physics object for car 2
   UniquePhysicsObject mStreetBoundaries[4];      ///< Physics objects for end of the street
   UniquePhysicsContactListener mContactListener; ///< Box2D contact listener
   UniqueArtificialRacer mAI;                     ///< A simple AI as opponent
   std::vector<UniqueCoin> mCoins;                ///< Coins in the world
   std::vector<Coin *> mCoinsToRemove;            ///< Coins that should be deleted
   int mPlayerCoins[2];                           ///< the score. 0 coins at beginning
   bool mRunning = false;                         ///< Is the game currently running?

public: // friend classing Testing does not work (TODO: find out why and fix it)
   // store old car positions incl. velocity vector
   struct OldData
   {
      int64_t timestamp;
      QVector2D position[2];
      QVector2D linVelo[2];
   };
   int mOldDataSoftLimit = 1000;   //< max size of mOldCarData (softlimit)
   std::vector<OldData> mOldCarData;  //< old car positions & velocity
   std::vector<OldData> mOldCoinData; //< old coin positions

public: // Getter, Setter
   PROPERTY(Running);
};

} // namespace simpleRacer
