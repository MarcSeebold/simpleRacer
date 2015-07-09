#include "MainWindow.hh"
#include "SimpleRacer.hh"
#include "Settings.hh"

#include <iostream>
#include <QKeyEvent>
#include <QWebFrame>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), mUI(new Ui::MainWindow)
{
   mUI->setupUi(this);
   SimpleRacer::create(this, mUI->widget);

   connect(mUI->actionExit, &QAction::triggered, SimpleRacer::the(), &SimpleRacer::exitGame);
   connect(Settings::the(), &Settings::testPlayStateChange, this, &MainWindow::onTestPlayStateChanged);
   connect(mUI->webView->page()->mainFrame(), &QWebFrame::javaScriptWindowObjectCleared, this, &MainWindow::populateJSWO);
   // set focus on game
   focusGameWindow();
#ifndef QT_DEBUG
   // Hide Debug-Stuff
   mUI->latencySettings_2->hide();
   mUI->compensationSettings_2->hide();
   mUI->menuBar->hide();
#endif
   mUI->webView->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);

   refreshDebugUI();
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
   mUI->labelLagStatus->setText((_val ? "on" : "off"));
}

void MainWindow::onTestPlayStateChanged(bool _val)
{
   // Show Button for starting a new game
   if (_val)
      mUI->webView->load(QUrl("qrc:/htdocs/testPlay.html"));
   else
      mUI->webView->load(QUrl("qrc:/htdocs/wait.html"));
}

void MainWindow::onStartNewTestGame()
{
   SimpleRacer::the()->startGame();
   // set focus on game
   focusGameWindow();
}

void MainWindow::focusGameWindow()
{
   mUI->widget->setFocus();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
   bool checked = (arg1 == Qt::Checked);
   Settings::the()->setClientSideCompensation(checked);
   // set focus on game
   focusGameWindow();
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
   bool checked = (arg1 == Qt::Checked);
   Settings::the()->setServerSideCompensation(checked);
   // set focus on game
   focusGameWindow();
}

void MainWindow::on_pushButton_clicked()
{
   // lag amount
   {
      bool ok = true;
      float l = mUI->editLatency->text().toInt(&ok);
      l /= 1000; // ms to s
      l /= 2;    // latency: server->client + client->server
      if (ok && l >= 0)
      {
         Settings::the()->setLatencyServerToClient(l);
         Settings::the()->setLatencyClientToServer(l);
      }
      else
         mUI->editLatency->setText(QString::number(
             int((Settings::the()->getLatencyClientToServer() + Settings::the()->getLatencyServerToClient()) * 1000.f)));
   }
   // lag probability
   {
      bool ok = true;
      float p = mUI->editProbability->text().toFloat(&ok);
      if (p >= 0 && p <= 1 && ok)
      {
         Settings::the()->setLagProbability(LagProbability::CUSTOM);
         Settings::the()->setLagProbabilityCustom(p);
      }
      else
         mUI->editProbability->setText(QString::number(Settings::the()->getLagProbabilityCustom()));
   }
   // lag duration
   {
      bool ok = true;
      float d = mUI->editDuration->text().toFloat(&ok);
      if (d >= 0 && ok)
      {
         Settings::the()->setLagDuration(d);
      }
      else
         mUI->editDuration->setText(QString::number(Settings::the()->getLagDuration()));
   }
   // set focus on game
   focusGameWindow();
}

void MainWindow::on_actionStart_Singleplayer_triggered()
{
   SimpleRacer::the()->startGame();
   // set focus on game
   focusGameWindow();
}

void MainWindow::populateJSWO()
{
   // register this class in javascript
   mUI->webView->page()->mainFrame()->addToJavaScriptWindowObject("simpleRacer", this);
}

void MainWindow::on_pushButton_2_clicked()
{
   // save config
   QFile h("./settings.json");
   _ ok = h.open(QIODevice::ReadWrite | QIODevice::Text);
   SR_ASSERT(ok && "failed to open file");

   QJsonObject jObj;
   Settings::the()->write(jObj);
   QJsonDocument jDoc(jObj);
   h.write(jDoc.toJson());
   h.close();
   QMessageBox::information(this, tr("Info: Settings written"), tr("Settings saved as \"settings.json\"."), QMessageBox::Ok);
}

void MainWindow::on_pushButton_3_clicked()
{
   // load config
   QFile h("./settings.json");
   _ ok = h.open(QIODevice::ReadOnly | QIODevice::Text);
   if (!ok) // no config to load
   {
      QMessageBox::warning(this, tr("Error: No settings file"),
                           tr("Could not load settings: No settings.json file found."), QMessageBox::Ok);
      return;
   }
   _ data = h.readAll();
   h.close();
   _ json = QJsonDocument::fromJson(data);
   Settings::the()->read(json.object());
   refreshDebugUI();
   QMessageBox::information(this, tr("Info: Settings loaded"), tr("Settings loaded from \"settings.json\"."), QMessageBox::Ok);
}

void MainWindow::refreshDebugUI()
{
   mUI->editLatency->setText(QString::number(
       int((Settings::the()->getLatencyClientToServer() + Settings::the()->getLatencyServerToClient()) * 1000.f)));
   mUI->editProbability->setText(QString::number(Settings::the()->getLagProbabilityCustom()));
   mUI->editDuration->setText(QString::number(Settings::the()->getLagDuration()));
   { // checkboxes
      Qt::CheckState c, s;
      _ c1 = Settings::the()->getClientSideInterpolation();
      _ c2 = Settings::the()->getClientSidePhysics();
      _ c3 = Settings::the()->getClientSidePrediction();
      if (c1 == c2 && c2 == c3)
         c = (c1 ? Qt::Checked : Qt::Unchecked);
      else
         c = Qt::PartiallyChecked;

      _ s1 = Settings::the()->getServerSideLagCompensation();
      s = (s1 ? Qt::Checked : Qt::Unchecked);
      mUI->checkBox->blockSignals(true);
      mUI->checkBox_2->blockSignals(true);
      mUI->checkBox->setCheckState(c);
      mUI->checkBox_2->setCheckState(s);
      mUI->checkBox->blockSignals(false);
      mUI->checkBox_2->blockSignals(false);
   }
}

void MainWindow::on_pushButton_4_clicked()
{
   // set focus on game
   focusGameWindow();
}
