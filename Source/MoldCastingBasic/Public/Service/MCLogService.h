// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AngelscriptBinds.h"
#include "Engine/Engine.h"
#include "MCBaseService.h"
#include "MCLogService.generated.h"

// Declare Macro ********************************************************************************
#define DECLARE_NATIVE_LOG_METHOD(CategoryName) \
template <typename... ArgTypes> \
void NativeLog##CategoryName##Info(UE::Core::TCheckedFormatString<TCHAR, ArgTypes...> Format, ArgTypes... Args) \
{ \
FString Message = FString::Printf(Format, Args...); \
UE_LOG(CategoryName, Display, TEXT("%s"), *Message); \
} \
\
template <typename... ArgTypes> \
void NativeLog##CategoryName##Warn(UE::Core::TCheckedFormatString<TCHAR, ArgTypes...> Format, ArgTypes... Args) \
{ \
FString Message = FString::Printf(Format, Args...); \
UE_LOG(CategoryName, Warning, TEXT("%s"), *Message); \
} \
\
template <typename... ArgTypes> \
void NativeLog##CategoryName##Error(UE::Core::TCheckedFormatString<TCHAR, ArgTypes...> Format, ArgTypes... Args) \
{ \
FString Message = FString::Printf(Format, Args...); \
UE_LOG(CategoryName, Error, TEXT("%s"), *Message); \
} \
\

#define DECLARE_AS_LOG_METHOD(CategoryName) \
UMCLogService_.Method("void Log"#CategoryName"Info(const FString& Message)", [](const UMCLogService* LogService, const FString& Message) -> void \
{ \
UE_LOG(CategoryName, Display, TEXT("%s"), *Message); \
}); \
\
UMCLogService_.Method("void Log"#CategoryName"Warn(const FString& Message)", [](const UMCLogService* LogService, const FString& Message) -> void \
{ \
UE_LOG(CategoryName, Warning, TEXT("%s"), *Message); \
}); \
\
UMCLogService_.Method("void Log"#CategoryName"Error(const FString& Message)", [](const UMCLogService* LogService, const FString& Message) -> void \
{ \
UE_LOG(CategoryName, Error, TEXT("%s"), *Message); \
}); \
\
// *******************************************************************************

DECLARE_LOG_CATEGORY_CLASS(MCCommon,	All, All);
DECLARE_LOG_CATEGORY_CLASS(MCUI,		All, All);
DECLARE_LOG_CATEGORY_CLASS(MCInScreen,	All, All);
DECLARE_LOG_CATEGORY_CLASS(MCCombat,	All, All);
DECLARE_LOG_CATEGORY_CLASS(MCItem,		All, All);
DECLARE_LOG_CATEGORY_CLASS(MCWorld,		All, All);
DECLARE_LOG_CATEGORY_CLASS(MCCharacter,	All, All);

UCLASS()
class MOLDCASTINGBASIC_API UMCLogService : public UMCBaseService
{
	GENERATED_BODY()
public:
	DECLARE_NATIVE_LOG_METHOD(MCCommon);
	DECLARE_NATIVE_LOG_METHOD(MCUI);
	DECLARE_NATIVE_LOG_METHOD(MCInScreen);
	DECLARE_NATIVE_LOG_METHOD(MCCombat);
	DECLARE_NATIVE_LOG_METHOD(MCItem);
	DECLARE_NATIVE_LOG_METHOD(MCWorld);
	DECLARE_NATIVE_LOG_METHOD(MCCharacter);

	void PrintInScreen(int32 InKey, const FString& Message, const FColor& Color = FColor::MakeRandomColor())
	{
		NativeLogMCInScreenInfo(TEXT("%s"), *Message);
		if (IsValid(GEngine)) GEngine->AddOnScreenDebugMessage(InKey, 5.0f, Color, Message);
	}

	void PrintInScreen(int32 InKey, const FString& Message, float Value, const FColor& Color = FColor::MakeRandomColor())
	{
		FString PrintMessage = FString::Printf(TEXT("%s : %s"), *Message, *(FString::SanitizeFloat(Value)));
		NativeLogMCInScreenInfo(TEXT("%s"), *PrintMessage);
		if (IsValid(GEngine)) GEngine->AddOnScreenDebugMessage(InKey, 5.0f, Color, PrintMessage);
	}
};
