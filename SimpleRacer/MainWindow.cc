#include "MainWindow.hh"
#include "ui_MainWindow.h"
#include "GameLogic.hh"
#include "RenderingWidget.hh"
#include "NetworkEngine.hh"

#include <iostream>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    mUI(new Ui::MainWindow),
    mGameLogic(new simpleRacer::GameLogic),
    mRendering(new simpleRacer::RenderingWidget),
    mKeyStatus(new KeyStatus),
    mNetwork(new simpleRacer::NetworkEngine)
{
   mUI->setupUi(this);
   mUI->widget->setGameLogicComponent(mGameLogic);
   connect(&mGlobalTimer, &QTimer::timeout, this, &MainWindow::performGameUpdateStep);
   connect(mUI->actionStart_Singleplayer, &QAction::triggered, this, &MainWindow::startSinglePlayerGame);
   connect(mUI->actionConnect_to, &QAction::triggered, this, &MainWindow::startMPGame);
   connect(mUI->actionStart_Server_Dedicated, &QAction::triggered, this, &MainWindow::startServerGame);
   connect(mUI->actionExit, &QAction::triggered, this, &MainWindow::exitGame);
}

MainWindow::~MainWindow()
{
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
   if (!mGameLogic->getRunning())
      return;

   switch (e->key())
   {
   case Qt::Key_Up:
      mKeyStatus->up = true;
      break;
   case Qt::Key_Down:
      mKeyStatus->down = true;
      break;
   case Qt::Key_Left:
      mKeyStatus->left = true;
      break;
   case Qt::Key_Right:
      mKeyStatus->right = true;
      break;
   }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
   if (!mGameLogic->getRunning())
      return;

   switch (e->key())
   {
   case Qt::Key_Up:
      mKeyStatus->up = false;
      break;
   case Qt::Key_Down:
      mKeyStatus->down = false;
      break;
   case Qt::Key_Left:
      mKeyStatus->left = false;
      break;
   case Qt::Key_Right:
      mKeyStatus->right = false;
      break;
   }
}

void MainWindow::processInput()
{
   _ p = simpleRacer::GameLogic::PlayerID::P1;
   if (mKeyStatus->up)
      mGameLogic->steerUp(p);
   if (mKeyStatus->down)
      mGameLogic->steerDown(p);
   if (mKeyStatus->left)
      mGameLogic->decelerate(p);
   if (mKeyStatus->right)
      mGameLogic->accelerate(p);
}

bool MainWindow::startSinglePlayerGame()
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

bool MainWindow::startServerGame()
{
   if (mGameLogic->getRunning())
   {
      SR_ASSERT(mGlobalTimer.isActive() && "GameLogic active but timer is not.");
      return false; // game is already running
   }
   mNetwork->startServer();
   // TODO: IMPLEMENT
   return true;
}

bool MainWindow::startMPGame()
{
   if (mGameLogic->getRunning())
   {
      SR_ASSERT(mGlobalTimer.isActive() && "GameLogic active but timer is not.");
      return false; // game is already running
   }
   //mNetwork->connectTo(host, port);
   // TODO: IMPLEMENT
   return true;
}

bool MainWindow::stopGame()
{
   if (!mGameLogic->getRunning())
   {
      SR_ASSERT(!mGlobalTimer.isActive() && "GameLogic is not active but timer is.");
      return true;
   }
   mNetwork->stop();
   mGlobalTimer.stop();
   mGameLogic->setRunning(false);
   return true;
}

void MainWindow::exitGame()
{
   stopGame();
   exit(EXIT_SUCCESS);
}

void MainWindow::changeStatusbarText(const QString &_newText)
{
    mUI->statusBar->showMessage(_newText);
}

void MainWindow::clearStatusbarText()
{
    mUI->statusBar->clearMessage();
}

void MainWindow::performGameUpdateStep()
{
   // process input
   processInput();
   // game logic
   mGameLogic->update(SR_RESOLUTION);
   // rendering
   update();
   // update score
   _ p1Score = mGameLogic->getScore(simpleRacer::GameLogic::PlayerID::P1);
   _ p2Score = mGameLogic->getScore(simpleRacer::GameLogic::PlayerID::P2);
   mUI->labelP1Points->setText(QString::number(p1Score));
   mUI->labelP2Points->setText(QString::number(p2Score));
}
