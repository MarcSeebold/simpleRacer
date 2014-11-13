#include "MainWindow.hh"
#include "ui_MainWindow.h"
#include "GameLogic.hh"
#include "Rendering.hh"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow), mGameLogic(new simpleRacer::GameLogic), mRendering(new simpleRacer::Rendering)
{
   ui->setupUi(this);
   connect(&mGlobalTimer, &QTimer::timeout, this, &MainWindow::performGameUpdateStep);
   connect(ui->actionStart_Singleplayer, &QAction::triggered, this, &MainWindow::startGame);
   connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitGame);
}

MainWindow::~MainWindow()
{
   delete ui;
}

bool MainWindow::startGame()
{
   if (mGameLogic->getRunning())
   {
      SR_ASSERT(mGlobalTimer.isActive() && "GameLogic active but timer is not.");
      return false; // game is already running
   }

   mGameLogic->setRunning(true);
   mGlobalTimer.start(SR_RESOLUTION);
   return true;
}

bool MainWindow::stopGame()
{
   if (!mGameLogic->getRunning())
   {
      SR_ASSERT(!mGlobalTimer.isActive() && "GameLogic is not active but timer is.");
      return true;
   }

   mGlobalTimer.stop();
   mGameLogic->setRunning(false);
   return true;
}

void MainWindow::exitGame()
{
   stopGame();
   exit(EXIT_SUCCESS);
}

void MainWindow::performGameUpdateStep()
{
   mGameLogic->update(SR_RESOLUTION);
   mRendering->render(mGameLogic->getGameState());
}
