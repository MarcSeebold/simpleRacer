#pragma once
#include "Common.hh"

enum class LagProbability : char
{
   LOW = 0,
   MEDIUM = 1,
   HIGH = 2,
   CUSTOM = 3
};

class QJsonObject;

class Settings
{
public:
   /// Singelton getter
   static Settings* the();

   /// Writes all settings to JSON
   void write(QJsonObject &_json) const;

   /// Read all settings from JSON
   void read(const QJsonObject &_json);

   /// Loads a pre-defined condition (1 to 10)
   /// See qrc:/conditions/*.json
   void loadCondition(unsigned int _num);

private: // game logic stuff
   /// Port the game will listen on for commands
   int mNetworkPort = 13337;


   /// Rate at which network updates are sent
   unsigned int mNetworkUpdateRate = 5;

   /// Linear damping of cars
   float mLinearDamping = 2.f;
   /// maximum squared offset before cars are warped instead of smoothly set
   float mMaxOffsetSquared = 10 * 10;
   /// How fast do coins and mud puddles move from right to left?
   float mCoinMudLinearVelocity = -7.85f;
   /// Amount of time coins need to spawn (in seconds)
   float mCoinSpawnTime = 1.f;
   /// Amount of time mud puddles need to spawn (in seconds)
   float mMudSpawnTime = 0.5f;
   /// Car acceleration for x and y axes
   /// @{
   float mCarAccX = 30;
   float mCarAccY = 12;
   /// @}
   /// Points for collecting a coin
   int mScoreCoin = 1;
   /// Points for collecting a mud puddle
   int mScoreMud = -2;

private: // latency stuff
   /// Probabilities that a critical game situations activates lag
   /// @{
   float mLagProbabilityLow = 0.02f;
   float mLagProbabilityMedium = 0.05f;
   float mLagProbabilityHigh = 1.f;
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
   float mClientSideInterpolationFactor = 2.3f;

   /// Singleton instance
   static Settings* instance;

public: // Getter, Setter
   // Getter
   float getLatencyServerToClient() const;
   float getLatencyClientToServer() const;
   GETTER(NetworkUpdateRate);
   GETTER(NetworkPort);
   GETTER(ScoreCoin);
   GETTER(ScoreMud);
   GETTER(CarAccX);
   GETTER(CarAccY);
   GETTER(CoinSpawnTime);
   GETTER(MudSpawnTime);
   GETTER(CoinMudLinearVelocity);
   GETTER(MaxOffsetSquared);
   GETTER(LinearDamping);
   GETTER(ClientSidePhysics);
   GETTER(ShortCircuiting);
   GETTER(ClientSidePrediction);
   GETTER(ClientSideInterpolation);
   GETTER(ServerSideLagCompensation);
   GETTER(ClientSideInterpolationFactor);
   GETTER(LagEnabled);
   GETTER(LagProbabilityLow);
   GETTER(LagProbabilityMedium);
   GETTER(LagProbabilityHigh);
   GETTER(LagProbabilityCustom);
   float getLagProbability() const;

   // Setter
   /// Enable/Disable artificial lag
   /// @{
   void setClientSideCompensation(bool _val);
   void setServerSideCompensation(bool _val);
   /// @}

   /// Set lag probability
   void setLagProbability(LagProbability _probability);
   SETTER(LatencyServerToClient);
   SETTER(LagProbabilityCustom);
   SETTER(LatencyClientToServer);
   void setLagEnabled(bool _val);

   // Properties (Getter AND Setter)
   PROPERTY(LagDuration);

private:
   /// Singleton: private c'tor
   Settings() {}
   /// Singleton: No copy c'tor
   Settings(Settings const&) = delete;
   /// Singleton: No copy assignment
   Settings& operator=(Settings const&) = delete;
};
