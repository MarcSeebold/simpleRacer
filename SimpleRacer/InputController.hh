#pragma once
#include <vector>
#include <list>
#include "Common.hh"
#include <QObject>

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

   struct KeyStatus
   {
      bool up = false;
      bool down = false;
      bool left = false;
      bool right = false;
   };

public:
   /// c'tor
   InputController(SharedGameLogic _client);

   /// d'tor
   ~InputController();

   void keyPressEvent(QKeyEvent *e);
   void keyReleaseEvent(QKeyEvent *e);

   const KeyStatus &getKeyStatus() const { return mKeyStatus; }

   /// process keypresses
   void update();

private:
   SharedGameLogic mGameLogicClient;      ///< Client-Logic
   KeyStatus mKeyStatus;
};
