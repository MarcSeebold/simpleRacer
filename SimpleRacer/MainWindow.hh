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

private:
   void processInput();

public slots:
   /// Change status bar text
   void changeStatusbarText(const QString &_newText);

   /// Clears status bar text
   void clearStatusbarText();

private:
   struct KeyStatus
   {
      bool up = false;
      bool down = false;
      bool left = false;
      bool right = false;
   };
   SHARED(struct, KeyStatus);

   Ui::MainWindow *mUI = nullptr;
   UniqueKeyStatus mKeyStatus;
};
