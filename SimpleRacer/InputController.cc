#include "InputController.hh"
#include "GameLogic.hh"
#include "Common.hh"
#include <QKeyEvent>
#include <QTimer>

InputController::InputController(SharedGameLogic _logic, SharedGameLogic _server)
  : mGameLogicClient(std::forward<SharedGameLogic>(_logic)), mGameLogicServer(std::forward<SharedGameLogic>(_server))
{
}

InputController::~InputController()
{
   for (_& a : mDelayedActions)
      delete a;
   mDelayedActions.clear();
}

void InputController::keyPressEvent(QKeyEvent* e)
{
   switch (e->key())
   {
   case Qt::Key_Up:
      mKeyStatus.up = true;
      break;
   case Qt::Key_Down:
      mKeyStatus.down = true;
      break;
   case Qt::Key_Left:
      mKeyStatus.left = true;
      break;
   case Qt::Key_Right:
      mKeyStatus.right = true;
      break;
   }
}

void InputController::keyReleaseEvent(QKeyEvent* e)
{
   switch (e->key())
   {
   case Qt::Key_Up:
      mKeyStatus.up = false;
      break;
   case Qt::Key_Down:
      mKeyStatus.down = false;
      break;
   case Qt::Key_Left:
      mKeyStatus.left = false;
      break;
   case Qt::Key_Right:
      mKeyStatus.right = false;
      break;
   }
}

void InputController::update()
{
   // clean up delayed actions
   for (_ it = mDelayedActions.begin(); it != mDelayedActions.end();)
   {
      if (!(*it)->isActive())
      {
         _ obj = *it;
         it = mDelayedActions.erase(it);
         delete obj;
      }
      else
      {
         ++it;
      }
   }
   // process keys
   _ makeDelayed = [this](KeyType _key)
   {
      _ delayed = new QTimer();
      mDelayedActions.push_back(delayed);
      delayed->setSingleShot(true);
      connect(delayed, &QTimer::timeout, [this, _key]()
              {
         switch (_key)
         {
         case KeyType::DOWN:
            mGameLogicServer->steerDown(PlayerID::P1);
            break;
         case KeyType::LEFT:
            mGameLogicServer->decelerate(PlayerID::P1);
            break;
         case KeyType::RIGHT:
            mGameLogicServer->accelerate(PlayerID::P1);
            break;
         case KeyType::UP:
            mGameLogicServer->steerUp(PlayerID::P1);
            break;
         }
      });
      delayed->start(mDelay);
   };


   if (mKeyStatus.down)
   {
      mGameLogicClient->steerDown(PlayerID::P1);
      makeDelayed(KeyType::DOWN);
   }
   if (mKeyStatus.left)
   {
      mGameLogicClient->decelerate(PlayerID::P1);
      makeDelayed(KeyType::LEFT);
   }
   if (mKeyStatus.right)
   {
      mGameLogicClient->accelerate(PlayerID::P1);
      makeDelayed(KeyType::RIGHT);
   }
   if (mKeyStatus.up)
   {
      mGameLogicClient->steerUp(PlayerID::P1);
      makeDelayed(KeyType::UP);
   }
}
