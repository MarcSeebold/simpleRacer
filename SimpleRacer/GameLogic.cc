#include "GameLogic.hh"
#include <algorithm>
#include <QRectF>
#include <iostream>
#include <algorithm>
#include <Box2D/Box2D.h>
#include "PhysicsObject.hh"
#include "PhysicsContactListener.hh"

#include <qdebug.h>

using namespace simpleRacer;

const float GameLogic::sConversionFactor = 1 / 15.f;
const float GameLogic::sCarHeight = 16.f * 3 * GameLogic::sConversionFactor;
const float GameLogic::sCarWidth = 35.f * 3 * GameLogic::sConversionFactor;
const float GameLogic::sCoinSize = 30.f * GameLogic::sConversionFactor;
const float GameLogic::sGameWidth = 800.f * GameLogic::sConversionFactor;
const float GameLogic::sGameHeight = 300.f * GameLogic::sConversionFactor;

GameLogic::GameLogic()
  : mPhysicsWorld(new b2World(b2Vec2(0, 0))), // no gravity
    mContactListener(new PhysicsContactListener())

{
   // set custom contact listener
   mContactListener->registerCallback([this](Car *car, Coin *coin)
                                      {
                                         coinCallback(car, coin);
                                      });
   mPhysicsWorld->SetContactListener(mContactListener.get());

   // create street boundaries
   // bottom
   mStreetBoundaries[0] = UniquePhysicsObject(
       new PhysicsObject(mPhysicsWorld, sGameWidth, 1, sGameWidth / 2, -.5f, PhysicsObject::Type::BOUNDARY, 0, true));
   // top
   mStreetBoundaries[1] = UniquePhysicsObject(new PhysicsObject(
       mPhysicsWorld, sGameWidth, 1, sGameWidth / 2, sGameHeight + .5f, PhysicsObject::Type::BOUNDARY, 0, true));
   // left
   mStreetBoundaries[2] = UniquePhysicsObject(
       new PhysicsObject(mPhysicsWorld, 1, sGameHeight, -.5f, sGameHeight / 2, PhysicsObject::Type::BOUNDARY, 0, true));
   // right
   mStreetBoundaries[3] = UniquePhysicsObject(new PhysicsObject(
       mPhysicsWorld, 1, sGameHeight, sGameWidth + .5f, sGameHeight / 2, PhysicsObject::Type::BOUNDARY, 0, true));
   // reset state
   reset();
}

GameLogic::~GameLogic()
{
   mUserInput = nullptr;
   mCar1 = nullptr;
   mCar2 = nullptr;
   for (int i = 0; i < 4; ++i)
      mStreetBoundaries[i] = nullptr;
   mContactListener = nullptr;
   mCoins.clear();
   mPhysicsWorld = nullptr;
}

void GameLogic::reset()
{
   mCoins.clear();
   // create cars
   _ linearDamping = 0.85f;
   // spawn car1 on top left
   mCar1 = UniqueCar(new Car(mPhysicsWorld, sCarWidth, sCarHeight, sCarWidth / 2, sCarHeight / 2, linearDamping));
   // spawn car2 on bottom left
   mCar2 = UniqueCar(new Car(mPhysicsWorld, sCarWidth, sCarHeight, sCarWidth / 2, sGameHeight - sCarHeight / 2, linearDamping));

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
   // Remove coins
   for (Coin *coin : mCoinsToRemove)
   {
      mCoins.erase(std::remove_if(mCoins.begin(), mCoins.end(), [coin](const UniqueCoin &_curr)
                                  {
                      if (_curr.get() == coin)
                         return true;
                      return false;
                   }),
                   mCoins.end());
   }
   mCoinsToRemove.clear();


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
      int32 positionIterations = 8;
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
   float posX = 0.f + (rand() % (int)(sGameWidth - sCoinSize / 2));
   float posY = 0.f + (rand() % (int)(sGameHeight - sCoinSize / 2));
   _ coin = UniqueCoin(new Coin(mPhysicsWorld, sCoinSize, sCoinSize, posX, posY));
   mCoins.push_back(std::move(coin));
}

void GameLogic::coinCallback(Car *_car, Coin *_coin)
{
   PlayerID player;
   if (mCar1.get() == _car)
      player = PlayerID::P1;
   else if (mCar2.get() == _car)
      player = PlayerID::P1;
   else
   {
      SR_ASSERT(0 && "Unknown car");
      return;
   }

   mCoinsToRemove.push_back(_coin);

   ++mPlayerCoins[(int)player];
   qDebug() << "coin! player: " << (int)player;
}


void GameLogic::UserInput::reset()
{
   deltaX[0] = 0;
   deltaX[1] = 0;
   deltaY[0] = 0;
   deltaY[1] = 0;
}
