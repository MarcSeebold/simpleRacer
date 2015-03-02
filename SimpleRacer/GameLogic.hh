#pragma once
#include <memory>
#include <QObject>
#include <QVector2D>
#include "Common.hh"
#include <QTimer>

#define SR_GAMESTEPTIME 10 // every 10 ms = 1/100s * 1000s/ms = 100Hz

SHARED(class, b2World);
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

   void start();

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
   QVector2D getCarCenterPosition(PlayerID _id);

   /// Returns the current linearvelocity of the car
   QVector2D getCarLinearVelocity(PlayerID _id);

   /// Get positions of all coins
   std::vector<QVector2D> getCoins();

   int getScore(PlayerID _id);

private slots:
   /// Simulate one time step
   void update();

private:
   /// Spawns a coin at a random position
   void spawnCoin();

   void coinCallback(Car *_car, Coin *_coin);

private:
   struct UserInput
   {
      void reset();
      int deltaX[2]; ///< Horizontal Delta
      int deltaY[2]; ///< Vertical Delta
   };
   SHARED(struct, UserInput);

private:
   QTimer mTimer;
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

public: // Getter, Setter
   PROPERTY(Running);
};
