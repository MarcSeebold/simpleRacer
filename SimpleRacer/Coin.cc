#include "Coin.hh"

using namespace simpleRacer;

Coin::Coin(const Sharedb2World &_world, float _width, float _height, float _x, float _y, float _linearDamping):
    PhysicsObject(_world, _width, _height, _x, _y, Type::COIN, _linearDamping, true)
{
    mFixDef.filter.maskBits ^= (uint16)Type::CAR;
}
