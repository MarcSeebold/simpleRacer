#pragma once
#include "Common.hh"

enum class LagProbability
{
   LOW,
   MEDIUM,
   HIGH,
   CUSTOM
};

class LagSettings
{
public:
   /// Singelton getter
   static LagSettings* the();

   /// Enable/Disable artificial lag
   /// @{
   void setClientSideCompensation(bool _val);
   void setServerSideCompensation(bool _val);
   /// @}

   /// Set lag probability
   void setLagProbability(LagProbability _probability);

private:
   /// Probabilities that a critical game situations activates lag
   /// @{
   float mLagProbabilityLow = 0.1f;
   float mLagProbabilityMedium = 0.5f;
   float mLagProbabilityHigh = 0.9f;
   float mLagProbabilityCustom = 0.5f;
   /// @}
   /// Current selected probability
   LagProbability mLagProbability = LagProbability::CUSTOM;

   /// How long is the lag activated after triggered (seconds)
   float mLagDuration = 1.0f;

   /// Is lag enabled?
   bool mLagEnabled = false;

   /// Latencies in seconds
   /// @{
   float mLatencyServerToClient = 200 /*ms*/ / 1000.f;
   float mLatencyClientToServer = 200 /*ms*/ / 1000.f;
   /// @}

   // Client
   /// Client-side lag settings
   /// @{
   bool mClientSidePhysics = true;
   bool mShortCircuiting = true;
   bool mClientSidePrediction = true;
   bool mClientSideInterpolation = true;
   /// @}

   // Server
   /// Server-side lag settings
   bool mServerSideLagCompensation = true;
   // Other
   /// Factor for applying prediction
   float mClientSideInterpolationFactor = 3;

   /// Singleton instance
   static LagSettings* instance;

public: // Getter
   float getLatencyServerToClient() const;
   float getLatencyClientToServer() const;
   SETTER(LatencyServerToClient);
   SETTER(LatencyClientToServer);
   GETTER(ClientSidePhysics);
   GETTER(ShortCircuiting);
   GETTER(ClientSidePrediction);
   GETTER(ClientSideInterpolation);
   GETTER(ServerSideLagCompensation);
   GETTER(ClientSideInterpolationFactor);
   PROPERTY(LagDuration);
   GETTER(LagEnabled);
   SETTER(LagProbabilityCustom);
   void setLagEnabled(bool _val);
   float getLagProbability() const;
};
