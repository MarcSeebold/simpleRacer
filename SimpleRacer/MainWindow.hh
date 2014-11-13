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
SHARED(class, Rendering);
}

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = 0);
   ~MainWindow();

public slots:
   /// Start the game
   bool startGame();

   /// Stops the game
   bool stopGame();

   /// Exit the game
   void exitGame();

private slots:
   /// Simulates game logic and renders the result
   void performGameUpdateStep();

private:
   Ui::MainWindow *ui;
   simpleRacer::SharedGameLogic mGameLogic;
   simpleRacer::SharedRendering mRendering;
   QTimer mGlobalTimer;

public: // Getter, Setter
   GETTER(GameLogic);
   GETTER(Rendering);
};
