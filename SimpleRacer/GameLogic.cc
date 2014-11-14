#include "GameLogic.hh"
#include <algorithm>
#include <QTimer>

using namespace simpleRacer;

GameLogic::GameLogic()
{
   reset();
}

GameLogic::~GameLogic()
{
}

void GameLogic::reset()
{
   mGameState = UniqueGameState(new GameState);
   mUserInput = UniqueUserInput(new UserInput);

   for (int i : {0, 1})
   {
      mUserInput->velocityDeltaX[i] = 0;
      mUserInput->velocityDeltaY[i] = 0;
      mGameState->playerCoins[i] = 0;
      mGameState->positionX[i] = 0;
      mGameState->positionY[i] = 0;
      mGameState->velocityX[i] = 10;
      mGameState->velocityY[i] = 0;
   }
   mGameState->positionY[1] = 100;
}

void GameLogic::steerUp(GameLogic::PlayerID _id)
{
   _ id = (int)_id;
   mUserInput->velocityDeltaY[id] -= 1;
}

void GameLogic::steerDown(GameLogic::PlayerID _id)
{
   _ id = (int)_id;
   mUserInput->velocityDeltaY[id] += 1;
}

void GameLogic::accelerate(GameLogic::PlayerID _id)
{
   _ id = (int)_id;
   mUserInput->velocityDeltaX[id] += 1;
}

void GameLogic::decelerate(GameLogic::PlayerID _id)
{
   _ id = (int)_id;
   mUserInput->velocityDeltaX[id] -= 1;
}

void GameLogic::update(const float &_timestep)
{
   SR_ASSERT(mRunning && "Update called but not running");
   // 1: update positions
   for (PlayerID player : {PlayerID::P1, PlayerID::P2})
   {
      _ id = (int)player;
      // apply deltas
      {
         mGameState->velocityX[id] += mUserInput->velocityDeltaX[id];
         mGameState->velocityY[id] += mUserInput->velocityDeltaY[id];
      }
      // clamp velocities
      {
         // horizontal
         if (mGameState->velocityX[id] > sMaxVeloX)
            mGameState->velocityX[id] = sMaxVeloX;
         else if (mGameState->velocityX[id] < sMinVeloX)
            mGameState->velocityX[id] = sMinVeloX;
         // vertical
         if (mGameState->velocityY[id] > sMaxVeloY)
            mGameState->velocityY[id] = sMaxVeloY;
         else if (mGameState->velocityY[id] < sMinVeloY)
            mGameState->velocityY[id] = sMinVeloY;
      }
      // update positions
      {
         mGameState->positionX[id] += _timestep * mGameState->velocityX[id];
         mGameState->positionY[id] += _timestep * mGameState->velocityY[id];
      }
      // clamp positions
      {
         // horizontal
         if (mGameState->positionX[id] - (sCarLength / 2) < 0)
         {
            mGameState->positionX[id] = 0 + sCarLength / 2;
            mGameState->velocityX[id] = 0; // damping of 100%
         }
         else if (mGameState->positionX[id] + (sCarLength / 2) > 100)
         {
            mGameState->positionX[id] = 0 - sCarLength / 2;
            mGameState->velocityX[id] = 0; // damping of 100%
         }
         // vertical
         if (mGameState->positionY[id] - (sCarWidth / 2) < 0)
         {
            mGameState->positionY[id] = 0 + sCarWidth / 2;
            mGameState->velocityY[id] *= -0.5f; // damping of 50% and recoil
         }
         else if (mGameState->positionY[id] + (sCarWidth / 2) > 100)
         {
            mGameState->positionY[id] = 100 - (sCarWidth / 2);
            mGameState->velocityY[id] *= -0.5f; // damping of 50% and recoil
         }
      }
   }
   // 2: resolve conflicts
   {
      // TODO: implement
   }
   // 3: collect coins/rocks
   {
      // TODO: implement}
   }
}
