// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/PGPUtilityLibrary.h"

	
	/*----------------------------------------------------------------------------
		Network
	----------------------------------------------------------------------------*/
void UPGPUtilityLibrary::DrawDebugGraphNetwork(UObject* WorldContextObject, const FPGPGraphNetwork& GraphNetwork, 
	FPBPDrawDebugGraphNetworkParams Params)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (IsValid(World))
	{
		GraphNetwork.DrawDebug(World, Params);
	}
}

FString UPGPUtilityLibrary::DebugLogPrintGraphNetwork(const FPGPGraphNetwork& GraphNetwork)
{
	return GraphNetwork.DebugPrint();
}

FString UPGPUtilityLibrary::GetDebugStringGraphNetwork(const FPGPGraphNetwork& GraphNetwork)
{
	return GraphNetwork.ToString();
}

FPGPGraphPoint UPGPUtilityLibrary::GetGeneratedGraphPointFromSource(const FPGPGraphNetwork& GraphNetwork, 
	TInstancedStruct<FPGPGraphSourcePoint> SourcePoint)
{
	return GraphNetwork.GetGeneratedGraphPointFromSource(SourcePoint);
}

	
	/*----------------------------------------------------------------------------
		Pathfinding
	----------------------------------------------------------------------------*/
FString UPGPUtilityLibrary::DebugLogPrintGraphFindPathResult(const FPGPGraphFindPathResult& FindPathResult)
{
	return FindPathResult.DebugPrint();
}

FString UPGPUtilityLibrary::GetDebugStringGraphFindPathResult(const FPGPGraphFindPathResult& FindPathResult)
{
	return FindPathResult.ToString();
}

void UPGPUtilityLibrary::DrawDebugGraphFindPathResult(UObject* WorldContextObject,
	const FPGPGraphFindPathResult& FindPathResult, FPBPDrawDebugGraphFindPathResultParams Params)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (IsValid(World))
	{
		FindPathResult.DrawDebug(World, Params);
	}
}
