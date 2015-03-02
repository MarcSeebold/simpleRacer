#pragma once
#include <QObject>
#include "Common.hh"
#include "GameLogic.hh"
#include "RenderingWidget.hh"
#include "InputController.hh"

class SimpleRacer : public QObject
{
   Q_OBJECT

public:
   static void create();
   static void destroy();
   static SimpleRacer *the();

public: // static getter
   static GameLogic *logicServer() { return the()->mLogicServer; }
   static GameLogic *logicClient() { return the()->mLogicClient; }
   static RenderingWidget *rendering() { return the()->mRendering; }
   static InputController *inputController() { return the()->mInput; }

public slots:
   void startGame();
   void exitGame();

public: // getter
   bool isRunning() const { return mRunning; }

private:
   SimpleRacer();

private:
   static SimpleRacer *sInstance;

   GameLogic *mLogicServer = nullptr;
   GameLogic *mLogicClient = nullptr;
   RenderingWidget *mRendering = nullptr;
   InputController *mInput = nullptr;

private:
   bool mRunning = false;
};
