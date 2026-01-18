// By hzFishy - 2026 - Do whatever you want with it.


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
	LinkedPointId(0)
{}

FPGPGraphPointLink::FPGPGraphPointLink(uint32 InId, uint32 InLinkedPointId) :
	LinkId(InId), 
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
{
}

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
	for (auto& SourcePointEntry : GenerateParams.SourcePoints)
	{
		auto& SourcePointBase = SourcePointEntry.Get<FPGPGraphSourcePoint>();
		
		FPGPGraphPoint NewPoint(GraphSubsystem->GenerateNewPointId(), SourcePointBase.GetWorldLocation());
		NewPoint.SourcePoint = SourcePointEntry;
		
		GraphPoints.Emplace(NewPoint.PointId, NewPoint);
	}
	
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
			FPGPGraphPointLink NewLink(GraphSubsystem->GenerateNewLinkId(), MatchingLinkedPointId);
			
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
			FString::Printf(TEXT("%i"), GraphPoint.PointId),
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
				FString::Printf(TEXT("%i"), LinkId),
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
		.Append("FPGPGraphNetwork")
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

	
	/*----------------------------------------------------------------------------
		Find Path Result
	----------------------------------------------------------------------------*/
FPGPGraphFindPathResult::FPGPGraphFindPathResult(): 
	Result(EPGPGraphFindPathResult::Error)
{}

FPGPGraphFindPathResult::FPGPGraphFindPathResult(EGraphAStarResult InResult)
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
			Result = EPGPGraphFindPathResult::Error;
			break;
		}
	case InfiniteLoop:
		{
			Result = EPGPGraphFindPathResult::Error;
			break;
		}
	}
}
