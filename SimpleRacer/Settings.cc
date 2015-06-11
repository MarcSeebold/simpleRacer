#include "Settings.hh"
#include "MainWindow.hh"
#include <QApplication>
#include <QJsonObject>
#include <iostream>

Settings* Settings::instance = nullptr;

Settings* Settings::the()
{
   if (!instance)
      instance = new Settings;
   return instance;
}

void Settings::write(QJsonObject& _json)
{
   QJsonObject jLat, jLog;
   // Game logic stuff
   {
      jLog["LinearDamping"] = getLinearDamping();
      jLog["MaxOffsetSquared"] = getMaxOffsetSquared();
      jLog["CoinMudLinearVelocity"] = getCoinMudLinearVelocity();
      jLog["CoinSpawnTime"] = getCoinSpawnTime();
      jLog["MudSpawnTime"] = getMudSpawnTime();
      jLog["CarAccX"] = getCarAccX();
      jLog["CarAccY"] = getCarAccY();
      jLog["ScoreCoin"] = getScoreCoin();
      jLog["ScoreMud"] = getScoreMud();
   }
   // Latency stuff
   {
      jLat["LagProbabilityLow"] = getLagProbabilityLow();
      jLat["LagProbabilityMedium"] = getLagProbabilityMedium();
      jLat["LagProbabilityHigh"] = getLagProbabilityHigh();
      jLat["LagProbabilityCustom"] = getLagProbabilityCustom();
      jLat["LagProbability"] = getLagProbability();
      jLat["LagDuration"] = getLagDuration();
      jLat["LagEnabled"] = getLagEnabled();
      jLat["LatencyServerToClient"] = getLatencyServerToClient();
      jLat["LatencyClientToServer"] = getLatencyClientToServer();
      jLat["ClientSidePhysics"] = getClientSidePhysics();
      jLat["ShortCircuiting"] = getShortCircuiting();
      jLat["ClientSidePrediction"] = getClientSidePrediction();
      jLat["ClientSideInterpolation"] = getClientSideInterpolation();
      jLat["ServerSideLagCompensation"] = getServerSideLagCompensation();
      jLat["ClientSideInterpolationFactor"] = getClientSideInterpolationFactor();
   }
   _json["LatencyStuff"] = jLat;
   _json["GameLogicStuff"] = jLog;
}

void Settings::read(const QJsonObject &_json)
{
   SR_ASSERT(0&&"Not implemented.");
   //TODO: implement
}

void Settings::setClientSideCompensation(bool _val)
{
   mClientSidePhysics = _val;
   mShortCircuiting = _val;
   mClientSidePrediction = _val;
   mClientSideInterpolation = _val;
}

void Settings::setServerSideCompensation(bool _val)
{
   mServerSideLagCompensation = _val;
}

void Settings::setLagProbability(LagProbability _probability)
{
   mLagProbability = _probability;
}

float Settings::getLatencyServerToClient() const
{
   if (!getLagEnabled())
      return 0.f;
   return mLatencyServerToClient;
}

float Settings::getLatencyClientToServer() const
{
   if (!getLagEnabled())
      return 0.f;
   return mLatencyClientToServer;
}

void Settings::setLagEnabled(bool _val)
{
   for (_ const& w : qApp->topLevelWidgets())
   {
      _ mainWindow = dynamic_cast<MainWindow*>(w);
      if (mainWindow)
         mainWindow->setLagStatusLabel(_val);
   }

   mLagEnabled = _val;
}

float Settings::getLagProbability() const
{
   switch (mLagProbability)
   {
   case LagProbability::LOW:
      return mLagProbabilityLow;
   case LagProbability::MEDIUM:
      return mLagProbabilityMedium;
   case LagProbability::HIGH:
      return mLagProbabilityHigh;
   case LagProbability::CUSTOM:
      return mLagProbabilityCustom;
   default:
      SR_ASSERT("Unknown lag probability value");
   }
   return -1.f;
}
