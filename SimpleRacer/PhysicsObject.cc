#include "PhysicsObject.hh"

#include <qdebug.h>

PhysicsObject::PhysicsObject(
    const Sharedb2World &_world, float _width, float _height, float _x, float _y, Type _type, float _linearDamping, bool _static, Shape _shape)
  : mWorld(_world), mType(_type)
{
   SR_ASSERT(mWorld && "World is null.");
   // create body
   if (_static)
      mDef.type = b2_staticBody;
   else
      mDef.type = b2_dynamicBody;
   mDef.position.Set(_x, _y);
   mDef.fixedRotation = true;

   if (!_static)
   {
      mDef.linearDamping = _linearDamping;
   }
   mBody = mWorld->CreateBody(&mDef);
   // create shape
   switch (_shape)
   {
   case Shape::BOX:
      mPolyShape.SetAsBox(_width / 2.f, _height / 2.f);
      mFixDef.shape = &mPolyShape;
      break;
   case Shape::CIRCLE:
      mCircleShape.m_p.Set(0, 0);
      mCircleShape.m_radius = _width / 2.f;
      mFixDef.shape = &mCircleShape;
      break;
   default:
      SR_ASSERT(0 && "Unhandled case.");
      // fallback to box
      mPolyShape.SetAsBox(_width / 2.f, _height / 2.f);
      mFixDef.shape = &mPolyShape;
      break;
   }

   mFixDef.density = 1.0f;
   mFixDef.friction = 0.3f;
   mFixDef.restitution = 0.3f;
   mBody->CreateFixture(&mFixDef);

   mBody->SetUserData(this);
}

PhysicsObject::~PhysicsObject()
{
   mWorld->DestroyBody(mBody);
   mBody = nullptr;
   mWorld = nullptr;
}

QVector2D PhysicsObject::getCenterPos() const
{
   _ center = mBody->GetWorldCenter();
   return QVector2D(center.x, center.y);
}

void PhysicsObject::setCenterPos(const QVector2D &_pos)
{
   mBody->SetTransform(b2Vec2(_pos.x(), _pos.y()), 0);
}

void PhysicsObject::setLinearVelocity(const QVector2D &_velocity)
{
   mBody->SetLinearVelocity(b2Vec2(_velocity.x(), _velocity.y()));
}

QVector2D PhysicsObject::getLinearVelocity() const
{
   _ velo = mBody->GetLinearVelocity();
   return QVector2D(velo.x, velo.y);
}

void PhysicsObject::applyForce(const QVector2D &_vec)
{
   mBody->ApplyLinearImpulse(b2Vec2(_vec.x(), _vec.y()), mBody->GetWorldCenter(), true);
}

void PhysicsObject::disableCollisions()
{
   mBody->GetFixtureList()->SetSensor(true);
}

Coin::Coin(const Sharedb2World &_world, float _width, float _height, float _x, float _y)
  : PhysicsObject(_world, _width, _height, _x, _y, Type::COIN, 0, false, Shape::CIRCLE)
{
   disableCollisions();
}

Car::Car(const Sharedb2World &_world, float _width, float _height, float _x, float _y, float _linearDamping)
  : PhysicsObject(_world, _width, _height, _x, _y, Type::CAR, _linearDamping, false)
{
}

Boundary::Boundary(const Sharedb2World &_world, float _width, float _height, float _x, float _y)
  : PhysicsObject(_world, _width, _height, _x, _y, Type::BOUNDARY, 0, true)
{
}


Mud::Mud(const Sharedb2World &_world, float _width, float _height, float _x, float _y)
  : PhysicsObject(_world, _width, _height, _x, _y, Type::MUD, 0, false, Shape::CIRCLE)
{
   disableCollisions();
}
