// Fill out your copyright notice in the Description page of Project Settings.


#include "Service/MCLogService.h"
#include "MCGameInstance.h"

const FAngelscriptBinds::FBind Bind_MCLog([]
{
	auto UMCLogService_ = FAngelscriptBinds::ExistingClass("UMCLogService");

	DECLARE_AS_LOG_METHOD(MCCommon);
	DECLARE_AS_LOG_METHOD(MCUI);
	DECLARE_AS_LOG_METHOD(MCInScreen);
	DECLARE_AS_LOG_METHOD(MCCombat);
	DECLARE_AS_LOG_METHOD(MCItem);
	DECLARE_AS_LOG_METHOD(MCWorld);
	DECLARE_AS_LOG_METHOD(MCCharacter);
});