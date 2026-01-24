// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "Core/PGPCoreTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PGPUtilityLibrary.generated.h"


UENUM(BlueprintType, DisplayName="Graph Source Link Rule")
enum class EPGPGraphSourceLinkRule : uint8
{
	OneWay,
	TwoWays,
};


USTRUCT(BlueprintType)
struct FPGPGraphSourceLinkCustomRule
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType, DisplayName="Graph Source Link Custom Rule Scene Component")
struct FPGPGraphSourceLinkCustomRuleSceneComponent : public FPGPGraphSourceLinkCustomRule
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* SceneComponent;
};


USTRUCT(BlueprintType)
struct FPGPGraphSourceCreateCustom
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType, DisplayName="Graph Source Create Custom Scene Component")
struct FPGPGraphSourceCreateCustomSceneComponent : public FPGPGraphSourceCreateCustom
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* SceneComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FPGPGraphSourceLinkCustomRuleSceneComponent> Links;
};

UCLASS(DisplayName="Point Graph Utility Library")
class POINTGRAPHPATHFINDING_API UPGPUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
	/*----------------------------------------------------------------------------
		Source
	----------------------------------------------------------------------------*/
public:
	/** 
	 *  From given source points link them with given directive.
	 *  For one way link we apply it from point A to point B.
	 */
	UFUNCTION(BlueprintPure, Category="PointGraphPathfinding|Source|Generic")
	static void LinkTwoSourcePoints(UPARAM(ref) TInstancedStruct<FPGPGraphSourcePoint>& SourcePointA,
		UPARAM(ref) TInstancedStruct<FPGPGraphSourcePoint>& SourcePointB, EPGPGraphSourceLinkRule LinkRule);
	
	/** 
	 *  From given source points link them with each other.
	 */
	UFUNCTION(BlueprintPure, Category="PointGraphPathfinding|Source|Generic")
	static void LinkSourcePoints(UPARAM(ref) TArray<TInstancedStruct<FPGPGraphSourcePoint>>& SourcePoints);
	
	
	/** 
	 *  From given scene components create source points.
	 */
	UFUNCTION(BlueprintPure, Category="PointGraphPathfinding|Source|SceneComponent")
	static TArray<TInstancedStruct<FPGPGraphSourcePoint>> CreateSourcePointsFromSceneComponents(const TArray<USceneComponent*>& SceneComponents);
	
	/** 
	 *  From given scene components create source points and link them with given directive.
	 *  For one way link we apply it from point A to point B.
	 */
	UFUNCTION(BlueprintPure, Category="PointGraphPathfinding|Source|SceneComponent")
	static TArray<TInstancedStruct<FPGPGraphSourcePoint>> CreateAndLinkSourcePointsFromTwoSceneComponents(USceneComponent* SceneComponentA,
		USceneComponent* SceneComponentB, EPGPGraphSourceLinkRule LinkRule);
	
	/** 
	 *  From given scene components create source points.
	 *  Linking is done between ALL the points.
	 */
	UFUNCTION(BlueprintPure, Category="PointGraphPathfinding|Source|SceneComponent")
	static TArray<TInstancedStruct<FPGPGraphSourcePoint>> CreateAndLinkSourcePointsFromSceneComponents(const TArray<USceneComponent*>& SceneComponents);
	
	/**
	 *  From given source points and custom link entries link points together.
	 */
	UFUNCTION(BlueprintPure, Category="PointGraphPathfinding|Source|SceneComponent")
	static TArray<TInstancedStruct<FPGPGraphSourcePoint>> CreateAndLinkSourcePointsFromSceneComponentsWithCustomRules(
		const TArray<FPGPGraphSourceCreateCustomSceneComponent>& CreateCustomSceneComponents);
	
	
	/**
	 *  From given locations create source points.
	 */
	UFUNCTION(BlueprintPure, Category="PointGraphPathfinding|Source|Location")
	static TArray<TInstancedStruct<FPGPGraphSourcePoint>> CreateSourcePointsFromLocations(const TArray<FVector>& Locations);
	
	/**
	 *  From given locations create source points and link them with given directive.
	 *  For one way link we apply it from point A to point B.
	 */
	UFUNCTION(BlueprintPure, Category="PointGraphPathfinding|Source|Location")
	static TArray<TInstancedStruct<FPGPGraphSourcePoint>> CreateAndLinkSourcePointsFromTwoLocations(FVector LocationA,
		FVector LocationB, EPGPGraphSourceLinkRule LinkRule);
	
	/**
	 *  From given locations create source points.
	 *  Linking is done between ALL the points.
	 */
	UFUNCTION(BlueprintPure, Category="PointGraphPathfinding|Source|Location")
	static TArray<TInstancedStruct<FPGPGraphSourcePoint>> CreateAndLinkSourcePointsFromLocations(const TArray<FVector>& Locations);
	
	
	/**
	 *  From given actors create source points.
	 */
	UFUNCTION(BlueprintPure, Category="PointGraphPathfinding|Source|Actor")
	static TArray<TInstancedStruct<FPGPGraphSourcePoint>> CreateSourcePointsFromActors(const TArray<AActor*>& Actors);
	
	/**
	 *  From given actors create source points and link them with given directive.
	 *  For one way link we apply it from point A to point B.
	 */
	UFUNCTION(BlueprintPure, Category="PointGraphPathfinding|Source|Actor")
	static TArray<TInstancedStruct<FPGPGraphSourcePoint>> CreateAndLinkSourcePointsFromTwoActors(AActor* ActorA,
		AActor* ActorB, EPGPGraphSourceLinkRule LinkRule);
	
	/**
	 *  From given actors create source points.
	 *  Linking is done between ALL the points.
	 */
	UFUNCTION(BlueprintPure, Category="PointGraphPathfinding|Source|Actor")
	static TArray<TInstancedStruct<FPGPGraphSourcePoint>> CreateAndLinkSourcePointsFromActors(const TArray<AActor*>& Actors);
	
	// TODO: spline version
	
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
	
	/** 
	 * From a given Graph Network and Source Point index get the generated Graph Point
	 */
	UFUNCTION(BlueprintPure, Category="PointGraphPathfinding|Network")
	static FPGPGraphPoint GetGeneratedGraphPointFromIndex(const FPGPGraphNetwork& GraphNetwork, int32 Index);
	
	
	/*----------------------------------------------------------------------------
		Pathfinding
	----------------------------------------------------------------------------*/
	UFUNCTION(BlueprintCallable, Category="PointGraphPathfinding|Pathfinding", meta=(DevelopmentOnly))
	static FString DebugLogPrintGraphFindPathResult(const FPGPGraphFindPathResult& FindPathResult);
	
	UFUNCTION(BlueprintCallable, Category="PointGraphPathfinding|Pathfinding", meta=(DevelopmentOnly))
	static FString GetDebugStringGraphFindPathResult(const FPGPGraphFindPathResult& FindPathResult);
	
	UFUNCTION(BlueprintCallable, Category="PointGraphPathfinding|Pathfinding", meta=(WorldContext="WorldContextObject", DevelopmentOnly))
	static void DrawDebugGraphFindPathResult(UObject* WorldContextObject, const FPGPGraphFindPathResult& FindPathResult,
		FPBPDrawDebugGraphFindPathResultParams Params);
};
