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
	static void DrawDebugGraphNetwork(UObject* WorldContextObject, const FPGPGraphNetwork& GraphNetwork, 
		FPBPDrawDebugGraphNetworkParams Params);
	
	UFUNCTION(BlueprintCallable)
	static FString DebugLogPrintGraphNetwork(const FPGPGraphNetwork& GraphNetwork);
	
	UFUNCTION(BlueprintCallable)
	static FString GetDebugStringGraphNetwork(const FPGPGraphNetwork& GraphNetwork);
	
	UFUNCTION(BlueprintCallable)
	static FString DebugLogPrintGraphFindPathResult(const FPGPGraphFindPathResult& FindPathResult);
	
	UFUNCTION(BlueprintCallable)
	static FString GetDebugStringGraphFindPathResult(const FPGPGraphFindPathResult& FindPathResult);
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static void DrawDebugGraphFindPathResult(UObject* WorldContextObject, const FPGPGraphFindPathResult& FindPathResult,
		FPBPDrawDebugGraphFindPathResultParams Params);
	
	UFUNCTION(BlueprintPure)
	static FPGPGraphPoint GetGeneratedGraphPointFromSource(const FPGPGraphNetwork& GraphNetwork, 
		TInstancedStruct<FPGPGraphSourcePoint> SourcePoint);
};
