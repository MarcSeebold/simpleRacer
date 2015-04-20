#pragma once
#include <QObject>
#include "Common.hh"
#include "GameLogic.hh"
#include "InputController.hh"
#include "QTimer"
#include "ArtificialRacer.hh"

#define SR_GAMESTEPTIME 10 // every 10 ms = 1/100s * 1000s/ms = 100Hz

class MainWindow;
class RenderingWidget;
SHARED(class, GameLogic);
SHARED(class, InputController);
SHARED(class, ArtificialRacer);
SHARED(class, DelaySimulator);

class SimpleRacer : public QObject
{
   Q_OBJECT

public:
   static void create(MainWindow *_mainWindow, RenderingWidget *_rendering);
   static void destroy();
   static SimpleRacer *the();

public: // static getter
   static SharedGameLogic logicServer() { return the()->mLogicServer; }
   static SharedGameLogic logicClient() { return the()->mLogicClient; }
   static SharedInputController inputController() { return the()->mInput; }
   static SharedArtificialRacer ai() { return the()->mAI; }
   static RenderingWidget *rendering() { return the()->mRendering; }

   static void coinCollectedCallback(QVector2D _pos);
   static void coinSpawnedCallback(QVector2D _pos);

public slots:
   void startGame();
   void exitGame();
   void update();

public: // getter
   bool isRunning() const { return mRunning; }

private:
   SimpleRacer(MainWindow *_mainWindow, RenderingWidget *_rendering);

private:
   static SimpleRacer *sInstance;

   SharedGameLogic mLogicServer;
   SharedGameLogic mLogicClient;
   SharedInputController mInput;
   SharedArtificialRacer mAI;
   SharedDelaySimulator mSynch;

   RenderingWidget *mRendering = nullptr;
   MainWindow *mMainWindow = nullptr;

   QTimer mGameTimer;

private:
   bool mRunning = false;
};
