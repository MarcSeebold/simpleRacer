#include "ArtificialRacer.hh"

#include <algorithm>
#include <limits>
#include <cassert>
#include <qdebug.h>

ArtificialRacer::ArtificialRacer(PlayerID _id, WeakGameLogic _gameLogic)
  : mPosition(-1, -1), mGameLogic(std::forward<WeakGameLogic>(_gameLogic)), mID(_id)
{
}

void ArtificialRacer::tellCoinPosition(const QVector2D &_pos)
{
   mCoinPosition = _pos;
}

void ArtificialRacer::tellMudPosition(const QVector2D &_pos)
{
   mMudPosition = _pos;
}

void ArtificialRacer::tellOwnPosition(const QVector2D &_pos)
{
   mPosition = _pos;
}

void ArtificialRacer::update()
{
   SharedGameLogic logic = mGameLogic.lock();
   SR_ASSERT(logic && "No GameLogic");

   SR_ASSERT(mDifficulty >= 0 && mDifficulty <= 1);
   int difficultyFactor = 3 + int(10 * mDifficulty);
   if (rand() % difficultyFactor == 0)
   {
      logic->accelerate(mID);
      return;
   }
   if (rand() % difficultyFactor == 0)
   {
      logic->decelerate(mID);
      return;
   }
   if (rand() % difficultyFactor == 0)
   {
      logic->steerDown(mID);
      return;
   }
   if (rand() % difficultyFactor == 0)
   {
      logic->steerUp(mID);
      return;
   }

   _ coinPos = mCoinPosition;
   // is current goal still valid?
   if (coinPos == QVector2D(-1, -1))
   {
      // No goal: hold distance to right border
      coinPos = QVector2D((GameLogic::sGameWidth - GameLogic::sCarWidth * 2.5f), (GameLogic::sGameHeight / 2.f));
   }

   // Seperate logic for x and y axes
   // Corner case: mud is in same lane as coin
   bool mudSameLaneAsCoin = std::abs(mMudPosition.y() - coinPos.y()) <= GameLogic::sMudSize;
   bool coinAboveUs = (coinPos.y() >= mPosition.y());
   bool mudLeftToCar = (mMudPosition.x() < mPosition.x());
   bool mudAboveCar = (mMudPosition.y() > mPosition.y());
   QVector2D distToMud;
   // get shortest distance between bounding box of mud and car
   {
      float distX, distY;
      // x
      if (mudLeftToCar)
         distX = (mPosition.x() - GameLogic::sCarWidth / 2.f) - (mMudPosition.x() + GameLogic::sMudSize / 2.f);
      else
         distX = (mMudPosition.x() - GameLogic::sMudSize / 2.f) - (mPosition.x() + GameLogic::sCarWidth / 2.f);
      // y
      if (mudAboveCar)
         distY = (mMudPosition.y() - GameLogic::sMudSize / 2.f) - (mPosition.y() + GameLogic::sCarHeight / 2.f);
      else
         distY = (mPosition.y() - GameLogic::sCarHeight / 2.f) - (mMudPosition.y() + GameLogic::sMudSize / 2.f);
      distToMud = QVector2D(std::abs(distX), std::abs(distY));
   }
   bool mudIsNear = (distToMud.length() < GameLogic::sMudSize * 1.5f);

   // 1) y-axis
   {
      if (mudIsNear)
      { // avoid mud
         if (!mudAboveCar)
            logic->steerUp(mID);
         else
            logic->steerDown(mID);
      }
      else
      { // no near mud: get closer to coin
         if (coinAboveUs)
            logic->steerUp(mID);
         else
            logic->steerDown(mID);
      }
   }
   // 2) x-axis
   {
      if (mudIsNear)
      {
         // avoid mud
         if (mMudPosition.x() < mPosition.x())
         {
            // mud is left from us
            logic->accelerate(mID);
         }
         else
            logic->decelerate(mID);
      }
      else
      {
         if (coinPos.x() > mPosition.x())
         { // coin is right from us: accelerate
            logic->accelerate(mID);
         }
         else
            logic->decelerate(mID);
      }
   }
}
