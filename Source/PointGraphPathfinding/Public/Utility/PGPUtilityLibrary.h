// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core/PGPCoreTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PGPUtilityLibrary.generated.h"


UCLASS(DisplayName="Graph Utility Library")
class POINTGRAPHPATHFINDING_API UPGPUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static void DrawDebugGraphNetwork(UObject* WorldContextObject, const FPGPGraphNetwork& GraphNetwork, FPBPDrawDebugGraphNetworkParams Params);
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static FString DebugLogPrintGraphNetwork(UObject* WorldContextObject, const FPGPGraphNetwork& GraphNetwork);
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static FString GetDebugStringGraphNetwork(UObject* WorldContextObject, const FPGPGraphNetwork& GraphNetwork);
	
	UFUNCTION(BlueprintPure, meta=(WorldContext="WorldContextObject"))
	static FPGPGraphPoint GetGeneratedGraphPointFromSource(UObject* WorldContextObject, const FPGPGraphNetwork& GraphNetwork, TInstancedStruct<FPGPGraphSourcePoint> SourcePoint);
};
