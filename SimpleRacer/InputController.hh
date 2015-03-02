#pragma once
#include <vector>
#include <list>
#include <QObject>
#include "Common.hh"

class QTimer;
class QKeyEvent;
SHARED(class, GameLogic);

/// Processes user input
/// Mainly used for delaying user input as effect of lag simulation
class InputController : public QObject
{
   Q_OBJECT

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
   InputController(SharedGameLogic _client, SharedGameLogic _server);

   /// d'tor
   ~InputController();

   void keyPressEvent(QKeyEvent *e);
   void keyReleaseEvent(QKeyEvent *e);

   /// process keypresses
   void update();
   /// Set user-input delay
   void setDelay(int64_t _delay) { mDelay = _delay; }

private:
   struct KeyStatus
   {
      bool up = false;
      bool down = false;
      bool left = false;
      bool right = false;
   };

private:
   int64_t mDelay = 500;                 ///< How much delay should we add to user input?
   SharedGameLogic mGameLogicClient;     ///< Client-Logic
   SharedGameLogic mGameLogicServer;     ///< Server-Logic
   std::vector<QTimer*> mDelayedActions; ///< Delayed input events using QTimer
   KeyStatus mKeyStatus;
};
