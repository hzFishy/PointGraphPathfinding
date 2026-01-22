// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "GraphAStar.h"
#include "PGPGraphSourceTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "PGPCoreTypes.generated.h"
class UPGPGraphSubsystem;


/**
 * How it works.
 * 
 * See PGPGraphSourceTypes.h for extra details for the user-side graph generation.
 * Here we have the core types used for points, links and pathfining.
 */


/** 
 *  A link between two points.
 *  A link is a one way link between two points.
 */
USTRUCT(BlueprintType)
struct POINTGRAPHPATHFINDING_API FPGPGraphPointLink
{
	GENERATED_BODY()
	
	FPGPGraphPointLink();
	FPGPGraphPointLink(uint32 InId, uint32 InStartPointId, uint32 InLinkedPointId);
	
	/** 
	 * Unique ID of a link for this world session.
	 */
	uint32 LinkId;
	
	/** The "start" point Id we are linked to */
	uint32 StartPointId;
	
	/** The "end" point Id we are linked to */
	uint32 LinkedPointId;
};

/** 
 *  A point in our graph.
 *  Represents a 3D position in the game world.
 *  A single point can have a infinite number of links.
 */
USTRUCT(BlueprintType)
struct POINTGRAPHPATHFINDING_API FPGPGraphPoint
{
	GENERATED_BODY()
	
	FPGPGraphPoint();
	/** Required by FGraphAStarDefaultNode, not used by this plugin */
	FPGPGraphPoint(int32 Index);
	FPGPGraphPoint(uint32 InId, FVector InLocation);
	
	/** 
	 * Unique ID of a point for this world session.
	 */
	uint32 PointId;
	
	TInstancedStruct<FPGPGraphSourcePoint> SourcePoint;
	
	FVector WorldLocation;
	
	/** 
	 * List of links this point has.
	 */
	TArray<uint32> LinkIds;
	
	bool IsValid() const;
	FString ToString() const;
	
	bool operator==(const FPGPGraphPoint& Other) const
	{
		return PointId == Other.PointId;
	}
	
	friend uint32 GetTypeHash(const FPGPGraphPoint& Self)
	{
		return Self.PointId;
	}
};

USTRUCT(BlueprintType, DisplayName="Graph Network Generate Params")
struct POINTGRAPHPATHFINDING_API FPGPGraphNetworkGenerateParams
{
	GENERATED_BODY()
	
	FPGPGraphNetworkGenerateParams();
	
	UPROPERTY(BlueprintReadWrite)
	TArray<TInstancedStruct<FPGPGraphSourcePoint>> SourcePoints;
};


USTRUCT(BlueprintType, DisplayName="Draw Debug Graph Network Params")
struct FPBPDrawDebugGraphNetworkParams
{
	GENERATED_BODY()
	
	FPBPDrawDebugGraphNetworkParams();
	
	//////////////////////////////////
	// Global
	UPROPERTY(BlueprintReadWrite)
	float Time;
	
	//////////////////////////////////
	// Points
	UPROPERTY(BlueprintReadWrite)
	FColor PointColor;
	
	UPROPERTY(BlueprintReadWrite)
	float PointRadius;
	
	UPROPERTY(BlueprintReadWrite)
	float PointThickness;
	
	UPROPERTY(BlueprintReadWrite)
	float PointDepth;
	
	UPROPERTY(BlueprintReadWrite)
	FVector PointTextOffset;
	
	UPROPERTY(BlueprintReadWrite)
	float PointTextScale;
	
	//////////////////////////////////
	// Links
	UPROPERTY(BlueprintReadWrite)
	FColor LinkColor;
	
	UPROPERTY(BlueprintReadWrite)
	float LinkArrowSize;
	
	UPROPERTY(BlueprintReadWrite)
	float LinkThickness;
	
	UPROPERTY(BlueprintReadWrite)
	float LinkDepth;
	
	UPROPERTY(BlueprintReadWrite)
	FVector LinkTextOffset;
	
	UPROPERTY(BlueprintReadWrite)
	float LinkTextScale;
	
};

/** 
 *  A graph network is a container that holds points and links which can be used for operations such as FindPath.
 */
USTRUCT(BlueprintType, DisplayName="Graph Network")
struct POINTGRAPHPATHFINDING_API FPGPGraphNetwork
{
	GENERATED_BODY()
	
	FPGPGraphNetwork();
	
protected:
	TMap<uint32, FPGPGraphPoint> GraphPoints;
	TMap<uint32, FPGPGraphPointLink> GraphLinks;
	
public:
	void Initialize(UPGPGraphSubsystem* GraphSubsystem, const FPGPGraphNetworkGenerateParams& GenerateParams);
	
	const FPGPGraphPoint* GetGraphPointPtr(uint32 PointId) const;
	const FPGPGraphPoint& GetGraphPoint(uint32 PointId) const;
	FPGPGraphPoint& GetMutableGraphPoint(uint32 PointId);
	const FPGPGraphPointLink* GetGraphLinkPtr(uint32 LinkId) const;

	FPGPGraphPoint GetGeneratedGraphPointFromSource(const TInstancedStruct<FPGPGraphSourcePoint>& SourcePoint) const;
	
	void DrawDebug(UWorld* World, const FPBPDrawDebugGraphNetworkParams& DrawDebugParams) const;
	
	FString DebugPrint() const;
	FString ToString() const;
};


struct FPGPGraphWrapper
{
	FPGPGraphWrapper(const FPGPGraphNetwork* InNet);
	
	using FSearchNode = FGraphAStarDefaultNode<FPGPGraphWrapper>;
	using FNodeRef = FPGPGraphPoint;
	
	const FPGPGraphNetwork* GraphNetwork;
	
	bool IsValidRef(FNodeRef NodeRef) const;
	int32 GetNeighbourCountV2(const FSearchNode& NodeRef) const;
	FNodeRef GetNeighbour(const FSearchNode& NodeRef, const int32 NeighbourIndex) const;
};
using FNodeRef = FPGPGraphPoint;

struct FPGPGraphAStar : public FGraphAStar<FPGPGraphWrapper>
{
	FPGPGraphAStar(const FPGPGraphWrapper& InWrapper);
};

using FSearchNode = FGraphAStarDefaultNode<FPGPGraphWrapper>;
using FGridNodeRef = FNodeRef;

struct FPGPGraphQueryFilter
{
	const FPGPGraphWrapper& GraphWrapper;
	
	FPGPGraphQueryFilter(const FPGPGraphWrapper& InWrapper);

	
	FVector::FReal GetHeuristicScale() const;
	
	/** 
	 * Estimate of cost from StartNode to EndNode from a search node.
	 */
	FVector::FReal GetHeuristicCost(const FSearchNode& NeighbourNode, const FSearchNode& EndNode) const;

	/** 
	 *  Real cost of traveling from StartNode directly to EndNode from a search node.
	 */
	FVector::FReal GetTraversalCost(const FSearchNode& CurrentNode, const FSearchNode& NeighbourNode) const;
	
	/** 
	 * Whether traversing given edge is allowed from a NodeRef
	 */
	bool IsTraversalAllowed(const FGridNodeRef NodeA, const FGridNodeRef NodeB) const;

	/** 
	 * Whether to accept solutions that do not reach the goal
	 */
	// TODO: make it a project settings and/or FindPath param
	bool WantsPartialSolution() const { return true; }
	
	// TODO: make it a project settings and/or FindPath param
	bool ShouldIncludeStartNodeInPath() const { return true; }
};

USTRUCT(BlueprintType, DisplayName="Graph Find Path Params")
struct POINTGRAPHPATHFINDING_API FPGPGraphFindPathParams
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FPGPGraphNetwork GraphNetwork;
	
	/** 
	 *  The point used as the start, MUST be in the Graph Network
	 */
	UPROPERTY(BlueprintReadWrite)
	FPGPGraphPoint StartPoint;
	
	/** 
	 *  The point used as the end, MUST be in the Graph Network
	 */
	UPROPERTY(BlueprintReadWrite)
	FPGPGraphPoint EndPoint;
};

UENUM(DisplayName="Graph Find Path Result")
enum class EPGPGraphFindPathResult
{
	Success,
	Error
};

USTRUCT(BlueprintType, DisplayName="Draw Debug Graph Find Path Result Params")
struct FPBPDrawDebugGraphFindPathResultParams
{
	GENERATED_BODY()
	
	FPBPDrawDebugGraphFindPathResultParams();
	
	//////////////////////////////////
	// Global
	UPROPERTY(BlueprintReadWrite)
	float Time;
	
	//////////////////////////////////
	// Points
	UPROPERTY(BlueprintReadWrite)
	FColor PointColor;
	
	UPROPERTY(BlueprintReadWrite)
	float PointRadius;
	
	UPROPERTY(BlueprintReadWrite)
	float PointThickness;
	
	UPROPERTY(BlueprintReadWrite)
	float PointDepth;
	
	UPROPERTY(BlueprintReadWrite)
	FVector PointOffset;
	
	UPROPERTY(BlueprintReadWrite)
	FVector PointTextOffset;
	
	UPROPERTY(BlueprintReadWrite)
	float PointTextScale;
	
	//////////////////////////////////
	// Links
	UPROPERTY(BlueprintReadWrite)
	FColor LinkColor;
	
	UPROPERTY(BlueprintReadWrite)
	float LinkArrowSize;
	
	UPROPERTY(BlueprintReadWrite)
	float LinkThickness;
	
	UPROPERTY(BlueprintReadWrite)
	float LinkDepth;
	
	UPROPERTY(BlueprintReadWrite)
	FVector LinkOffset;
	
	UPROPERTY(BlueprintReadWrite)
	FVector LinkTextOffset;
	
	UPROPERTY(BlueprintReadWrite)
	float LinkTextScale;
};

USTRUCT(BlueprintType, DisplayName="Graph Find Path Result")
struct POINTGRAPHPATHFINDING_API FPGPGraphFindPathResult
{
	GENERATED_BODY()
	
	FPGPGraphFindPathResult();
	FPGPGraphFindPathResult(EGraphAStarResult InResult, const TArray<FPGPGraphPoint>& InOutPath);
	
	UPROPERTY(BlueprintReadOnly)
	EPGPGraphFindPathResult Result;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FPGPGraphPoint> Path;
	
	void DrawDebug(UWorld* World, const FPBPDrawDebugGraphFindPathResultParams& DrawDebugParams) const;
	
	FString DebugPrint() const;
	
	FString ToString() const;
};
