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
      jLat["LagProbability"] = (int)mLagProbability;
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

#define __SR_SETTINGS_SET_IF_EXIST(jsonObject, key, type) \
   if (jsonObject.contains(#key))                         \
      m##key = jsonObject[#key].toString().to##type();    \
   do                                                     \
   {                                                      \
   } while (0)

#define __SR_SETTINGS_SET_IF_EXIST_BOOL(jsonObject, key) \
   if (jsonObject.contains(#key))                        \
      m##key = jsonObject[#key].toBool();                \
   do                                                    \
   {                                                     \
   } while (0)

void Settings::read(const QJsonObject& _json)
{
   // Game logic stuff
   if (_json.contains("GameLogicStuff"))
   {
      _ jLog = _json.value("GameLogicStuff").toObject();
      __SR_SETTINGS_SET_IF_EXIST(jLog, LinearDamping, Float);
      __SR_SETTINGS_SET_IF_EXIST(jLog, MaxOffsetSquared, Float);
      __SR_SETTINGS_SET_IF_EXIST(jLog, CoinMudLinearVelocity, Float);
      __SR_SETTINGS_SET_IF_EXIST(jLog, CoinSpawnTime, Float);
      __SR_SETTINGS_SET_IF_EXIST(jLog, MudSpawnTime, Float);
      __SR_SETTINGS_SET_IF_EXIST(jLog, CarAccX, Float);
      __SR_SETTINGS_SET_IF_EXIST(jLog, CarAccY, Float);
      __SR_SETTINGS_SET_IF_EXIST(jLog, ScoreCoin, Int);
      __SR_SETTINGS_SET_IF_EXIST(jLog, ScoreMud, Int);
   }
   // Latency stuff
   if (_json.contains("LatencyStuff"))
   {
      _ jLat = _json.value("LatencyStuff").toObject();
      __SR_SETTINGS_SET_IF_EXIST(jLat, LagProbabilityLow, Float);
      __SR_SETTINGS_SET_IF_EXIST(jLat, LagProbabilityMedium, Float);
      __SR_SETTINGS_SET_IF_EXIST(jLat, LagProbabilityHigh, Float);
      __SR_SETTINGS_SET_IF_EXIST(jLat, LagProbabilityCustom, Float);
      if (jLat.contains("LagProbability"))
      { // custom case for LagProbability enum
         int v = jLat["LagProbability"].toInt();
         mLagProbability = (LagProbability)v;
      }
      __SR_SETTINGS_SET_IF_EXIST(jLat, LagDuration, Float);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLat, LagEnabled);
      __SR_SETTINGS_SET_IF_EXIST(jLat, LatencyServerToClient, Float);
      __SR_SETTINGS_SET_IF_EXIST(jLat, LatencyClientToServer, Float);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLat, ClientSidePhysics);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLat, ShortCircuiting);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLat, ClientSidePrediction);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLat, ClientSideInterpolation);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLat, ServerSideLagCompensation);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLat, ClientSideInterpolationFactor);
   }
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
