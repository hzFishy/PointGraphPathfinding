// By hzFishy - 2026 - Do whatever you want with it.


#include "Core/PGPGraphNetworkDefinition.h"
#include "Core/PGPGraphSubsystem.h"


void UPGPGraphNetworkDefinition::RecacheGraphNetwork(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (IsValid(World))
	{
		if (auto* GraphSubsystem = World->GetSubsystem<UPGPGraphSubsystem>())
		{
			FPGPGraphNetworkGenerateParams Params;
			Params.SourcePoints = SourcePoints;
			CachedGraphNetwork = GraphSubsystem->GenerateGraphNetwork(Params);
		}
	}
}
