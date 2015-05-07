#pragma once

namespace lagSettings
{
// Latencies in seconds
static float latencyServerToClient = 500 /*ms*/ / 1000.f;
static float latencyClientToServer = 500 /*ms*/ / 1000.f;
// Client-side
static bool clientSideCompensations = true;
static bool clientSidePhysics = true;
static bool shortCircuiting = false && clientSideCompensations; // TODO: implement
static bool clientSidePrediction = true && clientSideCompensations;
static bool clientSideInterpolation = true && clientSideCompensations;
// Server-side
static bool serverSideLagCompensation = false; // TODO: implement
// Other
/// Factor for applying prediction
static float clientSideInterpolationFactor = 5;
// supress defined but not used warnings
static void func()
{
   (void)&latencyServerToClient;
   (void)&latencyClientToServer;
   (void)&clientSidePhysics;
   (void)&shortCircuiting;
   (void)&clientSidePrediction;
   (void)&latencyServerToClient;
   (void)&serverSideLagCompensation;
   (void)&clientSideInterpolationFactor;
   (void)&clientSideInterpolation;
   (void)&func;
}
}
