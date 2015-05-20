#include "LagSettings.hh"
#include <iostream>
#include <QApplication>
#include "MainWindow.hh"

LagSettings* LagSettings::instance = nullptr;

LagSettings* LagSettings::the()
{
   if (!instance)
      instance = new LagSettings;
   return instance;
}

void LagSettings::setClientSideCompensation(bool _val)
{
   mClientSidePhysics = _val;
   mShortCircuiting = _val;
   mClientSidePrediction = _val;
   mClientSideInterpolation = _val;
}

void LagSettings::setServerSideCompensation(bool _val)
{
   mServerSideLagCompensation = _val;
}

void LagSettings::setLagProbability(LagProbability _probability)
{
   mLagProbability = _probability;
}

float LagSettings::getLatencyServerToClient() const
{
   if (!getLagEnabled())
      return 0.f;
   return mLatencyServerToClient;
}

float LagSettings::getLatencyClientToServer() const
{
   if (!getLagEnabled())
      return 0.f;
   return mLatencyClientToServer;
}

void LagSettings::setLagEnabled(bool _val)
{
    for (_ const & w : qApp->topLevelWidgets())
    {
        _ mainWindow = dynamic_cast<MainWindow*>(w);
        if (mainWindow)
           mainWindow->setLagStatusLabel(_val);
    }

   mLagEnabled = _val;
}

float LagSettings::getLagProbability() const
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
