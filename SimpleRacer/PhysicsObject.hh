#pragma once
#include <QVector2D>
#include <Box2D/Box2D.h>
#include "Common.hh"

SHARED(class, b2World);
namespace simpleRacer
{
class PhysicsObject
{
public:
    enum class Type : char
    {
        COIN,
        CAR,
        BOUNDARY,
        OTHER
    };

public:
   PhysicsObject(const Sharedb2World &_world, float _width, float _height, float _x, float _y, Type _type = Type::OTHER, float _linearDamping = 0.f, bool _static = false);
   virtual ~PhysicsObject();

   QVector2D getCenterPos();
   void applyForce(const QVector2D &_vec);

protected:
   b2Body *mBody = nullptr;
   b2PolygonShape mShape;
   b2BodyDef mDef;
   b2FixtureDef mFixDef;
   Sharedb2World mWorld;
};
} // namespace simpleRacer
