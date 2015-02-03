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
   PhysicsObject(const Sharedb2World &_world,
                 float _width,
                 float _height,
                 float _x,
                 float _y,
                 Type _type = Type::OTHER,
                 float _linearDamping = 0.f,
                 bool _static = false);
   virtual ~PhysicsObject();

   QVector2D getCenterPos();
   QVector2D getLinearVelocity();
   void applyForce(const QVector2D &_vec);
   void disableCollisions();

protected:
   b2Body *mBody = nullptr;
   b2PolygonShape mShape;
   b2BodyDef mDef;
   b2FixtureDef mFixDef;
   Sharedb2World mWorld;
   Type mType;
};

class Coin : public PhysicsObject
{
public:
   Coin(const Sharedb2World &_world, float _width, float _height, float _x, float _y, float _linearDamping = 0.f);
};

class Car : public PhysicsObject
{
public:
   Car(const Sharedb2World &_world, float _width, float _height, float _x, float _y, float _linearDamping = 0.f);
};
} // namespace simpleRacer
