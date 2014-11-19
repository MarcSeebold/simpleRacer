#include "GameLogic.hh"
#include <algorithm>
#include <QRectF>
#include <iostream>
#include <Box2D/Box2D.h>
#include "PhysicsObject.hh"

using namespace simpleRacer;

const float GameLogic::sConversionFactor = 1 / 15.f;
const float GameLogic::sCarHeight = 16 * 3 * GameLogic::sConversionFactor;
const float GameLogic::sCarWidth = 35 * 3 * GameLogic::sConversionFactor;
const float GameLogic::sCoinSize = 30 * GameLogic::sConversionFactor;
const float GameLogic::sGameWidth = 800 * GameLogic::sConversionFactor;
const float GameLogic::sGameHeight = 300 * GameLogic::sConversionFactor;

GameLogic::GameLogic() : mPhysicsWorld(new b2World(b2Vec2(0, 0))) // no gravity

{
   // create street boundaries
   // bottom
   mStreetBoundaries[0] = UniquePhysicsObject(new PhysicsObject(mPhysicsWorld, sGameWidth, 1, sGameWidth / 2, -.5f, 0, true));
   // top
   mStreetBoundaries[1]
       = UniquePhysicsObject(new PhysicsObject(mPhysicsWorld, sGameWidth, 1, sGameWidth / 2, sGameHeight + .5f, 0, true));
   // left
   mStreetBoundaries[2] = UniquePhysicsObject(new PhysicsObject(mPhysicsWorld, 1, sGameHeight, -.5f, sGameHeight / 2, 0, true));
   // right
   mStreetBoundaries[3]
       = UniquePhysicsObject(new PhysicsObject(mPhysicsWorld, 1, sGameHeight, sGameWidth + .5f, sGameHeight / 2, 0, true));
   // reset state
   reset();
}

GameLogic::~GameLogic()
{
}

void GameLogic::reset()
{
   mCoins.clear();
   // create cars
   _ linearDamping = 0.85f;
   // spawn car1 on top left
   mCar1 = UniquePhysicsObject(new PhysicsObject(mPhysicsWorld, sCarWidth, sCarHeight, sCarWidth / 2, sCarHeight / 2, linearDamping));
   // spawn car2 on bottom left
   mCar2 = UniquePhysicsObject(new PhysicsObject(mPhysicsWorld, sCarWidth, sCarHeight, sCarWidth / 2,
                                                 sGameHeight - sCarHeight / 2, linearDamping));

   // reset other member vars
   mUserInput = UniqueUserInput(new UserInput);

   for (int i : {0, 1})
      mPlayerCoins[i] = 0;

   mUserInput->reset();
}

void GameLogic::steerUp(PlayerID _id)
{
   _ id = (int)_id;
   mUserInput->deltaY[id]++;
}

void GameLogic::steerDown(PlayerID _id)
{
   _ id = (int)_id;
   mUserInput->deltaY[id]--;
}

QVector2D GameLogic::getCarCenterPosition(PlayerID _id)
{
   if (_id == PlayerID::P1)
   {
      return mCar1->getCenterPos();
   }
   else
      return mCar2->getCenterPos();
}

std::vector<QVector2D> GameLogic::getCoins()
{
   std::vector<QVector2D> res;
   for (_ const &coin : mCoins)
   {
      res.push_back(coin->getCenterPos());
   }
   return res;
}

void GameLogic::accelerate(PlayerID _id)
{
   _ id = (int)_id;
   mUserInput->deltaX[id]++;
}

void GameLogic::decelerate(PlayerID _id)
{
   _ id = (int)_id;
   mUserInput->deltaX[id]--;
}

void GameLogic::update(const float &_timestep)
{
   SR_ASSERT(mRunning && "Update called but not running");
   // testing: random input for p2
//   if (rand() % 3 == 0)
//      steerUp(PlayerID::P2);
//   if (rand() % 3 == 0)
//      steerDown(PlayerID::P2);
//   if (rand() % 5 == 0)
//      accelerate(PlayerID::P2);
//   if (rand() % 6 == 0)
//      decelerate(PlayerID::P2);

   if (mCoins.size() < 1)
      spawnCoin();


   // 1: apply input
   float factorX = 2;
   float factorY = 2;
   mCar1->applyForce(QVector2D(mUserInput->deltaX[0] * factorX, 0));
   mCar1->applyForce(QVector2D(0, mUserInput->deltaY[0] * factorY));
   mCar2->applyForce(QVector2D(mUserInput->deltaX[1] * factorX, 0));
   mCar2->applyForce(QVector2D(0, mUserInput->deltaY[1] * factorY));

   // 2: step simulation
   {
      int32 velocityIterations = 4;
      int32 positionIterations = 2;
      mPhysicsWorld->Step(_timestep, velocityIterations, positionIterations);
   }
   // 3: collect coins/rocks
   {
      // TODO: implement
   }
   // reset input
   mUserInput->reset();
}

void GameLogic::spawnCoin()
{
   // todo: spawn coins far away from cars
   float posX = 0.f + (rand() % (int)(sGameWidth - sCoinSize));
   float posY = 0.f + (rand() % (int)(sGameHeight - sCoinSize));
   _ coin = UniquePhysicsObject(new PhysicsObject(mPhysicsWorld, sCoinSize, sCoinSize, posX, posY, 0, true));
   mCoins.push_back(std::move(coin));
}


void GameLogic::UserInput::reset()
{
   deltaX[0] = 0;
   deltaX[1] = 0;
   deltaY[0] = 0;
   deltaY[1] = 0;
}
