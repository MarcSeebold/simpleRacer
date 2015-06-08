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

   /// Tells AI the coin position or (-1,-1) if there is none
   void tellCoinPosition(const QVector2D &_pos);
   /// Tells AI the mud puddle position or (-1,-1) if there is none
   void tellMudPosition(const QVector2D &_pos);
   /// Tells AI its own position
   void tellOwnPosition(const QVector2D &_pos);

   void update();

private:
   QVector2D mPosition;
   QVector2D mCoinPosition;
   QVector2D mMudPosition;
   WeakGameLogic mGameLogic;
   PlayerID mID;
   float mDifficulty = 1.f; ///< Value from 0(easy) to 1(hard)
};
