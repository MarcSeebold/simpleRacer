#include "Settings.hh"
#include "MainWindow.hh"
#include <QApplication>
#include <QJsonObject>
#include <iostream>
#include <QJsonDocument>
#include <QFile>
#include "SimpleRacer.hh"
#include "SurveyEngine.hh"

Settings* Settings::instance = nullptr;

Settings* Settings::the()
{
   if (!instance)
      instance = new Settings;
   return instance;
}

void Settings::write(QJsonObject& _json) const
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
      jLog["CarVeloY"] = getCarVeloY();
      jLog["AILag"] = getAILag();
      jLog["ScoreCoin"] = getScoreCoin();
      jLog["ScoreMud"] = getScoreMud();
      jLog["NetworkPort"] = getNetworkPort();
      jLog["UserTrainingState"] = (int)getUserTrainingState();
      jLog["MakeSurveyAfterGame"] = getMakeSurveyAfterGame();
      jLog["UserInstruction"] = (int)getUserInstruction();
      jLog["MinTimeBetweenCarPosJumps"] = getMinTimeBetweenCarPosJumps();
      jLog["EnableCarPosSwitching"] = getEnableCarPosSwitching();
      jLog["HandleCoinMudCollisionsOnlyClientside"] = getHandleCoinMudCollisionsOnlyClientside();
      jLog["TestPlay"] = getTestPlay();
      jLog["NetworkUpdateRate"] = (int)getNetworkUpdateRate();
   }
   // Latency stuff
   {
      jLat["LagType"] = (int)getLagType();
      jLat["CarVeloX"] = getCarVeloX();
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
      m##key = jsonObject[#key].to##type();               \
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
      __SR_SETTINGS_SET_IF_EXIST(jLog, LinearDamping, Double);
      __SR_SETTINGS_SET_IF_EXIST(jLog, MaxOffsetSquared, Double);
      __SR_SETTINGS_SET_IF_EXIST(jLog, CoinMudLinearVelocity, Double);
      __SR_SETTINGS_SET_IF_EXIST(jLog, CoinSpawnTime, Double);
      __SR_SETTINGS_SET_IF_EXIST(jLog, MudSpawnTime, Double);
      __SR_SETTINGS_SET_IF_EXIST(jLog, CarAccX, Double);
      __SR_SETTINGS_SET_IF_EXIST(jLog, CarAccY, Double);
      __SR_SETTINGS_SET_IF_EXIST(jLog, CarVeloX, Double);
      __SR_SETTINGS_SET_IF_EXIST(jLog, CarVeloY, Double);
      __SR_SETTINGS_SET_IF_EXIST(jLog, ScoreCoin, Int);
      __SR_SETTINGS_SET_IF_EXIST(jLog, ScoreMud, Int);
      __SR_SETTINGS_SET_IF_EXIST(jLog, NetworkPort, Int);
      if (jLog.contains("UserInstruction"))
      { // custom case for UserInstruction enum
         int v = jLog["UserInstruction"].toInt();
         mUserInstruction = (Instruction)v;
      }
      if (jLog.contains("UserTrainingState"))
      { // custom case for UserTrainingState enum
         int v = jLog["UserTrainingState"].toInt();
         mUserTrainingState = (TrainingState)v;
      }
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLog, MakeSurveyAfterGame);
      __SR_SETTINGS_SET_IF_EXIST(jLog, MinTimeBetweenCarPosJumps, Double);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLog, EnableCarPosSwitching);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLog, HandleCoinMudCollisionsOnlyClientside);
      __SR_SETTINGS_SET_IF_EXIST(jLog, NetworkUpdateRate, Int);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLog, TestPlay);
   }
   // Latency stuff
   if (_json.contains("LatencyStuff"))
   {
      _ jLat = _json.value("LatencyStuff").toObject();
      __SR_SETTINGS_SET_IF_EXIST(jLat, LagProbabilityLow, Double);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLat, AILag);
      __SR_SETTINGS_SET_IF_EXIST(jLat, LagProbabilityMedium, Double);
      __SR_SETTINGS_SET_IF_EXIST(jLat, LagProbabilityHigh, Double);
      __SR_SETTINGS_SET_IF_EXIST(jLat, LagProbabilityCustom, Double);
      if (jLat.contains("LagProbability"))
      { // custom case for LagProbability enum
         int v = jLat["LagProbability"].toInt();
         mLagProbability = (LagProbability)v;
      }
      if (jLat.contains("LagType"))
      { // custom case for LagType enum
         int v = jLat["LagType"].toInt();
         mLagType = (LagType)v;
      }
      __SR_SETTINGS_SET_IF_EXIST(jLat, LagDuration, Double);
      __SR_SETTINGS_SET_IF_EXIST(jLat, LatencyServerToClient, Double);
      __SR_SETTINGS_SET_IF_EXIST(jLat, ClientSideInterpolationFactor, Double);
      __SR_SETTINGS_SET_IF_EXIST(jLat, LatencyClientToServer, Double);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLat, LagEnabled);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLat, ClientSidePhysics);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLat, ShortCircuiting);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLat, ClientSidePrediction);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLat, ClientSideInterpolation);
      __SR_SETTINGS_SET_IF_EXIST_BOOL(jLat, ServerSideLagCompensation);
   }
}

void Settings::loadCondition(unsigned int _num)
{
   QByteArray json;
   // read json from resource
   {
      QString path = ":/conditions/";
      if (_num > 14)
      {
         SR_ASSERT(0 && "Invalid input parameter");
         return;
      }
      path += QString::number(_num) + ".json";
      QFile fh(path);
      if (!fh.open(QFile::ReadOnly))
      {
         SR_ASSERT(0 && "Failed to read json file");
         return;
      }
      json = fh.readAll();
      fh.close();
   }
   _ jDoc = QJsonDocument::fromJson(json);
   _ jRoot = jDoc.object();
   if (!jRoot.contains("LatencyStuff"))
   {
      qDebug() << jRoot;
      SR_ASSERT(0 && "Malformed json conditions file");
      return;
   }
   read(jRoot);
   SimpleRacer::survey()->setLastLoadedCondition(_num);
}

void Settings::setTestPlay(bool _val)
{
   mTestPlay = _val;
   if (_val)
      loadCondition(0);
   emit testPlayStateChange(_val);
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

float Settings::getCurrentLatencyServerToClient() const
{
   if (!getLagEnabled())
      return 0.f;
   return mLatencyServerToClient;
}

float Settings::getCurrentLatencyClientToServer() const
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

bool Settings::getLagEnabled() const
{
   // constant lag = always enabled
   if (getLagType() == LagType::CONSTANT)
      return true;
   return mLagEnabled;
}
