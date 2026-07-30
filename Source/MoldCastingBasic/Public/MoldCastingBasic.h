#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMoldCastingBasicModule : public IModuleInterface
{
public:
	//~ Begin IModuleInterface Interface.
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~ End IModuleInterface Interface.
};
