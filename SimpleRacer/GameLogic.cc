#include "GameLogic.hh"
#include <algorithm>
#include <QRectF>
#include <iostream>
#include <algorithm>
#include <Box2D/Box2D.h>
#include "PhysicsObject.hh"
#include "ContactListener.hh"
#include "NetworkEngine.hh"
#include "ArtificialRacer.hh"
#include "SimpleRacer.hh"
#include "LagSettings.hh"
#include <qmath.h>

#include <qdebug.h>

const float GameLogic::sConversionFactor = 1 / 15.f;
const float GameLogic::sCarHeight = 16.f * 3 * GameLogic::sConversionFactor;
const float GameLogic::sCarWidth = 35.f * 3 * GameLogic::sConversionFactor;
const float GameLogic::sCoinSize = 30.f * GameLogic::sConversionFactor;
const float GameLogic::sGameWidth = 800.f * GameLogic::sConversionFactor;
const float GameLogic::sGameHeight = 300.f * GameLogic::sConversionFactor;

GameLogic::GameLogic(Type _type)
  : mType(_type),
    mPhysicsWorld(new b2World(b2Vec2(0, 0))),    // no gravity
    mPhysicsWorldOld(new b2World(b2Vec2(0, 0))), // no gravity
    mContactListener(new ContactListener())

{
   // set custom contact listener
   mContactListener->registerCallbackCarCoin([this](Car *car, Coin *coin)
                                             {
                                                callbackCarCoin(car, coin);
                                             });
   mContactListener->registerCallbackCarCar([this](Car *car, Car *coin)
                                            {
                                               callbackCarCar(car, coin);
                                            });
   mContactListener->registerCallbackCarBoundary([this](Car *car, Boundary *coin)
                                                 {
                                                    callbackCarBoundary(car, coin);
                                                 });
   mPhysicsWorld->SetContactListener(mContactListener.get());
   // create street boundaries
   // world
   {
      // bottom
      mStreetBoundaries[0] = UniqueBoundary(new Boundary(mPhysicsWorld, sGameWidth, 1, sGameWidth / 2, -.5f));
      // top
      mStreetBoundaries[1] = UniqueBoundary(new Boundary(mPhysicsWorld, sGameWidth, 1, sGameWidth / 2, sGameHeight + .5f));
      // left
      mStreetBoundaries[2] = UniqueBoundary(new Boundary(mPhysicsWorld, 1, sGameHeight, -.5f, sGameHeight / 2));
      // right
      mStreetBoundaries[3] = UniqueBoundary(new Boundary(mPhysicsWorld, 1, sGameHeight, sGameWidth + .5f, sGameHeight / 2));
   }
   // "old" world
   {
      // bottom
      mStreetBoundariesOld[0] = UniqueBoundary(new Boundary(mPhysicsWorldOld, sGameWidth, 1, sGameWidth / 2, -.5f));
      // top
      mStreetBoundariesOld[1] = UniqueBoundary(new Boundary(mPhysicsWorldOld, sGameWidth, 1, sGameWidth / 2, sGameHeight + .5f));
      // left
      mStreetBoundariesOld[2] = UniqueBoundary(new Boundary(mPhysicsWorldOld, 1, sGameHeight, -.5f, sGameHeight / 2));
      // right
      mStreetBoundariesOld[3]
          = UniqueBoundary(new Boundary(mPhysicsWorldOld, 1, sGameHeight, sGameWidth + .5f, sGameHeight / 2));
   }

   // reset state
   reset();
}

GameLogic::~GameLogic()
{
   mCar1 = nullptr;
   mCar2 = nullptr;
   for (int i = 0; i < 4; ++i)
      mStreetBoundaries[i] = nullptr;
   for (int i = 0; i < 4; ++i)
      mStreetBoundariesOld[i] = nullptr;
   mContactListener = nullptr;
   mCoins.clear();
   mPhysicsWorld = nullptr;
   mPhysicsWorldOld = nullptr;
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
   {
      // cars for the "past" world
      // spawn car1 on top left
      mCar1Old = UniqueCar(new Car(mPhysicsWorldOld, sCarWidth, sCarHeight, sCarWidth / 2, sCarHeight / 2, linearDamping));
      // spawn car2 on bottom left
      mCar2Old = UniqueCar(new Car(mPhysicsWorldOld, sCarWidth, sCarHeight, sCarWidth / 2, sGameHeight - sCarHeight / 2, linearDamping));
   }

   // reset other member vars
   mAIInput = UniqueAIInput(new AIInput);

   for (int i : {0, 1})
      mScore[i] = 0;

   mAIInput->reset();
}

void GameLogic::steerUp(PlayerID _id)
{
   _ id = (int)_id;
   mAIInput->deltaY[id]++;
}

void GameLogic::steerDown(PlayerID _id)
{
   _ id = (int)_id;
   mAIInput->deltaY[id]--;
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

void GameLogic::setCarPositionVelocity(PlayerID _player, const QVector2D &_pos, const QVector2D &_velo)
{
   SR_ASSERT(isClient() && "Client-only function");

   static float maxOffsetSquared = 10 * 10;
   const UniqueCar &carNew = (_player == PlayerID::P1) ? mCar1 : mCar2;

   if (LagSettings::the()->getClientSidePrediction())
   { // client side prediction
      const UniqueCar &car = (_player == PlayerID::P1) ? mCar1Old : mCar2Old;
      // update past state
      car->setCenterPos(_pos);
      car->setLinearVelocity(_velo);
      // predict current position
      mPhysicsWorldOld->Step(LagSettings::the()->getLatencyServerToClient(), 4, 8);
      // update current state
      _ posDiff = car->getCenterPos() - carNew->getCenterPos();
      // do not interpolate if offset is too high
      if (LagSettings::the()->getClientSideInterpolation() && !(posDiff.lengthSquared() >= maxOffsetSquared / 2))
      { // interpolate
         // we use a exponential function for position correction
         // why: small errors are tolerable, but big errors not
         _ signedPow = [](float x) -> float
         {
            bool minus = (x < 0);
            x = qAbs(x);
            x = x * x;
            x *= (minus ? -1.f : 1.f);
            return x;
         };
         _ squaredPosDiff = QVector2D(signedPow(posDiff.x()), signedPow(posDiff.y()));
         _ velo = squaredPosDiff * LagSettings::the()->getClientSideInterpolationFactor();
         // the current velocity already cointains user input
         carNew->setLinearVelocity(car->getLinearVelocity() + velo);
      }
      else
      { // hard-set
         std::cerr << "hard-set" << std::endl;
         carNew->setCenterPos(car->getCenterPos());
         carNew->setLinearVelocity(car->getLinearVelocity());
      }
   }
   else
   { // no client-side prediction
      carNew->setCenterPos(_pos);
      carNew->setLinearVelocity(_velo);
   }
}

void GameLogic::accelerate(PlayerID _id)
{
   _ id = (int)_id;
   mAIInput->deltaX[id]++;
}

void GameLogic::decelerate(PlayerID _id)
{
   _ id = (int)_id;
   mAIInput->deltaX[id]--;
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
   // 5: Update delayed stuff
   mDelayedLagDisabling.update();
   mDelayedServerCarPosUpdate.update();
   // 0: server-side lag compensation: step old physics world and update present
   if (isServer() && LagSettings::the()->getServerSideLagCompensation())
   {
      // server-side lag compensation for player 1
      // step old physics world to present
      // it would be more accurate to re-play AI's input. however, this is not so easy.
      mPhysicsWorldOld->Step(LagSettings::the()->getLatencyClientToServer() * 0.5f - _timestep, 4, 8);
      // store new data (only for P1!, P2 lives on the server)
      mCar1->setCenterPos(mCar1Old->getCenterPos());
      mCar1->setLinearVelocity(mCar1Old->getLinearVelocity());
   }
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
      float factorX = 30;
      float factorY = 12;
      // player
      int dirX = (mKeyStatus.right ? 1 : 0) + (mKeyStatus.left ? -1 : 0);
      int dirY = (mKeyStatus.up ? 1 : 0) + (mKeyStatus.down ? -1 : 0);
      mCar1->applyForce(QVector2D(dirX * factorX, 0));
      mCar1->applyForce(QVector2D(0, dirY * factorY));
      // AI
      mCar2->applyForce(QVector2D(mAIInput->deltaX[1] * factorX, 0));
      mCar2->applyForce(QVector2D(0, mAIInput->deltaY[1] * factorY));
#if 0
      // do this for "old" game state too
      if (isServer())
      {
         _ func = [this, factorX, factorY, dirX, dirY]()
         {
            mCar1Old->applyForce(QVector2D(dirX * factorX, 0));
            mCar1Old->applyForce(QVector2D(0, dirY * factorY));
            // Set AI car directly
            mCar2Old->setCenterPos(mCar2->getCenterPos());
            mCar2Old->setLinearVelocity(mCar2->getLinearVelocity());
            mCar2Old->applyForce(QVector2D(mAIInput->deltaX[1] * factorX, 0));
            mCar2Old->applyForce(QVector2D(0, mAIInput->deltaY[1] * factorY));
         };
         // execute it after x seconds
         mDelayedLagDisabling.pushDelayedAction(func, LagSettings::the()->getLatencyClientToServer());
      }
#endif
   }
   // 2: step simulation
   {
      if (LagSettings::the()->getClientSidePhysics() || isServer())
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
   // 4: Keep a copy of the world state for X seconds (for server-side lag compensation)
   if (isServer())
   {
      _ func = [this]()
      {
         // set car positions and velocities
         mCar1Old->setCenterPos(mCar1->getCenterPos());
         mCar2Old->setCenterPos(mCar2->getCenterPos());
         mCar1Old->setLinearVelocity(mCar1->getLinearVelocity());
         mCar2Old->setLinearVelocity(mCar2->getLinearVelocity());
      };
      // execute it after x seconds
      mDelayedLagDisabling.pushDelayedAction(func, LagSettings::the()->getLatencyClientToServer());
   }
   // reset AI input
   mAIInput->reset();
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

void GameLogic::callbackCarCoin(Car *_car, Coin *_coin)
{
   criticalSituationOccured();
   if (!LagSettings::the()->getClientSidePhysics() && !isServer())
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

void GameLogic::callbackCarCar(Car *_carA, Car *_carB)
{
   (void)&_carA;
   (void)&_carB;
   criticalSituationOccured();
}

void GameLogic::callbackCarBoundary(Car *_car, Boundary *_boundary)
{
   (void)&_car;
   (void)&_boundary;
   criticalSituationOccured();
}

void GameLogic::criticalSituationOccured()
{
   _ probability = LagSettings::the()->getLagProbability();
   float random = rand() % 100;
   if (random <= 100.f * probability)
   {
      // activate lag for a period of time
      LagSettings::the()->setLagEnabled(true);

      mDelayedLagDisabling.clear(); // ensure there is only one delayed task
      _ func = [this]()
      {
         LagSettings::the()->setLagEnabled(false);
      };
      mDelayedLagDisabling.pushDelayedAction(func, LagSettings::the()->getLagDuration());
   }
}

void GameLogic::AIInput::reset()
{
   deltaX[0] = 0;
   deltaX[1] = 0;
   deltaY[0] = 0;
   deltaY[1] = 0;
}
