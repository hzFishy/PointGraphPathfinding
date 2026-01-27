// By hzFishy - 2026 - Do whatever you want with it.


#include "Asserts/FUAsserts.h"
#include "Core/PGPCoreTypes.h"
#include "Core/PGPGraphSubsystem.h"
#include "Draw/FUDraw.h"
#include "Logging/FULogging.h"
#include "NavMesh/LinkGenerationDebugFlags.h"


/*----------------------------------------------------------------------------
		Link
	----------------------------------------------------------------------------*/
FPGPGraphPointLink::FPGPGraphPointLink() :
	LinkId(0), 
	StartPointId(0),
	LinkedPointId(0)
{}

FPGPGraphPointLink::FPGPGraphPointLink(uint32 InId, uint32 InStartPointId, uint32 InLinkedPointId) :
	LinkId(InId), 
	StartPointId(InStartPointId),
	LinkedPointId(InLinkedPointId)
{}

	
	/*----------------------------------------------------------------------------
		Graph Point
	----------------------------------------------------------------------------*/
FPGPGraphPoint::FPGPGraphPoint(): 
	PointId(0)
{}

FPGPGraphPoint::FPGPGraphPoint(int32 Index): 
	PointId(0)
{}

FPGPGraphPoint::FPGPGraphPoint(uint32 InId, FVector InLocation): 
	PointId(InId), 
	WorldLocation(InLocation)
{}

bool FPGPGraphPoint::IsValid() const
{
	return PointId != 0;
}

FString FPGPGraphPoint::ToString() const
{
	return FString::Printf(TEXT("%i - %s"), 
		PointId,
		*WorldLocation.ToString()
	);
}
	
	/*----------------------------------------------------------------------------
		Network
	----------------------------------------------------------------------------*/
FPGPGraphNetworkGenerateParams::FPGPGraphNetworkGenerateParams()
{}


FPBPDrawDebugGraphNetworkParams::FPBPDrawDebugGraphNetworkParams():
	Time(0),
	PointColor(FColor::Cyan),
	PointRadius(20),
	PointThickness(2),
	PointDepth(0),
	PointTextOffset(FVector(0, 0, 50)),
	PointTextScale(2), 
	LinkColor(FColor::White), 
	LinkArrowSize(20), 
	LinkThickness(2), 
	LinkDepth(0), 
	LinkTextOffset(FVector(0, 0, 50)),
	LinkTextScale(2)
{}

FPGPGraphNetwork::FPGPGraphNetwork()
{}

void FPGPGraphNetwork::Initialize(UPGPGraphSubsystem* GraphSubsystem, const FPGPGraphNetworkGenerateParams& GenerateParams)
{
	/** 
	 * From source points data generate graph points and graph links.
	 * We do that by iterating one by one all Source Points.
	 * Then for each point their links to other Source Points.
	 */
	
	// Generate points
	for (int i = 0; i < GenerateParams.SourcePoints.Num(); ++i)
	{
		auto& SourcePointEntry = GenerateParams.SourcePoints[i];
		
		auto& SourcePointBase = SourcePointEntry.Get<FPGPGraphSourcePoint>();
	
		FPGPGraphPoint NewPoint(GraphSubsystem->GenerateNewPointId(), SourcePointBase.GetWorldLocation());
		NewPoint.SourcePoint = SourcePointEntry;
	
		GraphPointsIndex.Emplace(i, NewPoint.PointId);
		GraphPoints.Emplace(NewPoint.PointId, NewPoint);
	}
	
	// TODO: kinda slow since we iterate everything, maybe generate a temporary TMap for faster lookup
	auto GetGeneratedPoint = [&] (const FPGPGraphSourcePoint& LinkedPoint) -> uint32
	{
		// from a given Linked Source Point, try to find its id from previously generated ids.
		for (auto& Pair : GraphPoints)
		{
			auto& GraphPoint = Pair.Value;
			if (LinkedPoint.IsEqual(GraphPoint.SourcePoint))
			{
				return GraphPoint.PointId;
			}
		}
		checkNoEntry()
		return 0;
	};
	
	// Generate links
	for (auto& SourcePointEntry : GenerateParams.SourcePoints)
	{
		auto& SourcePointBase = SourcePointEntry.Get<FPGPGraphSourcePoint>();
		
		for (auto& LinkedPoint : SourcePointBase.LinkedPoints)
		{
			auto& LinkedPointBase = LinkedPoint.Get<FPGPGraphSourcePoint>();
			
			uint32 StartPointId = GetGeneratedPoint(SourcePointBase);
			uint32 MatchingLinkedPointId = GetGeneratedPoint(LinkedPointBase);
			
			// create link
			FPGPGraphPointLink NewLink(GraphSubsystem->GenerateNewLinkId(), StartPointId, MatchingLinkedPointId);
			
			// add link id to start point
			auto& GraphPoint = GetMutableGraphPoint(StartPointId);
			GraphPoint.LinkIds.Add(NewLink.LinkId);
			
			GraphLinks.Emplace(NewLink.LinkId, NewLink);
		}
	}
}

const FPGPGraphPoint* FPGPGraphNetwork::GetGraphPointPtr(uint32 PointId) const
{
	if (auto* Entry = GraphPoints.Find(PointId))
	{
		return Entry;
	}
	return nullptr;
}

const FPGPGraphPoint& FPGPGraphNetwork::GetGraphPoint(uint32 PointId) const
{
	return GraphPoints[PointId];
}

FPGPGraphPoint& FPGPGraphNetwork::GetMutableGraphPoint(uint32 PointId)
{
	return GraphPoints[PointId];
}

const FPGPGraphPointLink* FPGPGraphNetwork::GetGraphLinkPtr(uint32 LinkId) const
{
	if (auto* Entry = GraphLinks.Find(LinkId))
	{
		return Entry;
	}
	return nullptr;
}

FPGPGraphPoint FPGPGraphNetwork::GetGeneratedGraphPointFromSource(
	const TInstancedStruct<FPGPGraphSourcePoint>& SourcePoint) const
{
	for (auto& Pair : GraphPoints)
	{
		auto& GraphPoint = Pair.Value;
		if (SourcePoint.Get<FPGPGraphSourcePoint>().IsEqual(GraphPoint.SourcePoint))
		{
			return GraphPoint;
		}
	}
	checkNoEntry()
	return 0;
}

FPGPGraphPoint FPGPGraphNetwork::GetGeneratedGraphPointFromIndex(int32 Index) const
{
	if (FU_ENSURE(GraphPointsIndex.Contains(Index)))
	{
		return GraphPoints[GraphPointsIndex[Index]];
	}
	return FPGPGraphPoint();
}

bool FPGPGraphNetwork::IsEmpty() const
{
	return GraphPoints.IsEmpty() && GraphLinks.IsEmpty();
}

void FPGPGraphNetwork::DrawDebug(UWorld* World, const FPBPDrawDebugGraphNetworkParams& DrawDebugParams) const
{
	for (auto& GraphPointPair : GraphPoints)
	{
		auto& GraphPoint = GraphPointPair.Value;
		
		// draw point
		FU::Draw::DrawDebugSphere(
			World,
			GraphPoint.WorldLocation,
			DrawDebugParams.PointRadius,
			DrawDebugParams.PointColor,
			DrawDebugParams.Time,
			DrawDebugParams.PointThickness,
			DrawDebugParams.PointDepth
		);
		
		FU::Draw::DrawDebugString(
			World,
			GraphPoint.WorldLocation + DrawDebugParams.PointTextOffset,
			FString::Printf(TEXT("Id: %i"), GraphPoint.PointId),
			DrawDebugParams.PointColor, 
			DrawDebugParams.Time, 
			DrawDebugParams.PointTextScale
		);
		
		// iterate its links
		for (auto& LinkId : GraphPoint.LinkIds)
		{
			auto& Link = GraphLinks[LinkId];
			auto& LinkedPoint = GraphPoints[Link.LinkedPointId];
			
			FVector StartLocation = GraphPoint.WorldLocation;
			FVector EndLocation = LinkedPoint.WorldLocation;
			FVector ScaledDirection = EndLocation - StartLocation;

			FU::Draw::DrawDebugDirectionalArrow(
				World,
				StartLocation,
				ScaledDirection,
				DrawDebugParams.LinkColor,
				DrawDebugParams.Time,
				DrawDebugParams.LinkArrowSize,
				DrawDebugParams.LinkThickness,
				DrawDebugParams.LinkDepth
			);
			
			const FVector MiddleLocation = StartLocation + ScaledDirection/2;
			
			FU::Draw::DrawDebugString(
				World,
				MiddleLocation + DrawDebugParams.LinkTextOffset,
				FString::Printf(TEXT("Id: %i"), LinkId),
				DrawDebugParams.LinkColor, 
				DrawDebugParams.Time, 
				DrawDebugParams.LinkTextScale
			);
		}
	}
}

FString FPGPGraphNetwork::DebugPrint() const
{
	FString DebugString = ToString();
	
	// TODO: make custom log cat
	FU_LOG_STemp_D("{0}", DebugString);
	return DebugString;
}

FString FPGPGraphNetwork::ToString() const
{
	FU::Utils::FFUMessageBuilder MessageBuilder;
	
	MessageBuilder
		.NewLine("FPGPGraphNetwork")
		.NewLinef(TEXT("Nb points: %i"), GraphPoints.Num())
		.NewLinef(TEXT("Nb links: %i"), GraphLinks.Num())
		.NewLine("----");
	
	for (auto& GraphPointPair : GraphPoints)
	{
		auto& GraphPoint = GraphPointPair.Value;
		
		MessageBuilder.NewLinef(TEXT("Id: %i, Loc: %s, SourceType: %s"),
			GraphPoint.PointId,
			*GraphPoint.WorldLocation.ToString(),
			*FU::Utils::SafeGetName(GraphPoint.SourcePoint.GetScriptStruct())
		);
		if (!GraphPoint.LinkIds.IsEmpty())
		{
			MessageBuilder.NewLine("	Links:");
		
			// iterate its links
			for (auto& LinkId : GraphPoint.LinkIds)
			{
				auto& Link = GraphLinks[LinkId];
				auto& LinkedPoint = GraphPoints[Link.LinkedPointId];
				
				MessageBuilder.NewLinef(TEXT("	- Id: %i, LinkedPointId: %i"), 
					Link.LinkId,
					LinkedPoint.PointId
				);
			}
		}
		else
		{
			MessageBuilder.Append(", No links");
		}
	}
	
	return MessageBuilder.GetMessage();
}


	/*----------------------------------------------------------------------------
		Graph Wrapper
	----------------------------------------------------------------------------*/
FPGPGraphWrapper::FPGPGraphWrapper(const FPGPGraphNetwork* InNet): 
	GraphNetwork(InNet)
{}

bool FPGPGraphWrapper::IsValidRef(FNodeRef NodeRef) const
{
	return NodeRef.IsValid();
}

int32 FPGPGraphWrapper::GetNeighbourCountV2(const FSearchNode& NodeRef) const
{
	return NodeRef.NodeRef.LinkIds.Num();
}

FNodeRef FPGPGraphWrapper::GetNeighbour(const FSearchNode& NodeRef, const int32 NeighbourIndex) const
{
	uint32 LinkId = NodeRef.NodeRef.LinkIds[NeighbourIndex];
	if (auto* Link = GraphNetwork->GetGraphLinkPtr(LinkId))
	{
		// from link id get linked node
		uint32 LinkedPointId = Link->LinkedPointId;
		return GraphNetwork->GetGraphPoint(LinkedPointId);
	}
	
	checkNoEntry()
	return FNodeRef();
}

	
	/*----------------------------------------------------------------------------
		GraphAStar
	----------------------------------------------------------------------------*/
FPGPGraphAStar::FPGPGraphAStar(const FPGPGraphWrapper& InWrapper):
	FGraphAStar(InWrapper)
{}

	
	/*----------------------------------------------------------------------------
		Query Filter
	----------------------------------------------------------------------------*/
FPGPGraphQueryFilter::FPGPGraphQueryFilter(const FPGPGraphWrapper& InWrapper): 
	GraphWrapper(InWrapper)
{}

FVector::FReal FPGPGraphQueryFilter::GetHeuristicScale() const
{
	return 1;
}

FVector::FReal FPGPGraphQueryFilter::GetHeuristicCost(const FSearchNode& NeighbourNode,
	const FSearchNode& EndNode) const
{
	// the cost of travel is the distance between the two points
	// TODO: add multiplier on the link
	return FVector::Distance(NeighbourNode.NodeRef.WorldLocation, EndNode.NodeRef.WorldLocation);
}

FVector::FReal FPGPGraphQueryFilter::GetTraversalCost(const FSearchNode& CurrentNode, const FSearchNode& NeighbourNode) const
{
	return FVector::Distance(CurrentNode.NodeRef.WorldLocation, NeighbourNode.NodeRef.WorldLocation);
}

bool FPGPGraphQueryFilter::IsTraversalAllowed(const FGridNodeRef NodeA, const FGridNodeRef NodeB) const
{
	// TODO: check if link is not negative weight
	return true;
}

FPGPGraphFindPathParams::FPGPGraphFindPathParams() {}

FPBPDrawDebugGraphFindPathResultParams::FPBPDrawDebugGraphFindPathResultParams():
	Time(0),
	StartPointColor(FColor::Purple),
	IntermediatePointColor(FColor::Green),
	EndPointColor(FColor::Blue),
	IncompleteStartPointColor(FColor::Yellow),
	IncompleteIntermediatePointColor(FColor::Orange),
	IncompleteEndPointColor(FColor::Red),
	ErrorPointColor(FColor::Red),
	PointRadius(20),
	PointThickness(2),
	PointDepth(0),
	PointOffset(FVector(0, 0, 70)),
	PointTextOffset(FVector(0, 0, 20)),
	PointTextScale(2), 
	LinkColor(FColor::White),
	LinkArrowSize(500), 
	LinkThickness(2), 
	LinkDepth(0), 
	LinkOffset(FVector(0, 0, 70)),
	LinkTextOffset(FVector(0, 0, 20)),
	LinkTextScale(2)
{}

	
	/*----------------------------------------------------------------------------
		Find Path Result
	----------------------------------------------------------------------------*/
FPGPGraphFindPathResult::FPGPGraphFindPathResult(): 
	Result(EPGPGraphFindPathResult::Error)
{}

FPGPGraphFindPathResult::FPGPGraphFindPathResult(EGraphAStarResult InResult, const TArray<FPGPGraphPoint>& InOutPath)
{
	switch (InResult) 
	{
	case SearchFail:
		{
			Result = EPGPGraphFindPathResult::Error;
			break;
		}
	case SearchSuccess:
		{
			Result = EPGPGraphFindPathResult::Success;
			break;
		}
	case GoalUnreachable:
		{
			Result = InOutPath.IsEmpty() 
				? EPGPGraphFindPathResult::Error : EPGPGraphFindPathResult::SuccessPartialPath;
			break;
		}
	case InfiniteLoop:
		{
			Result = EPGPGraphFindPathResult::Error;
			break;
		}
	}
	
	Path = InOutPath;
}

void FPGPGraphFindPathResult::DrawDebug(UWorld* World, const FPBPDrawDebugGraphFindPathResultParams& DrawDebugParams) const
{
	if (Result == EPGPGraphFindPathResult::Error)
	{
		if (DrawDebugParams.FindPathParams.StartPoint.IsValid())
		{
			FU::Draw::DrawDebugSphere(
				World,
				DrawDebugParams.FindPathParams.StartPoint.WorldLocation + DrawDebugParams.PointOffset,
				DrawDebugParams.PointRadius,
				DrawDebugParams.ErrorPointColor,
				DrawDebugParams.Time,
				DrawDebugParams.PointThickness,
				DrawDebugParams.PointDepth
			);
		}
		if (DrawDebugParams.FindPathParams.EndPoint.IsValid())
		{
			FU::Draw::DrawDebugSphere(
				World,
				DrawDebugParams.FindPathParams.EndPoint.WorldLocation + DrawDebugParams.PointOffset,
				DrawDebugParams.PointRadius,
				DrawDebugParams.ErrorPointColor,
				DrawDebugParams.Time,
				DrawDebugParams.PointThickness,
				DrawDebugParams.PointDepth
			);
		}
	}
	else
	{
		for (int32 i = 0; i < Path.Num(); i++)
		{
			auto& GraphPoint = Path[i];
			
			FColor ColorPoint;
			if (i == 0)
			{
				ColorPoint = Result == EPGPGraphFindPathResult::Success 
					? DrawDebugParams.StartPointColor : DrawDebugParams.IncompleteStartPointColor;
			}
			else if (i == Path.Num() - 1)
			{
				ColorPoint = Result == EPGPGraphFindPathResult::Success 
					? DrawDebugParams.EndPointColor : DrawDebugParams.IncompleteEndPointColor;
			}
			else
			{
				ColorPoint = Result == EPGPGraphFindPathResult::Success 
					? DrawDebugParams.IntermediatePointColor : DrawDebugParams.IncompleteIntermediatePointColor;
			}
		
			// draw point
			FU::Draw::DrawDebugSphere(
				World,
				GraphPoint.WorldLocation + DrawDebugParams.PointOffset,
				DrawDebugParams.PointRadius,
				ColorPoint,
				DrawDebugParams.Time,
				DrawDebugParams.PointThickness,
				DrawDebugParams.PointDepth
			);
		
			FU::Draw::DrawDebugString(
				World,
				GraphPoint.WorldLocation + DrawDebugParams.PointOffset + DrawDebugParams.PointTextOffset,
				FString::Printf(TEXT("[%i] Id: %i"), i, GraphPoint.PointId),
				DrawDebugParams.IntermediatePointColor, 
				DrawDebugParams.Time, 
				DrawDebugParams.PointTextScale
			);
			
			if (i == Path.Num() - 1 && Result == EPGPGraphFindPathResult::SuccessPartialPath)
			{
				FU::Draw::DrawDebugString(
					World,
					GraphPoint.WorldLocation + DrawDebugParams.PointOffset + (DrawDebugParams.PointTextOffset * 2),
					"Find path couldn't find next point from here",
					DrawDebugParams.IncompleteEndPointColor, 
					DrawDebugParams.Time, 
					DrawDebugParams.PointTextScale
				);
			}
		
			// get link between this point and next
			const int32 NextIndex = i + 1;
			if (Path.IsValidIndex(NextIndex))
			{
				auto& NextGraphPoint = Path[NextIndex];
			
				FVector StartLocation = GraphPoint.WorldLocation + DrawDebugParams.LinkOffset;
				FVector EndLocation = NextGraphPoint.WorldLocation + DrawDebugParams.LinkOffset;
				FVector ScaledDirection = EndLocation - StartLocation;

				FU::Draw::DrawDebugDirectionalArrow(
					World,
					StartLocation,
					ScaledDirection,
					DrawDebugParams.LinkColor,
					DrawDebugParams.Time,
					DrawDebugParams.LinkArrowSize,
					DrawDebugParams.LinkThickness,
					DrawDebugParams.LinkDepth
				);
			}
		}
	}
}

FString FPGPGraphFindPathResult::DebugPrint() const
{
	FString DebugString = ToString();
	
	// TODO: make custom log cat
	FU_LOG_STemp_D("{0}", DebugString);
	return DebugString;
}

FString FPGPGraphFindPathResult::ToString() const
{
	FU::Utils::FFUMessageBuilder MessageBuilder;
	
	MessageBuilder
		.NewLine("FPGPGraphFindPathResult")
		.NewLinef(TEXT("Result: %s"), *UEnum::GetValueAsString(Result))
		.NewLinef(TEXT("Nb path points: %i"), Path.Num())
		.NewLine("----");
	
	for (int32 i = 0; i < Path.Num(); i++)
	{
		MessageBuilder.NewLinef(TEXT("    [%i] Id: %i"), i, Path[i].PointId);
	}
	
	return MessageBuilder.GetMessage();
}
