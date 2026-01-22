// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core/PGPCoreTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PGPUtilityLibrary.generated.h"


UCLASS(DisplayName="Point Graph Utility Library")
class POINTGRAPHPATHFINDING_API UPGPUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
	/*----------------------------------------------------------------------------
		Network
	----------------------------------------------------------------------------*/
	UFUNCTION(BlueprintCallable, Category="PointGraphPathfinding|Network", meta=(WorldContext="WorldContextObject", DevelopmentOnly))
	static void DrawDebugGraphNetwork(UObject* WorldContextObject, const FPGPGraphNetwork& GraphNetwork, 
		FPBPDrawDebugGraphNetworkParams Params);
	
	UFUNCTION(BlueprintCallable, Category="PointGraphPathfinding|Network", meta=(DevelopmentOnly))
	static FString DebugLogPrintGraphNetwork(const FPGPGraphNetwork& GraphNetwork);
	
	UFUNCTION(BlueprintCallable, Category="PointGraphPathfinding|Network", meta=(DevelopmentOnly))
	static FString GetDebugStringGraphNetwork(const FPGPGraphNetwork& GraphNetwork);
	
	/** 
	 * From a given Graph Network and Source Point get the generated Graph Point
	 */
	UFUNCTION(BlueprintPure, Category="PointGraphPathfinding|Network")
	static FPGPGraphPoint GetGeneratedGraphPointFromSource(const FPGPGraphNetwork& GraphNetwork, 
		TInstancedStruct<FPGPGraphSourcePoint> SourcePoint);
	
	// TODO: generate 2 points + link (one or two way) method
	
	// TODO: link given array of points (one or two way links for ALL for each of them)
	
	
	/*----------------------------------------------------------------------------
		Pathfinding
	----------------------------------------------------------------------------*/
	UFUNCTION(BlueprintCallable, Category="PointGraphPathfinding|Pathfinding", meta=(DevelopmentOnly))
	static FString DebugLogPrintGraphFindPathResult(const FPGPGraphFindPathResult& FindPathResult);
	
	UFUNCTION(BlueprintCallable, Category="PointGraphPathfinding|Pathfinding", meta=(DevelopmentOnly))
	static FString GetDebugStringGraphFindPathResult(const FPGPGraphFindPathResult& FindPathResult);
	
	UFUNCTION(BlueprintCallable, Category="PointGraphPathfinding|Pathfinding", meta=(DevelopmentOnly))
	static void DrawDebugGraphFindPathResult(UObject* WorldContextObject, const FPGPGraphFindPathResult& FindPathResult,
		FPBPDrawDebugGraphFindPathResultParams Params);
};
