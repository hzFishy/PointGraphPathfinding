// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "Core/PGPCoreTypes.h"
#include "PGPGraphSubsystem.generated.h"


/**
 * This plugin graph sytem works with Points and Links.
 * All Points have an unique Ids and a number of Links.
 * All Links also have unique Ids.
 * Ids are unique across all graph networks.
 * 
 * This system works in two parts:
 * 1. Generating a graph network using a source (raw data, actors, spline, ...).
 * 2. Using the graph network data (for example FindPath).
 */
UCLASS(DisplayName="Point Graph Subsystem")
class POINTGRAPHPATHFINDING_API UPGPGraphSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	
	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
protected:
	uint32 LatestGeneratedSourcePointId;
	uint32 LatestGeneratedPointId;
	uint32 LatestGeneratedLinkId;
	
	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	
	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
public:
	UFUNCTION(BlueprintCallable, Category="PointGraphPathfinding|Network")
	FPGPGraphNetwork GenerateGraphNetwork(const FPGPGraphNetworkGenerateParams& GenerateParams);
	
	UFUNCTION(BlueprintCallable, Category="PointGraphPathfinding|Pathfinding")
	FPGPGraphFindPathResult FindPath(const FPGPGraphFindPathParams& Params);
	
	uint32 GenerateNewSourcePointId();
	
	uint32 GenerateNewPointId();
	
	uint32 GenerateNewLinkId();
};
