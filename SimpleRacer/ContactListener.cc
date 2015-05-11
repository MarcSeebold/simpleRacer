#include "ContactListener.hh"
#include <Box2D/Box2D.h>
#include "PhysicsObject.hh"
#include "Common.hh"

void ContactListener::BeginContact(b2Contact *_contact)
{
   PhysicsObject *aClass = (PhysicsObject *)_contact->GetFixtureA()->GetBody()->GetUserData();
   PhysicsObject *bClass = (PhysicsObject *)_contact->GetFixtureB()->GetBody()->GetUserData();
   if (aClass == nullptr || bClass == nullptr)
   {
      SR_ASSERT(0 && "Unknown physic object");
      return; // unknown object
   }
   _ carA = dynamic_cast<Car *>(aClass);
   _ carB = dynamic_cast<Car *>(bClass);
   // 1: Get a car
   Car *car = nullptr;
   PhysicsObject *otherObject = nullptr;
   if (carA != nullptr)
   {
      car = carA;
      otherObject = bClass;
   }
   else if (carB != nullptr)
   {
      car = carB;
      otherObject = aClass;
   }
   else
      return; // we only handle car/? collisions
   // 2: Get type of the other object and call callback
   switch (otherObject->getType())
   {
   case PhysicsObject::Type::BOUNDARY:
   {
      if (!mCallbackCarBoundary)
         return;
      _ boundary = dynamic_cast<Boundary *>(otherObject);
      SR_ASSERT(boundary && "expected a boundary obj");
      mCallbackCarBoundary(car, boundary);
   }
   break;
   case PhysicsObject::Type::CAR:
   {
      if (!mCallbackCarCar)
         return;
      _ otherCar = dynamic_cast<Car *>(otherObject);
      SR_ASSERT(otherCar && "expected a car obj");
      mCallbackCarCar(car, otherCar);
   }
   break;
   case PhysicsObject::Type::COIN:
   {
      if (!mCallbackCarCoin)
         return;
      _ coin = dynamic_cast<Coin *>(otherObject);
      SR_ASSERT(coin && "expected a coin obj");
      mCallbackCarCoin(car, coin);
   }
   break;
   case PhysicsObject::Type::OTHER: // fallthrough
   default:
      // Ignore unknown objects
      break;
   }
}
