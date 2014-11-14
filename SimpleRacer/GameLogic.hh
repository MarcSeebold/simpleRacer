#pragma once
#include <memory>
#include <QObject>
#include "Common.hh"

namespace simpleRacer
{
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
SHARED(struct, GameState);
class GameLogic : public QObject
{
   Q_OBJECT

public:
   /// max. horizontal velocity in px/sec
   static const int sMaxVeloX = 100;
   /// min. horizontal velocity in px/sec
   static const int sMinVeloX = 10;
   /// max. vertical velocity in px/sec
   static const int sMaxVeloY = 20;
   /// min. vertical velocity in px/sec
   static const int sMinVeloY = 0;

   ///@{
   /// Size of the car in px (it is a rectangle)
   static const int sCarWidth = 50;
   static const int sCarLength = 200;
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

public slots:
   /// Simulate one time step
   void update(const float &_timestep = 1 / 60.f);

private:
   struct UserInput
   {
      float velocityDeltaX[2]; ///< Horizontal Delta
      float velocityDeltaY[2]; ///< Vertical Delta
   };
   SHARED(struct, UserInput);

private:
   UniqueGameState mGameState; ///< current game state
   UniqueUserInput mUserInput; ///< actions applied to next game state
   bool mRunning = false;      ///< Is the game currently running?

public: // Getter, Setter
   const GameState *getGameState() const { return mGameState.get(); }
   PROPERTY(Running);
};

struct GameState
{
   int playerCoins[2]; ///< 0 coins at beginning
   float positionX[2]; ///< Horizotal car positions (center of car)
   float positionY[2]; ///< Vertical car positions (center of car)
   float velocityX[2]; ///< Velocity of the cars (px / sec)
   float velocityY[2]; ///< Steering velocity of the cars (px / sec)
};

} // namespace simpleRacer
