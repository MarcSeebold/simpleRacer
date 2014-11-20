#pragma once
#include <Box2D/Dynamics/b2WorldCallbacks.h>

namespace simpleRacer
{
class Coin;
class Car;

class PhysicsContactListener : public b2ContactListener
{
public:
   typedef void(CallbackFunc)(Car*, Coin*);

public:
   PhysicsContactListener();
   void BeginContact(b2Contact *_contact) override;
   void registerCallback(CallbackFunc *_func) { mCallback = _func; }

private:
   CallbackFunc *mCallback = nullptr;
};
}
