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
   float mLatencyServerToClient = 150 /*ms*/ / 1000.f;
   float mLatencyClientToServer = 150 /*ms*/ / 1000.f;
   /// @}

   // Client
   /// Client-side lag settings
   /// @{
   bool mClientSidePhysics = false;
   bool mShortCircuiting = false; // TODO: implement properly
   bool mClientSidePrediction = false;
   bool mClientSideInterpolation = false;
   /// @}

   // Server
   /// Server-side lag settings
   bool mServerSideLagCompensation = false; // TODO: implement
   // Other
   /// Factor for applying prediction
   float mClientSideInterpolationFactor = 9;

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
