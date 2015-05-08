#include "LagSettings.hh"

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
