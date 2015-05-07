#pragma once

namespace lagSettings
{
// Latencies in seconds
static float latencyServerToClient = 100/*ms*/ / 1000.f;
static float latencyClientToServer = 100/*ms*/ / 1000.f;
// Client-side
static bool clientSidePhysics = true;
static bool shortCircuiting = true;
static bool clientSideInterpolation = true; // TODO: implement
// Server-side
static bool serverSideLagCompensation = true; // TODO: implement
}
