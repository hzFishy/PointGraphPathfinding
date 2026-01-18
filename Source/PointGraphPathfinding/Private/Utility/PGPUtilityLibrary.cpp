// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/PGPUtilityLibrary.h"


void UPGPUtilityLibrary::DrawDebugGraphNetwork(UObject* WorldContextObject, const FPGPGraphNetwork& GraphNetwork, 
	FPBPDrawDebugGraphNetworkParams Params)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (IsValid(World))
	{
		GraphNetwork.DrawDebug(World, Params);
	}
}

FString UPGPUtilityLibrary::DebugLogPrintGraphNetwork(UObject* WorldContextObject, const FPGPGraphNetwork& GraphNetwork)
{
	return GraphNetwork.DebugPrint();
}

FString UPGPUtilityLibrary::GetDebugStringGraphNetwork(UObject* WorldContextObject, const FPGPGraphNetwork& GraphNetwork)
{
	return GraphNetwork.ToString();
}

FPGPGraphPoint UPGPUtilityLibrary::GetGeneratedGraphPointFromSource(UObject* WorldContextObject,
	const FPGPGraphNetwork& GraphNetwork, TInstancedStruct<FPGPGraphSourcePoint> SourcePoint)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (IsValid(World))
	{
		return GraphNetwork.GetGeneratedGraphPointFromSource(SourcePoint);
	}
	return FPGPGraphPoint();
}
