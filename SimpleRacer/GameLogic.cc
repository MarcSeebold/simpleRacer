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
#include "DelaySimulator.hh"

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
   // "old" world for lag compensation
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
   // free up resources
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
   // remove coins and mud
   {
      mCoins.clear();
      mMuds.clear();
   }
   _ linearDamping = Settings::the()->getLinearDamping();
   // (re-)create cars
   {
      // spawn car1 on top left
      mCar1 = UniqueCar(new Car(mPhysicsWorld, sCarWidth, sCarHeight, sCarWidth / 2, sCarHeight / 2, linearDamping));
      // spawn car2 on bottom left
      mCar2 = UniqueCar(new Car(mPhysicsWorld, sCarWidth, sCarHeight, sCarWidth / 2, sGameHeight - sCarHeight / 2, linearDamping));
   }
   // cars for the "past" world
   {
      // spawn car1 on top left
      mCar1Old = UniqueCar(new Car(mPhysicsWorldOld, sCarWidth, sCarHeight, sCarWidth / 2, sCarHeight / 2, linearDamping));
      // spawn car2 on bottom left
      mCar2Old = UniqueCar(new Car(mPhysicsWorldOld, sCarWidth, sCarHeight, sCarWidth / 2, sGameHeight - sCarHeight / 2, linearDamping));
   }

   // reset other stuff
   {
      mAIInput = UniqueAIInput(new AIInput);
      mAIInput->reset();

      for (int i : {0, 1})
         mScore[i] = 0;
   }
}

void GameLogic::setAICheating(bool _val)
{
   mAICheatingEnabled = _val;
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
   if (Settings::the()->getHandleCoinMudCollisionsOnlyClientside() && Settings::the()->getClientSideInterpolation())
      return; // clientside only: only process spawn commands

   mCoins.clear();
   for (_ const &c : _coins)
   {
      _ coin = UniqueCoin(new Coin(mPhysicsWorld, sCoinSize, sCoinSize, c.x(), c.y()));
      mCoins.push_back(std::move(coin));
   }
}

void GameLogic::setMuds(const std::vector<QVector2D> &_muds)
{
   if (Settings::the()->getHandleCoinMudCollisionsOnlyClientside() && Settings::the()->getClientSideInterpolation())
      return; // clientside only: only process spawn commands

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

   _ maxOffsetSquared = Settings::the()->getMaxOffsetSquared();
   const UniqueCar &carNew = (_player == PlayerID::P1) ? mCar1 : mCar2;

   if (Settings::the()->getClientSidePrediction())
   { // client side prediction
      const UniqueCar &car = (_player == PlayerID::P1) ? mCar1Old : mCar2Old;
      // update past state
      car->setCenterPos(_pos);
      car->setLinearVelocity(_velo);
      // predict current position
      mPhysicsWorldOld->Step(Settings::the()->getCurrentLatencyServerToClient(), 4, 8);
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
         // std::cerr << "hard-set" << std::endl;
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
   // 1: interpolate coins and mud?
   if (isClient())
   {
      _ coinMudLinearVelocity = Settings::the()->getCoinMudLinearVelocity();
      if (Settings::the()->getClientSideInterpolation())
      {
         for (_ const &c : mCoins)
         {
            c->setLinearVelocity(QVector2D(coinMudLinearVelocity, 0));
         }
         for (_ const &m : mMuds)
         {
            m->setLinearVelocity(QVector2D(coinMudLinearVelocity, 0));
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

   // 2: find coins and mud puddles that left the play-field
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

   // 3: Remove coins
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
   // 4: Remove mud puddles
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

   // 5: spawn stuff
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
             Settings::the()->getCoinSpawnTime());
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
             Settings::the()->getMudSpawnTime());
      }
   }
   // 6: Update delayed stuff
   mDelayedLagDisabling.update();
   mDelayedServerCarPosUpdate.update();
   mDelayedSpawner.update();
   // 7: server-side lag compensation: step old physics world and update present
   if (isServer() && Settings::the()->getServerSideLagCompensation())
   {
      // server-side lag compensation for player 1
      // step old physics world to present
      // it would be more accurate to re-play AI's input. however, this is not so easy.
      mPhysicsWorldOld->Step(Settings::the()->getCurrentLatencyClientToServer() * 0.5f - _timestep, 4, 8);
      // store new data (only for P1!, P2 lives on the server)
      mCar1->setCenterPos(mCar1Old->getCenterPos());
      mCar1->setLinearVelocity(mCar1Old->getLinearVelocity());
   }
   // 8: apply input
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
      _ factorX = Settings::the()->getCarAccX();
      _ factorY = Settings::the()->getCarAccY();
      // player
      int dirX = (mKeyStatus.right ? 1 : 0) + (mKeyStatus.left ? -1 : 0);
      int dirY = (mKeyStatus.up ? 1 : 0) + (mKeyStatus.down ? -1 : 0);
      mCar1->applyForce(QVector2D(dirX * factorX, 0));
      mCar1->applyForce(QVector2D(0, dirY * factorY));
      // AI
      float cheatingFactor = (mAICheatingEnabled ? 2.f : 1.f);
      mCar2->applyForce(QVector2D(mAIInput->deltaX[1] * factorX * cheatingFactor, 0));
      mCar2->applyForce(QVector2D(0, mAIInput->deltaY[1] * factorY * cheatingFactor));
   }
   // 9: Limit velocity of cars
   if (isServer() || (isClient() && !Settings::the()->getClientSideInterpolation()))
   { // disable when we are client and interpolation is active (we interpolate using velocity...)
      const _ x = Settings::the()->getCarVeloX();
      const _ y = Settings::the()->getCarVeloY();
      const _ correctVelo = [&x, &y](Car *car)
      {
         const _ cv = car->getLinearVelocity();
         QVector2D newVelo = cv;
         if (std::abs(cv.x()) > x)
            newVelo.setX(x * (cv.x() < 0? -1 : 1));
         if (std::abs(cv.y()) > y)
            newVelo.setY(y * (cv.y() < 0? -1 : 1));
         car->setLinearVelocity(newVelo);
      };
      correctVelo(mCar1.get());
      correctVelo(mCar2.get());
   }
   // 10: step simulation
   {
      if (Settings::the()->getClientSidePhysics() || isServer())
      {
         const _ velocityIterations = 4;
         const _ positionIterations = 8;
         mPhysicsWorld->Step(_timestep, velocityIterations, positionIterations);
      }
   }
   // 11: Keep a copy of the world state for X seconds (for server-side lag compensation)
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
      mDelayedLagDisabling.pushDelayedAction(func, Settings::the()->getCurrentLatencyClientToServer());
   }
   // reset AI input
   mAIInput->reset();
}

void GameLogic::clientSpawnCoin(const QVector2D &_pos)
{
   _ coin = UniqueCoin(new Coin(mPhysicsWorld, sCoinSize, sCoinSize, _pos.x(), _pos.y()));
   mCoins.push_back(std::move(coin));
}

void GameLogic::clientSpawnMud(const QVector2D &_pos)
{
   _ mud = UniqueMud(new Mud(mPhysicsWorld, sMudSize, sMudSize, _pos.x(), _pos.y()));
   mMuds.push_back(std::move(mud));
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
   coin->setLinearVelocity(QVector2D(Settings::the()->getCoinMudLinearVelocity(), 0));
   mCoins.push_back(std::move(coin));
   // tell AI
   if (mCoinSpawnCallback)
      mCoinSpawnCallback(QVector2D(posX, posY));
   // send packet to client
   SimpleRacer::delaySimulator()->scSpawnCoin(QVector2D(posX, posY));
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
   mud->setLinearVelocity(QVector2D(Settings::the()->getCoinMudLinearVelocity(), 0));
   mMuds.push_back(std::move(mud));
   // tell AI
   if (mMudSpawnCallback)
      mMudSpawnCallback(QVector2D(posX, posY));
   // send packet to client
   SimpleRacer::delaySimulator()->scSpawnMud(QVector2D(posX, posY));
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
      mScore[(int)player] += Settings::the()->getScoreMud();
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
      mScore[(int)player] += Settings::the()->getScoreCoin();
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
