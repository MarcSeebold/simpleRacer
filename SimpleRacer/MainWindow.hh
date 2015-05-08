#pragma once

#include <QMainWindow>
#include <QTimer>
#include <memory>
#include "Common.hh"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = 0);
   ~MainWindow();

protected:
   void keyPressEvent(QKeyEvent *e);
   void keyReleaseEvent(QKeyEvent *e);

public slots:
   /// Change status bar text
   void changeStatusbarText(const QString &_newText);

   /// Clears status bar text
   void clearStatusbarText();

private slots:
   void on_actionDisable_Server_Side_Latency_Compensation_triggered();

   void on_actionDisable_Client_Side_Latency_Compensation_triggered();

   void on_actionEnable_Server_Side_Latency_Compensation_triggered();

   void on_actionEnable_Client_Side_Latency_Compensation_triggered();

private:
   Ui::MainWindow *mUI = nullptr;

   friend class SimpleRacer;
};
