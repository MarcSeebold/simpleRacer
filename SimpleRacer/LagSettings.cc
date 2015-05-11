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
        default:
            SR_ASSERT("Unknown lag probability value");
    }
    return -1.f;
}
