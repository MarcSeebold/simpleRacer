#include "ArtificialRacer.hh"

#include <algorithm>
#include <limits>
#include <cassert>
#include <qdebug.h>

namespace
{
// get shortest distance between two bounding boxes
QVector2D minDistanceBetween(const QVector2D &_objA, const QVector2D &_objB, const QVector2D &_objASize, const QVector2D &_objBSize)
{
   float distX, distY;
   bool objALeftToB = (_objA.x() < _objB.x());
   bool objAAboveB = (_objA.y() > _objB.y());
   // x
   if (objALeftToB)
      distX = (_objB.x() - _objBSize.x() / 2.f) - (_objA.x() + _objASize.x() / 2.f);
   else
      distX = (_objA.x() - _objASize.x() / 2.f) - (_objB.x() + _objBSize.x() / 2.f);
   // y
   if (objAAboveB)
      distY = (_objA.y() - _objASize.y() / 2.f) - (_objB.y() + _objBSize.y() / 2.f);
   else
      distY = (_objB.y() - _objBSize.y() / 2.f) - (_objA.y() + _objASize.y() / 2.f);
   return QVector2D(std::abs(distX), std::abs(distY));
}
}

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

void ArtificialRacer::tellEnemyPosition(const QVector2D &_pos)
{
   mEnemyPosition = _pos;
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
      coinPos = QVector2D((GameLogic::sGameWidth / 2.f), (GameLogic::sGameHeight / 2.f));
   }

// Seperate logic for x and y axes
// Corner case: mud is in same lane as coin
#if 0
   bool mudSameLaneAsCoin = std::abs(mMudPosition.y() - coinPos.y()) <= GameLogic::sMudSize;
#endif
   bool coinAboveUs = (coinPos.y() >= mPosition.y());
   // bool mudLeftToCar = (mMudPosition.x() < mPosition.x());
   bool mudAboveCar = (mMudPosition.y() > mPosition.y());
   bool enemyAboveCar = (mEnemyPosition.y() > mPosition.y());
   // distances
   _ distToMud = minDistanceBetween(mPosition, mMudPosition, QVector2D(GameLogic::sCarWidth, GameLogic::sCarHeight),
                                    QVector2D(GameLogic::sMudSize, GameLogic::sMudSize));
   _ distToEnemy = minDistanceBetween(mPosition, mEnemyPosition, QVector2D(GameLogic::sCarWidth, GameLogic::sCarHeight),
                                      QVector2D(GameLogic::sCarWidth, GameLogic::sCarHeight));
   _ distToCoin = minDistanceBetween(mPosition, coinPos, QVector2D(GameLogic::sCarWidth, GameLogic::sCarHeight),
                                     QVector2D(GameLogic::sCoinSize, GameLogic::sCoinSize)).length();
   bool enemyClose = distToEnemy.x() < 1.f || distToEnemy.y() < 1.f;
   bool enemyOnOurLane = std::abs(mPosition.y() - mEnemyPosition.y()) < GameLogic::sCarHeight;
   // B
   bool borderAboveClose = std::abs(mPosition.y() + GameLogic::sCarHeight / 2.f - GameLogic::sGameHeight) < 2.f;
   bool borderBelowClose = std::abs(mPosition.y() - GameLogic::sCarHeight / 2.f) < 2.f;
   bool borderRightClose = std::abs(mPosition.x() + GameLogic::sCarWidth / 2.f - GameLogic::sGameWidth) < 2.f;
   bool borderLeftClose = (mPosition.x() - GameLogic::sCarWidth / 2.f) < 2.f;
   bool enemyDirectlyBehindUs = enemyClose && (mPosition.x() > mEnemyPosition.x());

   if (enemyOnOurLane && enemyClose && mDistToCoin > distToCoin && mCoinPosition != QVector2D(-1, -1))
   {
      if (mEnemyMockingUs < 100)
         mEnemyMockingUs++;
   }
   else
   {
      if (mEnemyMockingUs > 0)
         mEnemyMockingUs--;
   }
   bool mudIsNear = (distToMud.length() < GameLogic::sMudSize * 1.2f);
   mDistToCoin = distToCoin;

   // enable cheating
   if (mEnemyMockingUs > 20)
      logic->setAICheating(true);
   else
      logic->setAICheating(false);
   if (mEnemyMockingUs > 50) // push enemy to right border
   {
      // are we in a corner? escape it!
      {
         // top right
         if (borderAboveClose && borderRightClose)
         {
            logic->decelerate(mID);
            logic->steerDown(mID);
            return;
         }
         // top left
         else if (borderAboveClose && borderLeftClose)
         {
            logic->accelerate(mID);
            logic->steerDown(mID);
            return;
         }
         // bottom right
         else if (borderBelowClose && borderRightClose)
         {
            logic->decelerate(mID);
            logic->steerUp(mID);
            return;
         }
         // bottom left
         else if (borderBelowClose && borderLeftClose)
         {
            logic->accelerate(mID);
            logic->steerUp(mID);
            return;
         }
      }
      if (enemyDirectlyBehindUs)
         logic->accelerate(mID);
      else
         logic->decelerate(mID);
      // try to get in front of him
      if (!enemyAboveCar)
         logic->steerUp(mID);
      else
         logic->steerDown(mID);
      return;
   }

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
      { // no near mud: get closer to coin and avoid borders
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
         bool ourLaneIsMudLane = std::abs(mMudPosition.y() - mPosition.y()) < GameLogic::sCarHeight/2.f * 1.5f;
         // avoid mud
         if (mMudPosition.x() < mPosition.x())
         {
            // mud is left from us
            logic->accelerate(mID);
         }
         else if (!ourLaneIsMudLane)
            logic->accelerate(mID); // our lane is clear
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
