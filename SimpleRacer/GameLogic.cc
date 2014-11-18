#include "GameLogic.hh"
#include <algorithm>
#include <QRectF>
#include <iostream>
#include <Box2D/Box2D.h>
#include "PhysicsObject.hh"

using namespace simpleRacer;

GameLogic::GameLogic() : mPhysicsWorld(new b2World(b2Vec2(0, 0))) // no gravity
// mStreetboundary(new PhysicsObject(mPhysicsWorld, 100, 100, 0, 0, true))

{
   // create street boundaries
   // bottom
   mStreetBoundaries[0] = UniquePhysicsObject(new PhysicsObject(mPhysicsWorld, 100, 20, 50, -10, 0, true));
   // top
   mStreetBoundaries[1] = UniquePhysicsObject(new PhysicsObject(mPhysicsWorld, 100, 20, 50, 110, 0, true));
   // left
   mStreetBoundaries[2] = UniquePhysicsObject(new PhysicsObject(mPhysicsWorld, 20, 100, -10, 50, 0, true));
   // right
   mStreetBoundaries[3] = UniquePhysicsObject(new PhysicsObject(mPhysicsWorld, 20, 100, 110, 50, 0, true));
   // reset state
   reset();
}

GameLogic::~GameLogic()
{
}

void GameLogic::reset()
{
   _ linearDamping = 0.75f;
   // spawn car1 on top left
   mCar1 = UniquePhysicsObject(new PhysicsObject(mPhysicsWorld, sCarLength, sCarWidth, sCarWidth / 2, sCarLength / 2, linearDamping));
   // spawn car2 on bottom left
   mCar2 = UniquePhysicsObject(
       new PhysicsObject(mPhysicsWorld, sCarLength, sCarWidth, sCarWidth / 2, 100.f - sCarLength / 2, linearDamping));

   mUserInput = UniqueUserInput(new UserInput);

   for (int i : {0, 1})
   {
      mPlayerCoins[i] = 0;
   }
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


   // 1: apply input
   mCar1->applyForce(QVector2D(mUserInput->deltaX[0] * 1000, 0));
   mCar1->applyForce(QVector2D(0, mUserInput->deltaY[0] * 1000));
   mCar2->applyForce(QVector2D(mUserInput->deltaX[1] * 1000, 0));
   mCar2->applyForce(QVector2D(0, mUserInput->deltaY[1] * 1000));

   // 2: step simulation
   {
      int32 velocityIterations = 6;
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


void GameLogic::UserInput::reset()
{
   deltaX[0] = 0;
   deltaX[1] = 0;
   deltaY[0] = 0;
   deltaY[1] = 0;
}
