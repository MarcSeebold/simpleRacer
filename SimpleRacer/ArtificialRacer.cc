#include "ArtificialRacer.hh"

#include <algorithm>
#include <limits>

using namespace simpleRacer;

ArtificialRacer::ArtificialRacer(PlayerID _id) : mNextGoal(-1, -1), mPosition(-1, -1), mID(_id)
{
}

void ArtificialRacer::setGameLogic(SharedGameLogic _gameLogic)
{
   mGameLogic = _gameLogic;
}

void ArtificialRacer::tellCoinHasBeenSpawned(const QVector2D &_pos)
{
   mCoinPositions.push_back(_pos);
}

void ArtificialRacer::tellCoinHasBeenCollected(const QVector2D &_pos)
{
   if (_pos == mNextGoal)
      mNextGoal = QVector2D(-1, -1);

   // remove coin from list
   for (_ it = mCoinPositions.begin(); it != mCoinPositions.end(); ++it)
   {
      if (*it == _pos)
      {
         mCoinPositions.erase(it);
         break;
      }
   }
}

void ArtificialRacer::tellOwnPosition(const QVector2D &_pos)
{
   mPosition = _pos;
}

void ArtificialRacer::update()
{
   SharedGameLogic logic = mGameLogic.lock();
   SR_ASSERT(logic && "No GameLogic");

   // is current goal still valid?
   if (mNextGoal == QVector2D(-1, -1))
   {
      // invalid goal. search new.
      if (mCoinPositions.empty())
         return; // no possible new goals
      mNextGoal = getClosestCoin();
   }

   // move us closer to next goal
   if (std::abs(mPosition.x() - mNextGoal.x()) >= GameLogic::sCarWidth/2)
   {
      if (mPosition.x() < mNextGoal.x())
      {
         logic->accelerate(mID);
      }
      else
         logic->decelerate(mID);
   }

   if (std::abs(mPosition.y() - mNextGoal.y()) >= GameLogic::sCarHeight/2)
   {
      if (mPosition.y() < mNextGoal.y())
      {
         logic->steerUp(mID);
      }
      else
         logic->steerDown(mID);
   }
}

QVector2D ArtificialRacer::getClosestCoin()
{
   QVector2D closestCoin(-1, -1);
   float distance = std::numeric_limits<float>::max();

   for (_ const &coin : mCoinPositions)
   {
      _ currDistance = mPosition.distanceToPoint(coin);
      if (currDistance < distance)
      {
         closestCoin = coin;
         distance = currDistance;
      }
   }

   return closestCoin;
}
