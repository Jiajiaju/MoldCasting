UCLASS()
class UMCASDebugPingCommand : UMCScriptDebugCommand
{
	default CommandName = "mc.Debug.Ping";
	default Help = "Verify that AngelScript debug console commands are registered.";

	UFUNCTION(BlueprintOverride)
	void Execute(const TArray<FString>& Arguments, UWorld InWorld)
	{
		Log("mc.Debug.Ping: Pong");
	}
}
