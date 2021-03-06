#pragma once
#include <QObject>
#include "Common.hh"
#include "GameLogic.hh"
#include "InputController.hh"
#include "QTimer"
#include "ArtificialRacer.hh"

// TODO: Move to settings
#define SR_GAMESTEPTIME (1 / 60.f * 1000.f) // every 16,666... ms = 1/60s * 1000s/ms = 60Hz

class MainWindow;
class RenderingWidget;
SHARED(class, GameLogic);
SHARED(class, InputController);
SHARED(class, ArtificialRacer);
SHARED(class, DelaySimulator);
SHARED(class, NetworkEngine);
SHARED(class, SurveyEngine);
SHARED(class, ScreenRecorder);

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
   /// Singleton getter. Make sure to call create() before the() and destroy() to free up resources.
   static SimpleRacer *the();

public: // static getter
   static SharedGameLogic logicServer() { return the()->mLogicServer; }
   static SharedGameLogic logicClient() { return the()->mLogicClient; }
   static SharedInputController inputController() { return the()->mInput; }
   static SharedArtificialRacer ai() { return the()->mAI; }
   static RenderingWidget *rendering() { return the()->mRendering; }
   static SharedSurveyEngine survey() { return the()->mSurveyEngine; }
   static SharedDelaySimulator delaySimulator() { return the()->mSynch; }
   static SharedScreenRecorder screenRecorder() { return the()->mScreenRecorder; }

public slots:
   void showInstructionsAndStartGame();

   /// Starts the training phase
   void startTraining();

   /// Start a game
   void startGame();
   /// Stop the current game
   void stopGame();
   /// Exit everything
   void exitGame();

   /// Main game update function
   void update();
   /// Called when the user has finished a survey
   void onSurveyFinished(SurveyType _type);

public: // getter
   bool isRunning() const { return mRunning; }

private:
   // c'tor
   SimpleRacer(MainWindow *_mainWindow, RenderingWidget *_rendering);
   ~SimpleRacer();

   /// Starts the next training phase
   /// Make sure that the training phase is not over yet
   /// Calls startTrainingGame()
   void startNextTraining();
   /// Start a game with the config of the current training-phase
   void startTrainingGame();

private:
   static SimpleRacer *sInstance;

   SharedGameLogic mLogicServer;
   SharedGameLogic mLogicClient;
   SharedInputController mInput;
   SharedArtificialRacer mAI;
   SharedDelaySimulator mSynch;
   SharedNetworkEngine mNetwork;
   SharedSurveyEngine mSurveyEngine;
   SharedScreenRecorder mScreenRecorder;
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
