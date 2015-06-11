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
#include "Settings.hh"
#include "StatisticsEngine.hh"
#include <qmath.h>

#include <qdebug.h>

const float GameLogic::sConversionFactor = 1 / 15.f;
const float GameLogic::sCarHeight = 16.f * 3 * GameLogic::sConversionFactor;
const float GameLogic::sCarWidth = 35.f * 3 * GameLogic::sConversionFactor;
const float GameLogic::sCoinSize = 30.f * GameLogic::sConversionFactor;
const float GameLogic::sMudSize = 50.f * GameLogic::sConversionFactor;
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
   mContactListener->registerCallbackCarMud([this](Car *car, Mud *mud)
                                            {
                                               callbackCarMud(car, mud);
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
   mMuds.clear();
   mPhysicsWorld = nullptr;
   mPhysicsWorldOld = nullptr;
}

void GameLogic::reset()
{
   mCoins.clear();
   mMuds.clear();
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

std::vector<QVector2D> GameLogic::getMuds()
{
   std::vector<QVector2D> res;
   for (_ const &mud : mMuds)
   {
      res.push_back(mud->getCenterPos());
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

void GameLogic::setMuds(const std::vector<QVector2D> &_muds)
{
   mMuds.clear();
   for (_ const &m : _muds)
   {
      _ mud = UniqueMud(new Mud(mPhysicsWorld, sMudSize, sMudSize, m.x(), m.y()));
      mMuds.push_back(std::move(mud));
   }
}

void GameLogic::setCarPositionVelocity(PlayerID _player, const QVector2D &_pos, const QVector2D &_velo)
{
   SR_ASSERT(isClient() && "Client-only function");

   static float maxOffsetSquared = 10 * 10;
   const UniqueCar &carNew = (_player == PlayerID::P1) ? mCar1 : mCar2;

   if (Settings::the()->getClientSidePrediction())
   { // client side prediction
      const UniqueCar &car = (_player == PlayerID::P1) ? mCar1Old : mCar2Old;
      // update past state
      car->setCenterPos(_pos);
      car->setLinearVelocity(_velo);
      // predict current position
      mPhysicsWorldOld->Step(Settings::the()->getLatencyServerToClient(), 4, 8);
      // update current state
      _ posDiff = car->getCenterPos() - carNew->getCenterPos();
      // server position differs largely from ours
      bool highOffset = (posDiff.lengthSquared() >= maxOffsetSquared / 2);
      // do not interpolate if offset is too high
      if (Settings::the()->getClientSideInterpolation() && !highOffset)
      { // interpolate
         // we use a exponential function for position correction
         // why: small errors are tolerable, but big errors not
         _ signedPow = [](float x) -> float
         {
            bool minus = (x < 0);
            x = qAbs(x);
            x = x * x; // x^2
            x *= (minus ? -1.f : 1.f);
            return x;
         };
         _ squaredPosDiff = QVector2D(signedPow(posDiff.x()), signedPow(posDiff.y()));

         _ velo = squaredPosDiff * Settings::the()->getClientSideInterpolationFactor();
         // the current velocity already contains user input
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
   // interpolate coins and mud?
   if (isClient())
   {
      if (Settings::the()->getClientSideInterpolation())
      {
         for (_ const &c : mCoins)
         {
            c->setLinearVelocity(QVector2D(-7.9f, 0));
         }
         for (_ const &m : mMuds)
         {
            m->setLinearVelocity(QVector2D(-7.9f, 0));
         }
      }
      else
      {
         for (_ const &c : mCoins)
         {
            c->setLinearVelocity(QVector2D(0, 0));
         }
         for (_ const &m : mMuds)
         {
            m->setLinearVelocity(QVector2D(0, 0));
         }
      }
   }

   // find coins and mud puddles that left the play-field
   {
      for (_ const &c : mCoins)
      {
         if (c->getCenterPos().x() < 0)
            mCoinsToRemove.push_back(c.get());
      }
      for (_ const &m : mMuds)
      {
         if (m->getCenterPos().x() < 0)
            mMudsToRemove.push_back(m.get());
      }
   }

   // Remove coins
   for (Coin *coin : mCoinsToRemove)
   {
      mCoins.erase(std::remove_if(mCoins.begin(), mCoins.end(),
                                  [coin](const UniqueCoin &_curr)
                                  {
                                     if (_curr.get() == coin)
                                        return true;
                                     return false;
                                  }),
                   mCoins.end());
   }
   mCoinsToRemove.clear();
   // Remove mud puddles
   for (Mud *mud : mMudsToRemove)
   {
      mMuds.erase(std::remove_if(mMuds.begin(), mMuds.end(),
                                 [mud](const UniqueMud &_curr)
                                 {
                                    if (_curr.get() == mud)
                                       return true;
                                    return false;
                                 }),
                  mMuds.end());
   }
   mMudsToRemove.clear();

   // spawn stuff
   {
      // mCoins will be empty for a while when we use delayed spawning. use these "mutices" to avoid mass-spawning.
      static _ coinSpawnInProgress = false;
      static _ mudSpawnInProgress = false;
      // spawn coin after 1s
      if (mCoins.empty() && !coinSpawnInProgress)
      {
         coinSpawnInProgress = true;
         mDelayedSpawner.pushDelayedAction(
             [this]()
             {
                spawnCoin();
                coinSpawnInProgress = false;
             },
             1);
      }
      // spawn mud after 500ms
      if (mMuds.empty() && !mudSpawnInProgress)
      {
         mudSpawnInProgress = true;
         mDelayedSpawner.pushDelayedAction(
             [this]()
             {
                spawnMud();
                mudSpawnInProgress = false;
             },
             0.5f);
      }
   }
   // 5: Update delayed stuff
   mDelayedLagDisabling.update();
   mDelayedServerCarPosUpdate.update();
   mDelayedSpawner.update();
   // 0: server-side lag compensation: step old physics world and update present
   if (isServer() && Settings::the()->getServerSideLagCompensation())
   {
      // server-side lag compensation for player 1
      // step old physics world to present
      // it would be more accurate to re-play AI's input. however, this is not so easy.
      mPhysicsWorldOld->Step(Settings::the()->getLatencyClientToServer() * 0.5f - _timestep, 4, 8);
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
         mDelayedLagDisabling.pushDelayedAction(func, Settings::the()->getLatencyClientToServer());
      }
#endif
   }
   // 2: step simulation
   {
      if (Settings::the()->getClientSidePhysics() || isServer())
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
      mDelayedLagDisabling.pushDelayedAction(func, Settings::the()->getLatencyClientToServer());
   }
   // reset AI input
   mAIInput->reset();
}

void GameLogic::spawnCoin()
{
   // only server must create new coins
   if (!isServer())
      return;
   // random y-pos, but on the right
   float posX = sCoinSize / 2.f + ((int)(sGameWidth - sCoinSize));
   float posY = -1;
   // Get a random y-value. However, do not place coins to close to mud puddles
   _ rndY = [this]()
   {
      return sCoinSize + (rand() % (int)(sGameHeight - sCoinSize));
   };
   if (mMuds.empty())
   {
      posY = rndY();
   }
   else
   {
      QVector2D mudPos = mMuds[0]->getCenterPos();
      float distanceToMud = 0;
      while (distanceToMud <= sMudSize + sCoinSize)
      {
         posY = rndY();
         distanceToMud = std::abs(mudPos.distanceToPoint(QVector2D(posX, posY)));
      }
   }
   _ coin = UniqueCoin(new Coin(mPhysicsWorld, sCoinSize, sCoinSize, posX, posY));
   // let coins move to the left over time
   coin->setLinearVelocity(QVector2D(-7.8f, 0));
   mCoins.push_back(std::move(coin));
   // tell AI
   if (mCoinSpawnCallback)
      mCoinSpawnCallback(QVector2D(posX, posY));
}

void GameLogic::spawnMud()
{
   // only server must create new mud puddles
   if (!isServer())
      return;
   // random y-pos, but on the right
   float posX = sMudSize / 2.f + ((int)(sGameWidth - sMudSize));
   float posY = -1;
   // Get a random y-value. However, do not place mud puddles to close to coins
   _ rndY = [this]()
   {
      return sMudSize + (rand() % (int)(sGameHeight - sMudSize));
   };
   if (mCoins.empty())
   {
      posY = rndY();
   }
   else
   {
      QVector2D coinPos = mCoins[0]->getCenterPos();
      float distanceToCoin = 0;
      while (distanceToCoin <= sMudSize + sCoinSize)
      {
         posY = rndY();
         distanceToCoin = std::abs(coinPos.distanceToPoint(QVector2D(posX, posY)));
      }
   }
   _ mud = UniqueMud(new Mud(mPhysicsWorld, sMudSize, sMudSize, posX, posY));
   // let mud puddles move to the left over time
   mud->setLinearVelocity(QVector2D(-7.8f, 0));
   mMuds.push_back(std::move(mud));
   // tell AI
   if (mMudSpawnCallback)
      mMudSpawnCallback(QVector2D(posX, posY));
}

void GameLogic::callbackCarMud(Car *_car, Mud *_mud)
{
   // get owner of car
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
   // trigger artificial lag
   {
      bool triggered = criticalSituationOccured();
      if (isServer())
         StatisticsEngine::the()->tellCollision(PhysicsObject::Type::CAR, PhysicsObject::Type::MUD,
                                                Settings::the()->getLagEnabled(), triggered, player);
   }

   // mud logic
   {
      if (!Settings::the()->getClientSidePhysics() && !isServer())
         return; // only server should handle this
      // TODO: switch variable for handling this client or/and serverside

      mMudsToRemove.push_back(_mud);
      if (mMudCollectedCallback)
         mMudCollectedCallback(_mud->getCenterPos());

      // hit mud puddle: minus 2 points
      mScore[(int)player] -= 2;
      _ eventType = (player == PlayerID::P1) ? StatisticsEngine::EventType::P1MUD : StatisticsEngine::EventType::P2MUD;
      StatisticsEngine::the()->tellEvent(eventType);
   }
}

void GameLogic::callbackCarCoin(Car *_car, Coin *_coin)
{
   // get owner of car
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
   // trigger artificial lag
   {
      bool triggered = criticalSituationOccured();
      if (isServer())
         StatisticsEngine::the()->tellCollision(PhysicsObject::Type::CAR, PhysicsObject::Type::COIN,
                                                Settings::the()->getLagEnabled(), triggered, player);
   }
   // coin logic
   {
      if (!Settings::the()->getClientSidePhysics() && !isServer())
         return; // only server should handle this
      // TODO: switch variable for handling this client or/and serverside

      mCoinsToRemove.push_back(_coin);
      if (mCoinCollectedCallback)
         mCoinCollectedCallback(_coin->getCenterPos());

      // collected coin: 1 point
      ++mScore[(int)player];
      _ eventType = (player == PlayerID::P1) ? StatisticsEngine::EventType::P1COIN : StatisticsEngine::EventType::P2COIN;
      StatisticsEngine::the()->tellEvent(eventType);
   }
}

void GameLogic::callbackCarCar(Car *_carA, Car *_carB)
{
   (void)&_carA;
   (void)&_carB;
   bool triggered = criticalSituationOccured();
   if (isServer())
      StatisticsEngine::the()->tellCollision(PhysicsObject::Type::CAR, PhysicsObject::Type::CAR,
                                             Settings::the()->getLagEnabled(), triggered);
}

void GameLogic::callbackCarBoundary(Car *_car, Boundary *_boundary)
{
   (void)&_car;
   (void)&_boundary;
   bool triggered = criticalSituationOccured();
   if (isServer())
      StatisticsEngine::the()->tellCollision(PhysicsObject::Type::CAR, PhysicsObject::Type::BOUNDARY,
                                             Settings::the()->getLagEnabled(), triggered);
}

bool GameLogic::criticalSituationOccured()
{
   _ probability = Settings::the()->getLagProbability();
   float random = rand() % 100 + 1;   // 100 values: [1,100]
   if (random <= 100.f * probability) // probability: [0,1] * 100 -> [0,100]
   {
      // activate lag for a period of time
      Settings::the()->setLagEnabled(true);

      mDelayedLagDisabling.clear(); // ensure there is only one delayed task
      _ func = [this]()
      {
         Settings::the()->setLagEnabled(false);
      };
      mDelayedLagDisabling.pushDelayedAction(func, Settings::the()->getLagDuration());
      return true;
   }
   return false;
}

void GameLogic::AIInput::reset()
{
   deltaX[0] = 0;
   deltaX[1] = 0;
   deltaY[0] = 0;
   deltaY[1] = 0;
}
