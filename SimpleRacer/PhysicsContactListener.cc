#include "PhysicsContactListener.hh"
#include <Box2D/Box2D.h>
#include "PhysicsObject.hh"
#include "Common.hh"
#include "Car.hh"
#include "Coin.hh"

using namespace simpleRacer;

PhysicsContactListener::PhysicsContactListener()
{
}

void PhysicsContactListener::BeginContact(b2Contact *_contact)
{
   if (!mCallback)
      return;

   b2Fixture *aFixtures = _contact->GetFixtureA()->GetBody()->GetFixtureList();
   b2Fixture *bFixtures = _contact->GetFixtureB()->GetBody()->GetFixtureList();
   const b2Filter &aFilter = aFixtures->GetFilterData();
   const b2Filter &bFilter = bFixtures->GetFilterData();
   bool aCarBCoin = (aFilter.categoryBits == (uint16)PhysicsObject::Type::CAR
                     && bFilter.categoryBits == (uint16)PhysicsObject::Type::COIN);
   bool bCaraCoin = (aFilter.categoryBits == (uint16)PhysicsObject::Type::COIN
                     && bFilter.categoryBits == (uint16)PhysicsObject::Type::CAR);
   if (!aCarBCoin && !bCaraCoin)
      return;

   PhysicsObject* aClass = (PhysicsObject*)_contact->GetFixtureA()->GetBody()->GetUserData();
   PhysicsObject* bClass = (PhysicsObject*)_contact->GetFixtureB()->GetBody()->GetUserData();

   Car *car = (aCarBCoin ? dynamic_cast<Car*>(aClass) : dynamic_cast<Car*>(bClass));
   Coin *coin = (aCarBCoin ? dynamic_cast<Coin*>(aClass) : dynamic_cast<Coin*>(bClass));
   SR_ASSERT(car && "Car is null.");
   SR_ASSERT(coin && "Coin is null.");

   mCallback(car, coin);
}
