// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PGPGraphSubsystem.h"
#include "Logging/FULogging.h"

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
bool UPGPGraphSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}
	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
FPGPGraphNetwork UPGPGraphSubsystem::GenerateGraphNetwork(const FPGPGraphNetworkGenerateParams& GenerateParams)
{
	FPGPGraphNetwork NewNetwork;
	NewNetwork.Initialize(this, GenerateParams);
	return NewNetwork;
}

FPGPGraphFindPathResult UPGPGraphSubsystem::FindPath(const FPGPGraphFindPathParams& Params)
{
	FPGPGraphWrapper GraphWrapper(&Params.GraphNetwork);
	
	FPGPGraphAStar PathFinder(GraphWrapper);
	
	FPGPGraphQueryFilter Filter(GraphWrapper);
	
	TArray<FPGPGraphPoint> OutPath;
	EGraphAStarResult Result = PathFinder.FindPath(
		Params.StartPoint, 
		Params.EndPoint, 
		Filter, 
		OutPath
	);
	
	for (int32 i = 0; i < OutPath.Num(); i++)
	{
		FU_LOG_STemp_W("{0}: {1}", i, *OutPath[i].ToString());
	}
	
	return FPGPGraphFindPathResult(Result);
}

uint32 UPGPGraphSubsystem::GenerateNewSourcePointId()
{
	return ++LatestGeneratedSourcePointId;
}

uint32 UPGPGraphSubsystem::GenerateNewPointId()
{
	return ++LatestGeneratedPointId;
}

uint32 UPGPGraphSubsystem::GenerateNewLinkId()
{
	return ++LatestGeneratedLinkId;
}
