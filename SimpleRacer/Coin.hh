#pragma once
#include "PhysicsObject.hh"

namespace simpleRacer
{
class Coin : public PhysicsObject
{
public:
   Coin(const Sharedb2World &_world, float _width, float _height, float _x, float _y, float _linearDamping = 0.f);
};
}
