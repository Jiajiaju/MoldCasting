#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FJiajiaCombatEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
