#include "LagSettings.hh"
#include <iostream>
#include <QApplication>
#include "MainWindow.hh"

Settings* Settings::instance = nullptr;

Settings* Settings::the()
{
   if (!instance)
      instance = new Settings;
   return instance;
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
    for (_ const & w : qApp->topLevelWidgets())
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
