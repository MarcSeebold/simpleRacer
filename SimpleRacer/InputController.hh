#pragma once
#include <vector>
#include "Common.hh"

namespace simpleRacer
{
SHARED(class, GameLogic);

/// Processes user input
/// Mainly used for delaying user input as effect of lag simulation
class InputController
{
public:
   enum class KeyType : char
   {
      UP,
      DOWN,
      LEFT,
      RIGHT
   };

public:
   /// c'tor
   InputController(simpleRacer::SharedGameLogic _logic);

   /// inform us about a keypress
   void sendKeyPress(KeyType _type);
   /// process keypresses
   void update();
   /// Set user-input delay
   void setDelay(int64_t _delay) { mDelay = _delay; }

private:
   struct KeyEvent
   {
      int64_t timestamp;
      KeyType type;
   };

private:
   std::vector<KeyEvent> mKeyEvents;        ///< Unprocessed key events
   int64_t mDelay = 500;                    ///< How much delay should we add to user input?
   simpleRacer::SharedGameLogic mGameLogic; ///< GameLogic
};
} // namespace
