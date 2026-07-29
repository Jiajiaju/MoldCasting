#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "JiajiaCameraMovementInterface.generated.h"

UINTERFACE(MinimalAPI)
class UJiajiaCameraMovementInterface : public UInterface
{
	GENERATED_BODY()
};

class JIAJIACAMERA_API IJiajiaCameraMovementInterface
{
	GENERATED_BODY()

public:
	virtual FVector GetCameraVelocity() const = 0;
	virtual bool IsCameraFalling() const = 0;
	virtual bool IsCameraStrafing() const = 0;
	virtual bool IsCameraOnGround() const = 0;
	virtual bool IsCameraCrouching() const = 0;
	virtual bool IsCameraStanding() const = 0;
};
