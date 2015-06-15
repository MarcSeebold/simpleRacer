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
      //TODO: implement
   }
   return ok;
}
