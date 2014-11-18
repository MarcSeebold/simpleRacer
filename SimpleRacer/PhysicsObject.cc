#include "PhysicsObject.hh"

using namespace simpleRacer;

PhysicsObject::PhysicsObject(Sharedb2World _world, int _width, int _height, int _x, int _y, float _linearDamping, bool _static)
{
   mWorld = _world;
   SR_ASSERT(mWorld && "World is null.");
   // create body
   if (_static)
      mDef.type = b2_staticBody;
   else
      mDef.type = b2_dynamicBody;
   mDef.position.Set(_x, _y);
   mDef.linearDamping = _linearDamping;
   mBody = mWorld->CreateBody(&mDef);
   // create shape
   mShape.SetAsBox(_width/2.f, _height/2.f);
   if (_static)
   {
      // static body fixture
      mBody->CreateFixture(&mShape, 0);
   }
   else
   {
      // Dynamic body fixture
      mFixDef.shape = &mShape;
      mFixDef.density = 1.0f;
      mFixDef.friction = 0.3f;
      mFixDef.restitution = 0.3f;
      mBody->CreateFixture(&mFixDef);
   }
   mBody->SetUserData(this);
}

PhysicsObject::~PhysicsObject()
{
   mWorld->DestroyBody(mBody);
   mBody = nullptr;
   mWorld = nullptr;
}

QVector2D PhysicsObject::getCenterPos()
{
   _ center = mBody->GetWorldCenter();
   return QVector2D(center.x, center.y);
}

void PhysicsObject::applyForce(const QVector2D &_vec)
{
   mBody->ApplyLinearImpulse(b2Vec2(_vec.x(), _vec.y()), mBody->GetWorldCenter(), true);
}
