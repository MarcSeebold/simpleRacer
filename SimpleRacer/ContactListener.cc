#include "ContactListener.hh"
#include <Box2D/Box2D.h>
#include "PhysicsObject.hh"
#include "Common.hh"

template <class A, class B>
void ContactListener<A,B>::BeginContact(b2Contact *_contact)
{
    if (!mCallback)
       return;

    PhysicsObject *aClass = (PhysicsObject *)_contact->GetFixtureA()->GetBody()->GetUserData();
    PhysicsObject *bClass = (PhysicsObject *)_contact->GetFixtureB()->GetBody()->GetUserData();
    _ obj1 = dynamic_cast<A *>(aClass);
    _ obj2 = dynamic_cast<B *>(aClass);

    A *objA = (obj1 ? obj1 : dynamic_cast<A *>(bClass));
    B *objb = (obj2 ? obj2 : dynamic_cast<B *>(bClass));

    if (objA && objb)
    {
       // coin->
       mCallback(objA, objb);
    }
}

// specializations
template void ContactListener<Car,Coin>::BeginContact(b2Contact *_contact);
template void ContactListener<Car,Car>::BeginContact(b2Contact *_contact);
template void ContactListener<Car,Boundary>::BeginContact(b2Contact *_contact);
