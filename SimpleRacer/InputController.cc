#include "InputController.hh"
#include "GameLogic.hh"
#include <cassert>

using namespace simpleRacer;

InputController::InputController(simpleRacer::SharedGameLogic _logic)
  : mGameLogic(std::forward<simpleRacer::SharedGameLogic>(_logic))
{
}

void InputController::sendKeyPress(InputController::KeyType _type)
{
   mKeyEvents.push_back(KeyEvent{common::getCurrentTimestamp(), _type});
}

void InputController::update()
{
   if (mKeyEvents.empty())
      return;
   _ timestamp = common::getCurrentTimestamp();
   int eventsToRemove = 0;
   for (_& event : mKeyEvents)
   {
      if (event.timestamp + mDelay > timestamp)
         break;
      switch (event.type)
      {
      case KeyType::DOWN:
         mGameLogic->steerDown(PlayerID::P1);
         break;
      case KeyType::LEFT:
         mGameLogic->decelerate(PlayerID::P1);
         break;
      case KeyType::RIGHT:
         mGameLogic->accelerate(PlayerID::P1);
         break;
      case KeyType::UP:
         mGameLogic->steerUp(PlayerID::P1);
         break;
      }
      ++eventsToRemove;
   }
   // remove processed events
   assert(eventsToRemove <= mKeyEvents.size() && "Size missmatch.");
   if (eventsToRemove != 0)
      mKeyEvents.erase(mKeyEvents.begin(), mKeyEvents.begin() + eventsToRemove);
}
