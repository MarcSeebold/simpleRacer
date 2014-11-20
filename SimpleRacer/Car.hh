#pragma once
#include "PhysicsObject.hh"

namespace simpleRacer
{
class Car : public PhysicsObject
{
public:
   Car(const Sharedb2World &_world, float _width, float _height, float _x, float _y, float _linearDamping = 0.f);
};
}
