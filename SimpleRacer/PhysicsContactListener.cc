#include "PhysicsContactListener.hh"
#include <Box2D/Box2D.h>
#include "PhysicsObject.hh"
#include "Common.hh"

PhysicsContactListener::PhysicsContactListener()
{
}

void PhysicsContactListener::BeginContact(b2Contact *_contact)
{
   if (!mCallback)
      return;

   PhysicsObject *aClass = (PhysicsObject *)_contact->GetFixtureA()->GetBody()->GetUserData();
   PhysicsObject *bClass = (PhysicsObject *)_contact->GetFixtureB()->GetBody()->GetUserData();
   _ aCar = dynamic_cast<Car *>(aClass);
   _ aCoin = dynamic_cast<Coin *>(aClass);

   Car *car = (aCar ? aCar : dynamic_cast<Car *>(bClass));
   Coin *coin = (aCoin ? aCoin : dynamic_cast<Coin *>(bClass));

   if (car && coin)
   {
      // coin->
      mCallback(car, coin);
   }
}
