#include "Camera/JiajiaCameraTypes.h"

FJiajiaCameraCollisionSettings::FJiajiaCameraCollisionSettings()
{
	PenetrationAvoidanceFeelers.Reserve(7);
	PenetrationAvoidanceFeelers.Emplace(FRotator(0.0f, 0.0f, 0.0f), 1.0f, 1.0f, 15.0f);
	PenetrationAvoidanceFeelers.Emplace(FRotator(0.0f, 5.0f, 0.0f), 0.2f, 0.75f, 15.0f);
	PenetrationAvoidanceFeelers.Emplace(FRotator(0.0f, 10.0f, 0.0f), 0.2f, 0.75f, 15.0f);
	PenetrationAvoidanceFeelers.Emplace(FRotator(0.0f, -5.0f, 0.0f), 0.2f, 0.75f, 15.0f);
	PenetrationAvoidanceFeelers.Emplace(FRotator(0.0f, -10.0f, 0.0f), 0.2f, 0.75f, 15.0f);
	PenetrationAvoidanceFeelers.Emplace(FRotator(15.0f, 0.0f, 0.0f), 0.5f, 1.0f, 10.0f);
	PenetrationAvoidanceFeelers.Emplace(FRotator(-10.0f, 0.0f, 0.0f), 0.5f, 0.5f, 10.0f);
}
