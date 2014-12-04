#pragma once

#include <QMainWindow>
#include <QTimer>
#include <memory>
#include "Common.hh"

// Resolution of game simulation.
#define SR_RESOLUTION 1/60.f

namespace simpleRacer
{
SHARED(class, GameLogic);
SHARED(class, RenderingWidget);
SHARED(class, NetworkEngine);
}

namespace Ui
{
SHARED(class, MainWindow);
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = 0);
   ~MainWindow();

protected:
   void keyPressEvent(QKeyEvent *e);
   void keyReleaseEvent(QKeyEvent *e);

private:
   void processInput();

public slots:
   /// Start a SP game
   bool startSinglePlayerGame();
   /// Start (dedicated) server game
   bool startServerGame();
   /// Connect to a server and start a game
   bool startMPGame();

   /// Stops the game
   bool stopGame();

   /// Exit the game
   void exitGame();

   /// Change status bar text
   void changeStatusbarText(const QString &_newText);

   /// Clears status bar text
   void clearStatusbarText();

private slots:
   /// Simulates game logic and renders the result
   void performGameUpdateStep();

private:
   struct KeyStatus
   {
      bool up = false;
      bool down = false;
      bool left = false;
      bool right = false;
   };
   SHARED(struct,KeyStatus);

   Ui::UniqueMainWindow mUI;
   simpleRacer::SharedGameLogic mGameLogic;
   simpleRacer::SharedRenderingWidget mRendering;
   QTimer mGlobalTimer;
   UniqueKeyStatus mKeyStatus;
   simpleRacer::UniqueNetworkEngine mNetwork;

public: // Getter, Setter
   GETTER(GameLogic);
   GETTER(Rendering);
};
