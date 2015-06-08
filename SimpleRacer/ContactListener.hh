#pragma once
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <functional>

class Coin;
class Car;
class Boundary;
class Mud;

typedef std::function<void(Car *, Mud *)> CallbackFuncCarMud;
typedef std::function<void(Car *, Coin *)> CallbackFuncCarCoin;
typedef std::function<void(Car *, Car *)> CallbackFuncCarCar;
typedef std::function<void(Car *, Boundary *)> CallbackFuncCarBoundary;

class ContactListener : public b2ContactListener
{
public:
   // C-style function naming avoiding ambious calls using lambdas
   void registerCallbackCarMud(CallbackFuncCarMud _func) { mCallbackCarMud = _func; }
   void registerCallbackCarCoin(CallbackFuncCarCoin _func) { mCallbackCarCoin = _func; }
   void registerCallbackCarCar(CallbackFuncCarCar _func) { mCallbackCarCar = _func; }
   void registerCallbackCarBoundary(CallbackFuncCarBoundary _func) { mCallbackCarBoundary = _func; }

private:
   void BeginContact(b2Contact *_contact) override;
   CallbackFuncCarMud mCallbackCarMud;
   CallbackFuncCarCoin mCallbackCarCoin;
   CallbackFuncCarCar mCallbackCarCar;
   CallbackFuncCarBoundary mCallbackCarBoundary;
};
