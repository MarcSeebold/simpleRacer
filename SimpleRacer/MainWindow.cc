#include "MainWindow.hh"
#include "SimpleRacer.hh"

#include <iostream>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), mUI(new Ui::MainWindow), mKeyStatus(new KeyStatus)
{
   SimpleRacer::create();
   mUI->setupUi(this);

   connect(mUI->actionStart_Singleplayer, &QAction::triggered, SimpleRacer::the(), &SimpleRacer::startGame);
   connect(mUI->actionExit, &QAction::triggered, SimpleRacer::the(), &SimpleRacer::exitGame);
}

MainWindow::~MainWindow()
{
   SimpleRacer::destroy();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
   if (!SimpleRacer::the()->isRunning())
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
   if (!SimpleRacer::the()->isRunning())
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
   if (mKeyStatus->up)
      SimpleRacer::the()->inputController()->sendKeyPress(InputController::KeyType::UP);
   if (mKeyStatus->down)
      SimpleRacer::the()->inputController()->sendKeyPress(InputController::KeyType::DOWN);
   if (mKeyStatus->left)
      SimpleRacer::the()->inputController()->sendKeyPress(InputController::KeyType::LEFT);
   if (mKeyStatus->right)
      SimpleRacer::the()->inputController()->sendKeyPress(InputController::KeyType::RIGHT);
}

void MainWindow::changeStatusbarText(const QString &_newText)
{
   mUI->statusBar->showMessage(_newText);
}

void MainWindow::clearStatusbarText()
{
   mUI->statusBar->clearMessage();
}
