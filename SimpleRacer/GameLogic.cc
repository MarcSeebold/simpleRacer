#include "GameLogic.hh"
#include <algorithm>
#include <QRectF>
#include <iostream>

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
   mGameState = std::make_shared<GameState>();
   mUserInput = UniqueUserInput(new UserInput);

   for (int i : {0, 1})
   {
      mGameState->playerCoins[i] = 0;
      mGameState->positionX[i] = 0;
      mGameState->positionY[i] = 0;
      mGameState->velocityX[i] = 0;
      mGameState->velocityY[i] = 0;
   }
   mGameState->positionY[1] = 100.f;
   mUserInput->reset();
}

void GameLogic::steerUp(GameLogic::PlayerID _id)
{
   _ id = (int)_id;
   mUserInput->velocityDeltaY[id] -= 0.2f;
}

void GameLogic::steerDown(GameLogic::PlayerID _id)
{
   _ id = (int)_id;
   mUserInput->velocityDeltaY[id] += 0.2f;
}

void GameLogic::accelerate(GameLogic::PlayerID _id)
{
   _ id = (int)_id;
   mUserInput->velocityDeltaX[id] += 0.035f;
}

void GameLogic::decelerate(GameLogic::PlayerID _id)
{
   _ id = (int)_id;
   mUserInput->velocityDeltaX[id] -= 0.035f;
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
      // steering/acceleration damping
      {
         mGameState->velocityX[id] *= 0.99f;
         mGameState->velocityY[id] *= 0.99f;
      }
      // clamp positions
      {
         // horizontal
         if (mGameState->positionX[id] - (sCarLength / 2.f) < 0.f)
         {
            mGameState->positionX[id] = 0.f + sCarLength / 2.f;
            mGameState->velocityX[id] = 0.f; // damping of 100%
         }
         else if (mGameState->positionX[id] + (sCarLength / 2.f) > 100.f)
         {
            mGameState->positionX[id] = 100.f - sCarLength / 2.f;
            mGameState->velocityX[id] = 0.f; // damping of 100%
         }
         // vertical
         if (mGameState->positionY[id] - (sCarWidth / 2.f) < 0.f)
         {
            mGameState->positionY[id] = 0.f + sCarWidth / 2.f;
            mGameState->velocityY[id] *= -0.5f; // damping of 50% and recoil
         }
         else if (mGameState->positionY[id] + (sCarWidth / 2.f) > 100.f)
         {
            mGameState->positionY[id] = 100.f - (sCarWidth / 2.f);
            mGameState->velocityY[id] *= -0.5f; // damping of 50% and recoil
         }
      }
   }
   // 2: resolve conflicts
   {
      QPoint topLeft[2];
      for (int p : {0, 1})
         topLeft[p] = QPoint(mGameState->positionX[p] - sCarLength / 2, mGameState->positionY[p] - sCarWidth / 2);

      QRectF car1(topLeft[0], QSize(sCarLength, sCarWidth));
      QRectF car2(topLeft[1], QSize(sCarLength, sCarWidth));

      if (car1.intersects(car2))
      {
         for (int p : {0, 1})
         {
            mGameState->velocityX[p] *= -1.5f;
            mGameState->velocityY[p] *= -5;
            mGameState->positionX[p] += _timestep * mGameState->velocityX[p];
            mGameState->positionY[p] += _timestep * mGameState->velocityY[p];
         }
      }
   }
   // 3: collect coins/rocks
   {
      // TODO: implement
   }
   // reset input
   mUserInput->reset();
}


void GameLogic::UserInput::reset()
{
   velocityDeltaX[0] = 0;
   velocityDeltaX[1] = 0;
   velocityDeltaY[0] = 0;
   velocityDeltaY[1] = 0;
}
