#include "SimpleRacer.hh"
#include "Common.hh"
#include "RenderingWidget.hh"
#include "MainWindow.hh"
#include "Settings.hh"
#include "DelaySimulator.hh"
#include "StatisticsEngine.hh"
#include "NetworkEngine.hh"
#include "Testing.hh"
#include "SurveyEngine.hh"

SimpleRacer *SimpleRacer::sInstance = nullptr;

SimpleRacer::SimpleRacer(MainWindow *_mainWindow, RenderingWidget *_rendering)
  : mLogicServer(new GameLogic(GameLogic::Type::SERVER)),
    mLogicClient(new GameLogic(GameLogic::Type::CLIENT)),
    mInput(new InputController(mLogicClient)),
    mAI(new ArtificialRacer(PlayerID::P2, mLogicServer)),
    mSynch(new DelaySimulator(mLogicClient, mLogicServer)),
    mNetwork(new NetworkEngine),
    mSurveyEngine(new SurveyEngine(_mainWindow->mUI->webView, _rendering)),
    mRendering(_rendering),
    mMainWindow(_mainWindow)
{
   connect(&mGameTimer, &QTimer::timeout, this, &SimpleRacer::update);
}

SimpleRacer::~SimpleRacer()
{
}

void SimpleRacer::create(MainWindow *_mainWindow, RenderingWidget *_rendering)
{
   SR_ASSERT(_rendering && "rendering is null");
   SR_ASSERT(!sInstance && "create() already called.");
   Settings::the();
#ifdef QT_DEBUG // run tests in debug mode
   Testing tests;
   tests.runTestSettingsSaveLoad();
#endif
   sInstance = new SimpleRacer(_mainWindow, _rendering);
   // At first: show a waiting screen
   sInstance->mSurveyEngine->toogleSurveyWindow(true);
   sInstance->mSurveyEngine->showWaitingScreen();
   sInstance->mNetwork->listen();
}

void SimpleRacer::destroy()
{
   SR_ASSERT(sInstance && "destroy called without create or twice");
   StatisticsEngine::the()->saveToFile();
   delete sInstance;
   sInstance = nullptr;
}

SimpleRacer *SimpleRacer::the()
{
   SR_ASSERT(sInstance && "the() called, but instance is null");
   return sInstance;
}

void SimpleRacer::startGame()
{
   StatisticsEngine::the()->tellNewGameRound();
   mMainWindow->mUI->labelBG->hide();
   // hide survey, show game
   mSurveyEngine->toogleSurveyWindow(false);
   mTimeLeft = 60.f;   // One minute gameplay
   mStartTimer = 3.5f; // 3 Second-countdown before start
   mMainWindow->mUI->widget->setOpacity(1.f);
   mGameTimer.start(SR_GAMESTEPTIME);
   mRunning = true;
}

void SimpleRacer::stopGame()
{
   mGameTimer.stop();
   logicClient()->reset();
   logicServer()->reset();
   mRunning = false;
   // show survey, hide game
   mSurveyEngine->toogleSurveyWindow(true);
}

void SimpleRacer::exitGame()
{
   if (isRunning())
      stopGame();
   exit(EXIT_SUCCESS);
}

void SimpleRacer::update()
{
   const float timeStep = SR_GAMESTEPTIME / 1000.f; // ms -> s

   if (mFadeOutTimer > 0)
   {
      mFadeOutTimer -= timeStep;
      if (mFadeOutTimer <= 0)
      {
         // fadeout complete: stop the game
         stopGame();
         return;
      }
      // reduce opacity of render window with decreasing timer value
      _ opacity = (mFadeOutTimer / 3.5f);
      mMainWindow->mUI->widget->setOpacity(opacity);
      mMainWindow->repaint(); // paint
      return;                 // game is paused when fading-out
   }

   // Countdown
   if (mStartTimer > 0)
   {
      mStartTimer -= timeStep;
      if (mStartTimer <= 0)
      {
         mMainWindow->mUI->labelBG->hide();
         return;
      }
      QString text = "Starting in " + QString::number((int)mStartTimer);
      mMainWindow->mUI->labelBG->setText(text);
      mMainWindow->mUI->labelBG->show();
      mMainWindow->repaint(); // rendering
      return;                 // Game is paused while countdown
   }

   // Game-Timer
   {
      mTimeLeft -= timeStep;
      if (mTimeLeft <= 0)
      {
         mMainWindow->mUI->labelTimer->setText("0");
         int scoreP1 = mLogicServer->getScore(PlayerID::P1);
         int scoreP2 = mLogicServer->getScore(PlayerID::P2);
         // declare winner
         {
            mFadeOutTimer = 3.5f; // Fade-out game widget
            QString gameOverText = "Game Over\n";
#if 0
            if (scoreP1 > scoreP2)
               gameOverText += "You have won!";
            else if (scoreP1 < scoreP2)
               gameOverText += "You have lost!";
            else
               gameOverText += "Draw!";
#else
            (void)&scoreP2;
            gameOverText += "You achived " + QString::number(scoreP1) + " points!";
#endif
            mMainWindow->mUI->labelBG->setText(gameOverText);
            mMainWindow->mUI->labelBG->show();
         }
         return;
      }
      mMainWindow->mUI->labelTimer->setText(QString::number((int)mTimeLeft));
   }
   // Input
   mInput->update();
   // Game Logic
   mLogicClient->update(timeStep);
   mLogicServer->update(timeStep);
   // AI
   mAI->tellOwnPosition(mLogicServer->getCarCenterPosition(PlayerID::P2));
   mAI->tellEnemyPosition(mLogicServer->getCarCenterPosition(PlayerID::P1));
   const _ coins = mLogicServer->getCoins();
   const _ muds = mLogicServer->getMuds();
   QVector2D cPos(-1, -1);
   QVector2D mPos(-1, -1);
   if (!coins.empty())
      cPos = coins[0];
   if (!muds.empty())
      mPos = muds[0];
   mAI->tellCoinPosition(cPos);
   mAI->tellMudPosition(mPos);
   mAI->update();
   // Rendering
   mMainWindow->update();
   // Synch Server and Client
   static int counter = 0;
   if (counter == 2) // 0,1,2
   { // only send network stuff all three frames (at 60Hz that means 20Hz, default value for a source server)
      // Client->Server
      mSynch->csSendInput(mInput->getKeyStatus());
      // Server->Client
      mSynch->scSendCar(PlayerID::P1, mLogicServer->getCarCenterPosition(PlayerID::P1),
                        mLogicServer->getCarLinearVelocity(PlayerID::P1));
      mSynch->scSendCar(PlayerID::P2, mLogicServer->getCarCenterPosition(PlayerID::P2),
                        mLogicServer->getCarLinearVelocity(PlayerID::P2));
      mSynch->scSendCoins(mLogicServer->getCoins());
      mSynch->scSendMuds(mLogicServer->getMuds());
      mSynch->scSendScore(PlayerID::P1, mLogicServer->getScore(PlayerID::P1));
      mSynch->scSendScore(PlayerID::P2, mLogicServer->getScore(PlayerID::P2));
      // Update function
      mSynch->update();
      counter = 0;
   }
   else
   {
      ++counter;
   }
   // Update main window
   {
      mMainWindow->mUI->labelP1Points->setText(QString::number(mLogicClient->getScore(PlayerID::P1)));
      mMainWindow->mUI->labelP2Points->setText(QString::number(mLogicClient->getScore(PlayerID::P2)));
   }
}
