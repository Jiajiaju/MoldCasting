#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "JiajiaCameraInputInterface.generated.h"

UINTERFACE(MinimalAPI)
class UJiajiaCameraInputInterface : public UInterface
{
	GENERATED_BODY()
};

class JIAJIACAMERA_API IJiajiaCameraInputInterface
{
	GENERATED_BODY()

public:
	virtual FVector2D GetCameraLookInput() const = 0;
	virtual FVector GetCameraMoveInput() const = 0;
};
