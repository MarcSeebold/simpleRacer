#include "GameLogic.hh"
#include <algorithm>
#include <QRectF>
#include <iostream>
#include <algorithm>
#include <Box2D/Box2D.h>
#include "PhysicsObject.hh"
#include "PhysicsContactListener.hh"
#include "NetworkEngine.hh"
#include "ArtificialRacer.hh"
#include "SimpleRacer.hh"
#include "LagSettings.hh"

#include <qdebug.h>

const float GameLogic::sConversionFactor = 1 / 15.f;
const float GameLogic::sCarHeight = 16.f * 3 * GameLogic::sConversionFactor;
const float GameLogic::sCarWidth = 35.f * 3 * GameLogic::sConversionFactor;
const float GameLogic::sCoinSize = 30.f * GameLogic::sConversionFactor;
const float GameLogic::sGameWidth = 800.f * GameLogic::sConversionFactor;
const float GameLogic::sGameHeight = 300.f * GameLogic::sConversionFactor;

GameLogic::GameLogic(Type _type)
  : mType(_type),
    mPhysicsWorld(new b2World(b2Vec2(0, 0))), // no gravity
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
   _ linearDamping = 2.f;
   // spawn car1 on top left
   mCar1 = UniqueCar(new Car(mPhysicsWorld, sCarWidth, sCarHeight, sCarWidth / 2, sCarHeight / 2, linearDamping));
   // spawn car2 on bottom left
   mCar2 = UniqueCar(new Car(mPhysicsWorld, sCarWidth, sCarHeight, sCarWidth / 2, sGameHeight - sCarHeight / 2, linearDamping));

   // reset other member vars
   mUserInput = UniqueUserInput(new UserInput);

   for (int i : {0, 1})
      mScore[i] = 0;

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
      return mCar1->getCenterPos();
   else
      return mCar2->getCenterPos();
}

QVector2D GameLogic::getCarLinearVelocity(PlayerID _id)
{
   if (_id == PlayerID::P1)
      return mCar1->getLinearVelocity();
   else
      return mCar2->getLinearVelocity();
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

int GameLogic::getScore(PlayerID _id)
{
   _ id = (int)_id;
   return mScore[id];
}

void GameLogic::setCoins(const std::vector<QVector2D> &_coins)
{
   mCoins.clear();
   for (_ const &c : _coins)
   {
      _ coin = UniqueCoin(new Coin(mPhysicsWorld, sCoinSize, sCoinSize, c.x(), c.y()));
      mCoins.push_back(std::move(coin));
   }
}

void GameLogic::setCarPositionVelocity(PlayerID _player, const QVector2D &_pos, const QVector2D &_velo, bool _interpolate)
{
   const UniqueCar &car = (_player == PlayerID::P1) ? mCar1 : mCar2;
   if (!_interpolate)
   {
      car->setCenterPos(_pos);
      car->setLinearVelocity(_velo);
   }
   else
   {
      // TODO
      std::cerr << "Not implemented" << std::endl;
   }
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

   if (mCoins.size() < 1)
      spawnCoin();
   // 1: apply input
   {
      // Server: Client input
      if (isServer())
      {
         if (mKeyStatus.down)
         {
            steerDown(PlayerID::P1);
         }
         if (mKeyStatus.left)
         {
            decelerate(PlayerID::P1);
         }
         if (mKeyStatus.right)
         {
            accelerate(PlayerID::P1);
         }
         if (mKeyStatus.up)
         {
            steerUp(PlayerID::P1);
         }
      }
      float factorX = 10;
      float factorY = 6;
      mCar1->applyForce(QVector2D(mUserInput->deltaX[0] * factorX, 0));
      mCar1->applyForce(QVector2D(0, mUserInput->deltaY[0] * factorY));
      mCar2->applyForce(QVector2D(mUserInput->deltaX[1] * factorX, 0));
      mCar2->applyForce(QVector2D(0, mUserInput->deltaY[1] * factorY));
      // server only: client input
   }
   // 2: step simulation
   {
      if (lagSettings::clientSidePhysics || isServer())
      {
         int32 velocityIterations = 4;
         int32 positionIterations = 8;
         mPhysicsWorld->Step(_timestep, velocityIterations, positionIterations);
      }
   }
   // 3: collect coins/rocks
   {
      // Done via collision-callback: coinCallback(...)
   }
   // reset input
   mUserInput->reset();
}

void GameLogic::spawnCoin()
{
   // only server must create new coins
   if (!isServer())
      return;
   // todo: spawn coins far away from cars
   float posX = sCoinSize / 2.f + (rand() % (int)(sGameWidth - sCoinSize));
   float posY = sCoinSize / 2.f + (rand() % (int)(sGameHeight - sCoinSize));
   _ coin = UniqueCoin(new Coin(mPhysicsWorld, sCoinSize, sCoinSize, posX, posY));
   mCoins.push_back(std::move(coin));
   // tell AI
   if (mCoinSpawnCallback)
      mCoinSpawnCallback(QVector2D(posX, posY));
}

void GameLogic::coinCallback(Car *_car, Coin *_coin)
{
   if (!lagSettings::clientSidePhysics && !isServer())
      return; // only server should handle this
   // TODO: switch variable for handling this client or/and serverside
   PlayerID player;
   if (mCar1.get() == _car)
      player = PlayerID::P1;
   else if (mCar2.get() == _car)
      player = PlayerID::P2;
   else
   {
      SR_ASSERT(0 && "Unknown car");
      return;
   }

   mCoinsToRemove.push_back(_coin);
   if (mCoinCollectedCallback)
      mCoinCollectedCallback(_coin->getCenterPos());

   ++mScore[(int)player];
}

void GameLogic::UserInput::reset()
{
   deltaX[0] = 0;
   deltaX[1] = 0;
   deltaY[0] = 0;
   deltaY[1] = 0;
}
