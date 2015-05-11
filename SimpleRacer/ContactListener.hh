#pragma once
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <functional>

class Coin;
class Car;
class Boundary;

template <class A, class B>
class ContactListener : public b2ContactListener
{
public:
    typedef std::function<void(A*, B*)> CallbackFunc;
    void registerCallback(CallbackFunc _func) { mCallback = _func; }
private:
    void BeginContact(b2Contact *_contact) override;
    CallbackFunc mCallback;
};

class ContactListenerCarCoin : public ContactListener<Car, Coin> {};
