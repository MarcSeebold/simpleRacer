#include "Testing.hh"
#include "Settings.hh"
#include <QJsonObject>

Testing::Testing()
{
}

bool Testing::runTestSettingsSaveLoad()
{
   // get instance
   _ s = Settings::the();
   // make copy of values
   // game logic stuff
   _ linearDamping = s->getLinearDamping();
   _ maxOffsetSquared = s->getMaxOffsetSquared();
   _ coinMudLinearVelocity = s->getCoinMudLinearVelocity();
   _ coinSpawnTime = s->getCoinSpawnTime();
   _ mudSpawnTime = s->getMudSpawnTime();
   _ carAccX = s->getCarAccX();
   _ carAccY = s->getCarAccY();
   _ scoreCoin = s->getScoreCoin();
   _ scoreMud = s->getScoreMud();
   _ networkPort = s->getNetworkPort();
   _ testPlay = s->getTestPlay();
   _ networkUpdateRate = s->getNetworkUpdateRate();
   // latency stuff
   _ lagProbabilityLow = s->getLagProbabilityLow();
   _ lagProbabilityMedium = s->getLagProbabilityMedium();
   _ lagProbabilityHigh = s->getLagProbabilityHigh();
   _ lagProbabilityCustom = s->getLagProbabilityCustom();
   _ lagProbability = s->getLagProbability();
   _ lagDuration = s->getLagDuration();
   _ lagEnabled = s->getLagEnabled();
   _ latencyServerToClient = s->getLatencyServerToClient();
   _ latencyClientToServer = s->getLatencyClientToServer();
   _ clientSidePhysics = s->getClientSidePhysics();
   _ shortCircuiting = s->getShortCircuiting();
   _ clientSidePrediction = s->getClientSidePrediction();
   _ clientSideInterpolation = s->getClientSideInterpolation();
   _ serverSideLagCompensation = s->getServerSideLagCompensation();
   _ clientSideInterpolationFactor = s->getClientSideInterpolationFactor();
   // store as json
   QJsonObject json;
   s->write(json);
   // load from json
   s->read(json);
   // make sure data is correct
   bool ok = true;
   {
      // game logic stuff
      ok &= linearDamping == s->getLinearDamping();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= maxOffsetSquared == s->getMaxOffsetSquared();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= coinMudLinearVelocity == s->getCoinMudLinearVelocity();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= coinSpawnTime == s->getCoinSpawnTime();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= mudSpawnTime == s->getMudSpawnTime();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= carAccX == s->getCarAccX();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= carAccY == s->getCarAccY();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= scoreCoin == s->getScoreCoin();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= scoreMud == s->getScoreMud();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= networkPort == s->getNetworkPort();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= networkUpdateRate == s->getNetworkUpdateRate();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= testPlay == s->getTestPlay();
      SR_ASSERT(ok && "Test: Value missmatch");
      // latency stuff
      ok &= lagProbabilityLow == s->getLagProbabilityLow();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= lagProbabilityMedium == s->getLagProbabilityMedium();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= lagProbabilityHigh == s->getLagProbabilityHigh();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= lagProbabilityCustom == s->getLagProbabilityCustom();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= lagProbability == s->getLagProbability();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= lagDuration == s->getLagDuration();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= lagEnabled == s->getLagEnabled();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= latencyServerToClient == s->getLatencyServerToClient();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= latencyClientToServer == s->getLatencyClientToServer();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= clientSidePhysics == s->getClientSidePhysics();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= shortCircuiting == s->getShortCircuiting();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= clientSidePrediction == s->getClientSidePrediction();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= clientSideInterpolation == s->getClientSideInterpolation();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= serverSideLagCompensation == s->getServerSideLagCompensation();
      SR_ASSERT(ok && "Test: Value missmatch");
      ok &= clientSideInterpolationFactor == s->getClientSideInterpolationFactor();
      SR_ASSERT(ok && "Test: Value missmatch");
   }
   return ok;
}
