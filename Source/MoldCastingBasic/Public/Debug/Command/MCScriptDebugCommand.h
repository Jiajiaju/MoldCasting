// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MCScriptDebugCommand.generated.h"

UCLASS(Abstract, Blueprintable)
class MOLDCASTINGBASIC_API UMCScriptDebugCommand : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug Command")
	FString CommandName {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug Command")
	FString Help {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debug Command")
	void Execute(const TArray<FString>& Arguments, UWorld* InWorld);
	virtual void Execute_Implementation(const TArray<FString>& Arguments, UWorld* InWorld);
};
