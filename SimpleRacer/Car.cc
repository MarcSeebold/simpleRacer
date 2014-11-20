#include "Car.hh"

using namespace simpleRacer;

Car::Car(const Sharedb2World &_world, float _width, float _height, float _x, float _y, float _linearDamping):
    PhysicsObject(_world, _width, _height, _x, _y, Type::CAR, _linearDamping, false)
{
    mFixDef.filter.maskBits ^= (uint16)Type::COIN;
}
