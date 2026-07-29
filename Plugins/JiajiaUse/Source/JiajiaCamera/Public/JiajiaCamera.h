#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

JIAJIACAMERA_API DECLARE_LOG_CATEGORY_EXTERN(LogJiajiaCamera, Log, All);

class FJiajiaCameraModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
