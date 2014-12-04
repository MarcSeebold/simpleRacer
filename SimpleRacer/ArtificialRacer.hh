#pragma once
#include <QVector2D>
#include "Common.hh"

#include "GameLogic.hh"


namespace simpleRacer
{
SHARED(class, GameLogic);
/**
 * @brief A simple AI for our racing game
 */
class ArtificialRacer
{
public:
   ArtificialRacer(GameLogic::PlayerID _id);

   void setGameLogic(SharedGameLogic _gameLogic);

   void tellCoinHasBeenSpawned(const QVector2D &_pos);
   void tellCoinHasBeenCollected(const QVector2D &_pos);
   void tellOwnPosition(const QVector2D &_pos);

   void update();

private:
   /// Returns the closest coin or (-1,-1) if there are no coins.
   QVector2D getClosestCoin();

private:
   QVector2D mNextGoal;
   QVector2D mPosition;
   std::vector< QVector2D > mCoinPositions;
   WeakGameLogic mGameLogic;
   GameLogic::PlayerID mID;
};
} // namespace simpleRacer