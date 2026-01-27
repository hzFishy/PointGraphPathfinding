// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "PGPCoreTypes.h"
#include "Engine/DataAsset.h"
#include "StructUtils/InstancedStruct.h"
#include "PGPGraphNetworkDefinition.generated.h"


/**
 * Used to cache graph networks for fast runtime usage.
 * See the "Point Graph Pathfinding" panel with the Selection Mode.
 */
UCLASS(DisplayName="Graph Network Definition")
class POINTGRAPHPATHFINDING_API UPGPGraphNetworkDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 *  Source points used to generate the graph networkd.
	 *  If you edit this you should recalculate a new graph network.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TInstancedStruct<FPGPGraphSourcePoint>> SourcePoints;

	/**
	 *  A cached graph network.
	 */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FPGPGraphNetwork CachedGraphNetwork;

	
	/** 
	 *  Update CachedGraphNetwork from SourcePoints.
	 */
	UFUNCTION(BlueprintCallable, Category="PointGraphPathfinding", meta=(WorldContext="WorldContextObject"))
	void RecacheGraphNetwork(UObject* WorldContextObject);
};
