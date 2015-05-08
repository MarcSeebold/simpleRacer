#include "SimpleRacer.hh"
#include "Common.hh"
#include "RenderingWidget.hh"
#include "MainWindow.hh"
#include "LagSettings.hh"
#include "DelaySimulator.hh"

SimpleRacer *SimpleRacer::sInstance = nullptr;

void SimpleRacer::create(MainWindow *_mainWindow, RenderingWidget *_rendering)
{
   SR_ASSERT(_rendering && "rendering is null");
   SR_ASSERT(!sInstance && "create() already called.");
   LagSettings::the();
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

void SimpleRacer::coinCollectedCallback(QVector2D _pos)
{
   the()->ai()->tellCoinHasBeenCollected(_pos);
}

void SimpleRacer::coinSpawnedCallback(QVector2D _pos)
{
   the()->ai()->tellCoinHasBeenSpawned(_pos);
}

void SimpleRacer::startGame()
{
   mGameTimer.start(SR_GAMESTEPTIME);
   mRunning = true;
}

void SimpleRacer::exitGame()
{
   mGameTimer.stop();
   logicClient()->reset();
   logicServer()->reset();
   mRunning = false;
   exit(EXIT_SUCCESS);
}

void SimpleRacer::update()
{
   // Input
   mInput->update();
   // Game Logic
   mLogicClient->update(SR_GAMESTEPTIME / 1000.f); // ms -> s
   mLogicServer->update(SR_GAMESTEPTIME / 1000.f); // ms -> s
   // AI
   mAI->tellOwnPosition(mLogicServer->getCarCenterPosition(PlayerID::P2));
   mAI->update();
   // Rendering
   mMainWindow->repaint();
   // Synch Server and Client
   static int counter = 0;
   if (counter == 5)
   { // only send network stuff all six frames (at 60Hz that means 10Hz, compareable with a source server)
      // Client->Server
      mSynch->csSendInput(mInput->getKeyStatus());
      // Server->Client
      mSynch->scSendCar(PlayerID::P1, mLogicServer->getCarCenterPosition(PlayerID::P1),
                        mLogicServer->getCarLinearVelocity(PlayerID::P1));
      mSynch->scSendCar(PlayerID::P2, mLogicServer->getCarCenterPosition(PlayerID::P2),
                        mLogicServer->getCarLinearVelocity(PlayerID::P2));
      mSynch->scSendCoins(mLogicServer->getCoins());
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

SimpleRacer::SimpleRacer(MainWindow *_mainWindow, RenderingWidget *_rendering)
  : mLogicServer(new GameLogic(GameLogic::Type::SERVER)),
    mLogicClient(new GameLogic(GameLogic::Type::CLIENT)),
    mInput(new InputController(mLogicClient)),
    mAI(new ArtificialRacer(PlayerID::P2, mLogicServer)),
    mSynch(new DelaySimulator(mLogicClient, mLogicServer)),
    mRendering(_rendering),
    mMainWindow(_mainWindow)
{
   mLogicServer->setCoinCollectedCallback(&coinCollectedCallback);
   mLogicServer->setCoinSpawnCallback(&coinSpawnedCallback);
   connect(&mGameTimer, &QTimer::timeout, this, &SimpleRacer::update);
}
