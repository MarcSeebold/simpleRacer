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
   PhysicsObject(Sharedb2World _world, int _width, int _height, int _x, int _y, float _linearDamping = 0.f, bool _static = false);
   ~PhysicsObject();

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
