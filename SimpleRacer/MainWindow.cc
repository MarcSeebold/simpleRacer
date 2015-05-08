#include "MainWindow.hh"
#include "SimpleRacer.hh"
#include "LagSettings.hh"

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

void MainWindow::on_actionDisable_Server_Side_Latency_Compensation_triggered()
{
    LagSettings::the()->setServerSideCompensation(false);
}

void MainWindow::on_actionDisable_Client_Side_Latency_Compensation_triggered()
{
    LagSettings::the()->setClientSideCompensation(false);
}

void MainWindow::on_actionEnable_Server_Side_Latency_Compensation_triggered()
{
    LagSettings::the()->setServerSideCompensation(true);
}

void MainWindow::on_actionEnable_Client_Side_Latency_Compensation_triggered()
{
    LagSettings::the()->setClientSideCompensation(true);
}
