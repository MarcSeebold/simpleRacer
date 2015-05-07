#pragma once

namespace lagSettings
{
// Latencies in seconds
static float latencyServerToClient = 500 /*ms*/ / 1000.f;
static float latencyClientToServer = 500 /*ms*/ / 1000.f;
// Client-side
static bool clientSidePhysics = true;
static bool shortCircuiting = true;
static bool clientSideInterpolation = false; // TODO: implement
// Server-side
static bool serverSideLagCompensation = true; // TODO: implement
// Other
/// Time interval for offset corrections in seconds
static float clientSideInterpolationInterval = 1000 /*ms*/ / 1000.f;

// supress defined but not used warnings
static void func()
{
   (void)&latencyServerToClient;
   (void)&latencyClientToServer;
   (void)&clientSidePhysics;
   (void)&shortCircuiting;
   (void)&clientSideInterpolation;
   (void)&latencyServerToClient;
   (void)&serverSideLagCompensation;
   (void)&clientSideInterpolationInterval;
   (void)&func;
}
}
