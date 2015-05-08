#pragma once
#include "Common.hh"

class LagSettings
{
public:
   static LagSettings* the();
   void setClientSideCompensation(bool _val);
   void setServerSideCompensation(bool _val);

private:
   // Latencies in seconds
   float mLatencyServerToClient = 150 /*ms*/ / 1000.f;
   float mLatencyClientToServer = 150 /*ms*/ / 1000.f;
   // Client-side
   bool mClientSidePhysics = false;
   bool mShortCircuiting = false; // TODO: implement
   bool mClientSidePrediction = false;
   bool mClientSideInterpolation = false;
   // Server-side
   bool mServerSideLagCompensation = false; // TODO: implement
   // Other
   /// Factor for applying prediction
   float mClientSideInterpolationFactor = 5;

   static LagSettings* instance;

public:
   GETTER(LatencyServerToClient);
   GETTER(LatencyClientToServer);
   GETTER(ClientSidePhysics);
   GETTER(ShortCircuiting);
   GETTER(ClientSidePrediction);
   GETTER(ClientSideInterpolation);
   GETTER(ServerSideLagCompensation);
   GETTER(ClientSideInterpolationFactor);
};
