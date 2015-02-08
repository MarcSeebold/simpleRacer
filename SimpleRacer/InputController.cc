#include "InputController.hh"

using namespace simpleRacer;

InputController::InputController()
{
}

void InputController::sendKeyPress(InputController::KeyType _type)
{
    mKeyEvents.push_back(UniqueKeyEvent(new KeyEvent{common::getCurrentTimestamp(), _type}));
}

void InputController::update()
{

}
