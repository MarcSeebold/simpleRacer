#pragma once
#include <memory>
#include <QObject>
#include "Common.hh"

SHARED(class, b2World);

namespace simpleRacer
{
SHARED(class, PhysicsObject);

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
class GameLogic : public QObject
{
   Q_OBJECT

public:
   /// max. horizontal velocity in myUnit/sec
   static const int sMaxVeloX = 10;
   /// min. horizontal velocity in myUnit/sec
   static const int sMinVeloX = -10;
   /// max. vertical velocity in myUnit/sec
   static const int sMaxVeloY = 5;
   /// min. vertical velocity in myUnit/sec
   static const int sMinVeloY = -5;

   ///@{
   /// Size of the car in myUnit (it is a rectangle)
   static const int sCarWidth = 12;
   static const int sCarLength = 10;
   ///@}

   /// Unique player id
   enum class PlayerID : int
   {
      P1 = 0,
      P2 = 1
   };

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
   QVector2D getCarCenterPosition(PlayerID _id);

public slots:
   /// Simulate one time step
   void update(const float &_timestep = 1 / 60.f);

private:
   struct UserInput
   {
      void reset();
      int deltaX[2]; ///< Horizontal Delta
      int deltaY[2]; ///< Vertical Delta
   };
   SHARED(struct, UserInput);

private:
   UniqueUserInput mUserInput;               ///< actions applied to next game state
   Sharedb2World mPhysicsWorld;              ///< Box2D World
   UniquePhysicsObject mCar1;                ///< Physics object for car 1
   UniquePhysicsObject mCar2;                ///< Physics object for car 2
   UniquePhysicsObject mStreetBoundaries[4]; ///< Physics objects for end of the street
   int mPlayerCoins[2];                      ///< 0 coins at beginning

   bool mRunning = false; ///< Is the game currently running?

public: // Getter, Setter
   PROPERTY(Running);
};

} // namespace simpleRacer
