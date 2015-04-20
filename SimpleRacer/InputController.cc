#include "InputController.hh"
#include "GameLogic.hh"
#include "Common.hh"
#include <QKeyEvent>
#include <QTimer>

InputController::InputController(SharedGameLogic _client)
: mGameLogicClient(std::forward<SharedGameLogic>(_client))
{
}

InputController::~InputController()
{
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
#if 0
   if (mKeyStatus.down)
   {
      mGameLogicClient->steerDown(PlayerID::P1);
   }
   if (mKeyStatus.left)
   {
      mGameLogicClient->decelerate(PlayerID::P1);
   }
   if (mKeyStatus.right)
   {
      mGameLogicClient->accelerate(PlayerID::P1);
   }
   if (mKeyStatus.up)
   {
      mGameLogicClient->steerUp(PlayerID::P1);
   }
#endif
}
