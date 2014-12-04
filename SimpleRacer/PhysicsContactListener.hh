#pragma once
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <functional>

namespace simpleRacer
{
class Coin;
class Car;

class PhysicsContactListener : public b2ContactListener
{
public:
   typedef std::function<void(Car*, Coin*)> CallbackFunc;

public:
   PhysicsContactListener();
   void BeginContact(b2Contact *_contact) override;
   void registerCallback(CallbackFunc _func) { mCallback = _func; }

private:
   CallbackFunc mCallback;
};
}
