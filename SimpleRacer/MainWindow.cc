#include "MainWindow.hh"
#include "SimpleRacer.hh"
#include "LagSettings.hh"

#include <iostream>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), mUI(new Ui::MainWindow)
{
   mUI->setupUi(this);
   SimpleRacer::create(this, mUI->widget);

   connect(mUI->actionExit, &QAction::triggered, SimpleRacer::the(), &SimpleRacer::exitGame);
   // set focus on game
   mUI->widget->setFocus();
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

void MainWindow::setLagStatusLabel(bool _val)
{
   mUI->labelLagStatus->setText((_val? "on":"off"));
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
   bool checked = (arg1 == Qt::Checked);
   LagSettings::the()->setClientSideCompensation(checked);
   // set focus on game
   mUI->widget->setFocus();
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
   bool checked = (arg1 == Qt::Checked);
   LagSettings::the()->setServerSideCompensation(checked);
   // set focus on game
   mUI->widget->setFocus();
}

void MainWindow::on_pushButton_clicked()
{
   // lag amount
   {
      bool ok = true;
      float l = mUI->editLatency->text().toInt(&ok);
      l /= 1000; // ms to s
      l /= 2; // latency: server->client + client->server
      if (ok && l >= 0)
      {
         LagSettings::the()->setLatencyServerToClient(l);
         LagSettings::the()->setLatencyClientToServer(l);
      }
      else
         mUI->editLatency->setText("150");
   }
   // lag probability
   {
      bool ok = true;
      float p = mUI->editProbability->text().toFloat(&ok);
      if (p >= 0 && p <= 1 && ok)
      {
         LagSettings::the()->setLagProbability(LagProbability::CUSTOM);
         LagSettings::the()->setLagProbabilityCustom(p);
      }
      else
         mUI->editProbability->setText("0.5");
   }
   // lag duration
   {
      bool ok = true;
      float d = mUI->editDuration->text().toFloat(&ok);
      if (d >= 0 && ok)
      {
         LagSettings::the()->setLagDuration(d);
      }
      else
         mUI->editDuration->setText("1.0");
   }
   // set focus on game
   mUI->widget->setFocus();
}

void MainWindow::on_actionStart_Singleplayer_triggered()
{
   SimpleRacer::the()->startGame();
   // set focus on game
   mUI->widget->setFocus();
}
