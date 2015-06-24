#pragma once
#include <QObject>
#include "Common.hh"
#include "GameLogic.hh"
#include "InputController.hh"
#include "QTimer"
#include "ArtificialRacer.hh"

#define SR_GAMESTEPTIME (1 / 60.f * 1000.f) // every 16,666... ms = 1/60s * 1000s/ms = 60Hz

class MainWindow;
class RenderingWidget;
SHARED(class, GameLogic);
SHARED(class, InputController);
SHARED(class, ArtificialRacer);
SHARED(class, DelaySimulator);
SHARED(class, NetworkEngine);
SHARED(class, SurveyEngine);

enum class GameState : char
{
   INVALID = 0,
   PLAYING,
   SURVEY,
   WAITING
};

class SimpleRacer : public QObject
{
   Q_OBJECT

public:
   /// Singleton create
   static void create(MainWindow *_mainWindow, RenderingWidget *_rendering);
   /// Singleton destroy
   static void destroy();
   /// Singleton getter. Make sure to call create() before the() and destory() to free up resources.
   static SimpleRacer *the();

public: // static getter
   static SharedGameLogic logicServer() { return the()->mLogicServer; }
   static SharedGameLogic logicClient() { return the()->mLogicClient; }
   static SharedInputController inputController() { return the()->mInput; }
   static SharedArtificialRacer ai() { return the()->mAI; }
   static RenderingWidget *rendering() { return the()->mRendering; }
   static SharedSurveyEngine survey() { return the()->mSurveyEngine; }
   static SharedDelaySimulator delaySimulator() { return the()->mSynch; }

public slots:
   void startGame();
   void stopGame();
   void exitGame();
   /// Main game update function
   void update();

public: // getter
   bool isRunning() const { return mRunning; }

private:
   // c'tor
   SimpleRacer(MainWindow *_mainWindow, RenderingWidget *_rendering);
   ~SimpleRacer();

private:
   static SimpleRacer *sInstance;

   SharedGameLogic mLogicServer;
   SharedGameLogic mLogicClient;
   SharedInputController mInput;
   SharedArtificialRacer mAI;
   SharedDelaySimulator mSynch;
   SharedNetworkEngine mNetwork;
   SharedSurveyEngine mSurveyEngine;
   float mTimeLeft = -1;     ///< Game-Timer
   float mStartTimer = -1;   ///< Another timer for pre-game start (e.g., 3 seconds countdown before game start)
   float mFadeOutTimer = -1; ///< Timer for letting the game widget fade out after game over

   RenderingWidget *mRendering = nullptr;
   MainWindow *mMainWindow = nullptr;

   QTimer mGameTimer;

private:
   bool mRunning = false;
   GameState mGameState = GameState::INVALID;

public:
   PROPERTY(GameState);

private:
   /// Singleton: no default c'tor
   SimpleRacer() = delete;
   /// Singleton: No copy c'tor
   SimpleRacer(SimpleRacer const &) = delete;
   /// Singleton: No copy assignment
   SimpleRacer &operator=(SimpleRacer const &) = delete;
};
