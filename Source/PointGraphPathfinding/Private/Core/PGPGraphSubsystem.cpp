// By hzFishy - 2026 - Do whatever you want with it.


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
	
	return FPGPGraphFindPathResult(Result, OutPath);
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
