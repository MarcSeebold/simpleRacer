#pragma once
#include <QVector2D>
#include "Common.hh"

#include "GameLogic.hh"

SHARED(class, GameLogic);
/**
 * @brief A simple AI for our racing game
 */
class ArtificialRacer
{
public:
   ArtificialRacer(PlayerID _id, WeakGameLogic _gameLogic);

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
   PlayerID mID;
   float mDifficulty = 0.1f; ///< Value from 0(easy) to 1(hard)
};
