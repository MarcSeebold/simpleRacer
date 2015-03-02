#include "SimpleRacer.hh"
#include "Common.hh"

SimpleRacer *SimpleRacer::sInstance = nullptr;

void SimpleRacer::create()
{
   SR_ASSERT(!sInstance && "create() already called.");
   sInstance = new SimpleRacer();
}

void SimpleRacer::destroy()
{
   SR_ASSERT(sInstance && "destroy called without create or twice");
   delete sInstance; sInstance=nullptr;
}

SimpleRacer *SimpleRacer::the()
{
   SR_ASSERT(sInstance && "the() called, but instance is null");
   return sInstance;
}

void SimpleRacer::startGame()
{
   logicServer()->start();
   logicClient()->start();
   mRunning = true;
}

void SimpleRacer::exitGame()
{
   logicClient()->reset();
   logicServer()->reset();
   mRunning = false;
   exit(EXIT_SUCCESS);
}

SimpleRacer::SimpleRacer():
   mLogicServer(new GameLogic()),
   mLogicClient(new GameLogic()),
   mRendering(new RenderingWidget()),
   mInput(new InputController(mLogicClient))
{
}


#if 0

void MainWindow::performGameUpdateStep()
{
   // process input
   processInput();
   mInput->update();
   // game logic
   mGameLogic->update(SR_RESOLUTION);
   // rendering
   update();
   // update score
   _ p1Score = mGameLogic->getScore(PlayerID::P1);
   _ p2Score = mGameLogic->getScore(PlayerID::P2);
   mUI->labelP1Points->setText(QString::number(p1Score));
   mUI->labelP2Points->setText(QString::number(p2Score));
}
#endif
