#include "MainWindow.hh"
#include "SimpleRacer.hh"

#include <iostream>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), mUI(new Ui::MainWindow)
{
   mUI->setupUi(this);
   SimpleRacer::create(this, mUI->widget);

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

   SimpleRacer::inputController()->keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
   if (!SimpleRacer::the()->isRunning())
      return;

   SimpleRacer::inputController()->keyReleaseEvent(e);
}

void MainWindow::changeStatusbarText(const QString &_newText)
{
   mUI->statusBar->showMessage(_newText);
}

void MainWindow::clearStatusbarText()
{
   mUI->statusBar->clearMessage();
}
