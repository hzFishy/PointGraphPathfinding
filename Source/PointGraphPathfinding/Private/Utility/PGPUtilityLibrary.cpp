// By hzFishy - 2026 - Do whatever you want with it.


#include "Utility/PGPUtilityLibrary.h"

	
	/*----------------------------------------------------------------------------
		Source
	----------------------------------------------------------------------------*/
void UPGPUtilityLibrary::LinkTwoSourcePoints(TInstancedStruct<FPGPGraphSourcePoint>& SourcePointA, TInstancedStruct<FPGPGraphSourcePoint>& SourcePointB,
	EPGPGraphSourceLinkRule LinkRule)
{
	auto& BasePointA = SourcePointA.GetMutable<FPGPGraphSourcePoint>();
	auto& BasePointB = SourcePointB.GetMutable<FPGPGraphSourcePoint>();
	
	switch (LinkRule)
	{
	case EPGPGraphSourceLinkRule::OneWay:
		{
			BasePointA.LinkedPoints.Emplace(SourcePointB);
			break;
		}
	case EPGPGraphSourceLinkRule::TwoWays:
		{
			BasePointA.LinkedPoints.Emplace(SourcePointB);
			BasePointB.LinkedPoints.Emplace(SourcePointA);
			break;
		}
	}
}

void UPGPUtilityLibrary::LinkSourcePoints(TArray<TInstancedStruct<FPGPGraphSourcePoint>>& SourcePoints)
{
	auto AllEntriesExcept = [&] (int32 ExcludedIndex)
	{
		TArray<TInstancedStruct<FPGPGraphSourcePoint>> Points;
		Points.Reserve(SourcePoints.Num());
		
		for (int32 i = 0; i < SourcePoints.Num(); ++i)
		{
			if (i != ExcludedIndex)
			{
				Points.Emplace(SourcePoints[i]);
			}
		}
		return Points;
	};
	
	// for each point link it with others
	for (int32 i = 0; i < SourcePoints.Num(); ++i)
	{
		auto& SourcePoint = SourcePoints[i];
		auto& BasePoint = SourcePoint.GetMutable<FPGPGraphSourcePoint>();
		BasePoint.LinkedPoints.Append(AllEntriesExcept(i));
	}
}
	

TArray<TInstancedStruct<FPGPGraphSourcePoint>> UPGPUtilityLibrary::CreateSourcePointsFromSceneComponents(const TArray<USceneComponent*>& SceneComponents)
{
	TArray<TInstancedStruct<FPGPGraphSourcePoint>> OutSourcePoints;
	OutSourcePoints.Reserve(SceneComponents.Num());

	for (auto& SceneComponent : SceneComponents)
	{
		OutSourcePoints.Emplace(TInstancedStruct<FPGPGraphSourcePoint>::Make<FPGPGraphSourcePointSceneComponent>(SceneComponent));
	}
	return OutSourcePoints;
}

TArray<TInstancedStruct<FPGPGraphSourcePoint>> UPGPUtilityLibrary::CreateAndLinkSourcePointsFromTwoSceneComponents(
	USceneComponent* SceneComponentA, USceneComponent* SceneComponentB, EPGPGraphSourceLinkRule LinkRule)
{
	auto SourcePoints = CreateSourcePointsFromSceneComponents({SceneComponentA, SceneComponentB});
	
	auto& PointA = SourcePoints[0];
	auto& PointB = SourcePoints[1];
	
	LinkTwoSourcePoints(PointA, PointB, LinkRule);
	
	return SourcePoints;
}

TArray<TInstancedStruct<FPGPGraphSourcePoint>> UPGPUtilityLibrary::CreateAndLinkSourcePointsFromSceneComponents(
	const TArray<USceneComponent*>& SceneComponents)
{
	auto SourcePoints = CreateSourcePointsFromSceneComponents(SceneComponents);
	LinkSourcePoints(SourcePoints);
	return SourcePoints;
}

TArray<TInstancedStruct<FPGPGraphSourcePoint>> UPGPUtilityLibrary::CreateAndLinkSourcePointsFromSceneComponentsWithCustomRules(
	const TArray<FPGPGraphSourceCreateCustomSceneComponent>& CreateCustomSceneComponents)
{
	// used for fast lookup for linking
	TMap<USceneComponent*, int32> SourcePointMap;
	SourcePointMap.Reserve(CreateCustomSceneComponents.Num());
	
	TArray<TInstancedStruct<FPGPGraphSourcePoint>> SourcePoints;
	SourcePoints.Reserve(CreateCustomSceneComponents.Num());
	
	// generate source points from components
	for (auto& RootEntry : CreateCustomSceneComponents)
	{
		auto GenPoint = TInstancedStruct<FPGPGraphSourcePoint>::Make<FPGPGraphSourcePointSceneComponent>(RootEntry.SceneComponent);
		int32 Index = SourcePoints.Emplace(GenPoint);
		SourcePointMap.Emplace(RootEntry.SceneComponent, Index);
	}
	
	
	// setup link rules for each entry
	for (int i = 0; i < CreateCustomSceneComponents.Num(); ++i)
	{
		auto& RootEntry = CreateCustomSceneComponents[i];
		auto& SourcePoint = SourcePoints[i];
		auto& BaseSourcePointRef = SourcePoint.GetMutable<FPGPGraphSourcePoint>();
		
		for (auto& EntryLink : RootEntry.Links)
		{
			// find the generated source point for this component
			auto& SourcePointTarget = SourcePoints[SourcePointMap[EntryLink.SceneComponent]];
		
			// link ourself to the target link
			BaseSourcePointRef.LinkedPoints.Emplace(SourcePointTarget);
		}
	}
	
	return SourcePoints;
}


TArray<TInstancedStruct<FPGPGraphSourcePoint>> UPGPUtilityLibrary::CreateSourcePointsFromLocations(const TArray<FVector>& Locations)
{
	TArray<TInstancedStruct<FPGPGraphSourcePoint>> OutSourcePoints;
	OutSourcePoints.Reserve(Locations.Num());

	for (auto& Location : Locations)
	{
		OutSourcePoints.Emplace(TInstancedStruct<FPGPGraphSourcePoint>::Make<FPGPGraphSourcePointLocation>(Location));
	}
	return OutSourcePoints;
}

TArray<TInstancedStruct<FPGPGraphSourcePoint>> UPGPUtilityLibrary::CreateAndLinkSourcePointsFromTwoLocations(
	FVector LocationA, FVector LocationB, EPGPGraphSourceLinkRule LinkRule)
{
	auto SourcePoints = CreateSourcePointsFromLocations({LocationA, LocationB});
	
	auto& PointA = SourcePoints[0];
	auto& PointB = SourcePoints[1];
	
	LinkTwoSourcePoints(PointA, PointB, LinkRule);
	
	return SourcePoints;
}

TArray<TInstancedStruct<FPGPGraphSourcePoint>> UPGPUtilityLibrary::CreateAndLinkSourcePointsFromLocations(
	const TArray<FVector>& Locations)
{
	auto SourcePoints = CreateSourcePointsFromLocations(Locations);
	LinkSourcePoints(SourcePoints);
	return SourcePoints;
}


TArray<TInstancedStruct<FPGPGraphSourcePoint>> UPGPUtilityLibrary::CreateSourcePointsFromActors(
	const TArray<AActor*>& Actors)
{
	TArray<TInstancedStruct<FPGPGraphSourcePoint>> OutSourcePoints;
	OutSourcePoints.Reserve(Actors.Num());

	for (auto& Actor : Actors)
	{
		OutSourcePoints.Emplace(TInstancedStruct<FPGPGraphSourcePoint>::Make<FPGPGraphSourcePointActor>(Actor));
	}
	return OutSourcePoints;
}

TArray<TInstancedStruct<FPGPGraphSourcePoint>> UPGPUtilityLibrary::CreateAndLinkSourcePointsFromTwoActors(
	AActor* ActorA, AActor* ActorB, EPGPGraphSourceLinkRule LinkRule)
{
	auto SourcePoints = CreateSourcePointsFromActors({ActorA, ActorB});
	
	auto& PointA = SourcePoints[0];
	auto& PointB = SourcePoints[1];
	
	LinkTwoSourcePoints(PointA, PointB, LinkRule);
	
	return SourcePoints;
}

TArray<TInstancedStruct<FPGPGraphSourcePoint>> UPGPUtilityLibrary::CreateAndLinkSourcePointsFromActors(
	const TArray<AActor*>& Actors)
{
	auto SourcePoints = CreateSourcePointsFromActors(Actors);
	LinkSourcePoints(SourcePoints);
	return SourcePoints;
}

	
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

FPGPGraphPoint UPGPUtilityLibrary::GetGeneratedGraphPointFromIndex(const FPGPGraphNetwork& GraphNetwork, int32 Index)
{
	return GraphNetwork.GetGeneratedGraphPointFromIndex(Index);
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
