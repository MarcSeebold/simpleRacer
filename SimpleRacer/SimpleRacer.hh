#pragma once
#include <QObject>
#include "Common.hh"
#include "GameLogic.hh"
#include "RenderingWidget.hh"
#include "InputController.hh"

SHARED(class, GameLogic);
SHARED(class, RenderingWidget);
SHARED(class, InputController);

class SimpleRacer : public QObject
{
   Q_OBJECT

public:
   static void create();
   static void destroy();
   static SimpleRacer *the();

public: // static getter
   static SharedGameLogic logicServer() { return the()->mLogicServer; }
   static SharedGameLogic logicClient() { return the()->mLogicClient; }
   static SharedRenderingWidget rendering() { return the()->mRendering; }
   static SharedInputController inputController() { return the()->mInput; }

public slots:
   void startGame();
   void exitGame();

public: // getter
   bool isRunning() const { return mRunning; }

private:
   SimpleRacer();

private:
   static SimpleRacer *sInstance;

   SharedGameLogic mLogicServer;
   SharedGameLogic mLogicClient;
   SharedRenderingWidget mRendering;
   SharedInputController mInput;

private:
   bool mRunning = false;
};
