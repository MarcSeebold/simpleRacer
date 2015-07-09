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
#include "ScreenRecorder.hh"
#include <QDir>

SimpleRacer *SimpleRacer::sInstance = nullptr;

SimpleRacer::SimpleRacer(MainWindow *_mainWindow, RenderingWidget *_rendering)
  : mLogicServer(new GameLogic(GameLogic::Type::SERVER)),
    mLogicClient(new GameLogic(GameLogic::Type::CLIENT)),
    mInput(new InputController(mLogicClient)),
    mAI(new ArtificialRacer(PlayerID::P2, mLogicServer)),
    mSynch(new DelaySimulator(mLogicClient, mLogicServer)),
    mNetwork(new NetworkEngine),
    mSurveyEngine(new SurveyEngine(_mainWindow->mUI->webView, _rendering)),
    mScreenRecorder(new ScreenRecorder(this)),
    mRendering(_rendering),
    mMainWindow(_mainWindow)
{
   // At first: show a waiting screen
   mSurveyEngine->toogleWebWindow(true);
   setGameState(GameState::WAITING);
   mSurveyEngine->showWaitingScreen();
   mNetwork->listen();

   // record screen
   QDir dir("./screendumps/");
   if (!dir.exists())
      dir.mkpath("."); // ensure path exists
   mScreenRecorder->startRecording(dir.absolutePath() + "/" + QString::number(common::getCurrentTimestamp()) + ".mkv");

   connect(&mGameTimer, &QTimer::timeout, this, &SimpleRacer::update);
}

SimpleRacer::~SimpleRacer()
{
   StatisticsEngine::the()->saveToFile();
   mScreenRecorder->stopRecording();
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
}

void SimpleRacer::destroy()
{
   SR_ASSERT(sInstance && "destroy called without create or twice");
   delete sInstance;
   sInstance = nullptr;
}

SimpleRacer *SimpleRacer::the()
{
   SR_ASSERT(sInstance && "the() called, but instance is null");
   return sInstance;
}

void SimpleRacer::showInstructionsAndStartGame()
{
   bool ok = mSurveyEngine->showInstructions(Settings::the()->getUserInstruction());
   if (!ok)
      startGame(); // no valid instruction: startGame without one
}

void SimpleRacer::startTraining()
{
   Settings::the()->setUserTrainingState(TrainingState::DRIVING_1);
   startTrainingGame();
}

void SimpleRacer::startNextTraining()
{
   // set next stage
   const _ currTraining = Settings::the()->getUserTrainingState();
   if (currTraining == TrainingState::DONE)
   {
      SR_ASSERT(0 && "Training is already over. There is no next one.");
      return;
   }
   const _ nextTraining = TrainingState(((int)currTraining) + 1);
   Settings::the()->setUserTrainingState(nextTraining);
   // start game
   startTrainingGame();
}

void SimpleRacer::startTrainingGame()
{
   const _ state = Settings::the()->getUserTrainingState();
   switch (state)
   {
   // Intended fall-through
   case TrainingState::DRIVING_1:
   case TrainingState::DRIVING_2:
   case TrainingState::DRIVING_3:
      Settings::the()->loadCondition(1);
      // No special instruction
      Settings::the()->setUserInstruction(Instruction::NONE);
      // Do not make a survey after a game has ended
      Settings::the()->setMakeSurveyAfterGame(false);
      // Show instructions and start it
      showInstructionsAndStartGame();
      break;

   case TrainingState::TASK_1:
      Settings::the()->loadCondition(1);
      // Make a survey after a game has ended
      Settings::the()->setMakeSurveyAfterGame(true);
      // Show instructions and start it
      showInstructionsAndStartGame();
      break;
   case TrainingState::TASK_2:
      Settings::the()->loadCondition(2);
      // Make a survey after a game has ended
      Settings::the()->setMakeSurveyAfterGame(true);
      // Show instructions and start it
      showInstructionsAndStartGame();
      break;

   case TrainingState::DELAY:
      Settings::the()->loadCondition(3);
      // Make a survey after a game has ended
      Settings::the()->setMakeSurveyAfterGame(true);
      // No special instruction
      Settings::the()->setUserInstruction(Instruction::NONE);
      // Show instructions and start it
      showInstructionsAndStartGame();
      break;

   case TrainingState::INCONSISTENCY_1:
      Settings::the()->loadCondition(10);
      // Make a survey after a game has ended
      Settings::the()->setMakeSurveyAfterGame(true);
      // No special instruction
      Settings::the()->setUserInstruction(Instruction::NONE);
      // Show instructions and start it
      showInstructionsAndStartGame();
      break;
   case TrainingState::INCONSISTENCY_2:
      Settings::the()->loadCondition(14);
      // Make a survey after a game has ended
      Settings::the()->setMakeSurveyAfterGame(true);
      // No special instruction
      Settings::the()->setUserInstruction(Instruction::NONE);
      // Show instructions and start it
      showInstructionsAndStartGame();
      break;

   case TrainingState::DONE:
      // load default condition
      Settings::the()->loadCondition(0);
      // next games should have surveys after game-end
      Settings::the()->setMakeSurveyAfterGame(true);
      break;
   default:
      SR_ASSERT(0 && "unhandled case");
      break;
   }
}

void SimpleRacer::startGame()
{
   // stop game if already running
   if (isRunning())
      stopGame();
   // create new screencap file
   mScreenRecorder->stopRecording();
   sInstance->mScreenRecorder->startRecording("./screendumps/" + QString::number(common::getCurrentTimestamp())
                                              + ".mkv");
   // stats
   StatisticsEngine::the()->tellNewGameRound();
   mMainWindow->mUI->labelBG->hide();
   // hide survey, show game
   mSurveyEngine->toogleWebWindow(false);
   // TODO: Move to settings
   mTimeLeft = 60.f; // One minute gameplay
   // TODO: Move to settings
   mStartTimer = 3.5f; // 3 Second-countdown before start
   mMainWindow->mUI->widget->setOpacity(1.f);
   mGameTimer.start(SR_GAMESTEPTIME);
   mRunning = true;
   setGameState(GameState::PLAYING);
   mMainWindow->focusGameWindow();
}

void SimpleRacer::stopGame()
{
   mGameTimer.stop();
   logicClient()->reset();
   logicServer()->reset();
   mRunning = false;
   // show waiting screen, hide game
   mSurveyEngine->showWaitingScreen();
   mSurveyEngine->toogleWebWindow(true);
   setGameState(GameState::WAITING);
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
         // if this was an condition-game: open ingame survey
         if (mSurveyEngine->getLastLoadedCondition() > 0 && Settings::the()->getMakeSurveyAfterGame())
         {
            mSurveyEngine->makeSurvey(SurveyType::INGAME);
         }
         return;
      }
      // reduce opacity of render window with decreasing timer value
      // TODO: Move to settings
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
            // TODO: Move to settings
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
            gameOverText += "You got " + QString::number(scoreP1) + " points!";
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
   // 60/UpdateRateInHz = Number of updates per second; -1 because we count from 0 on
   int everyXFrames = int((60.f / Settings::the()->getNetworkUpdateRate())) - 1;
   if (counter == everyXFrames)
   {
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
