// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StructUtils/InstancedStruct.h"
#include "PGPGraphSourceTypes.generated.h"

/**
 * How it works.
 * 
 * There is many ways a user might want to generate a graph network.
 * This plugin implements a base type that can be used as base for project specific methods.
 * 
 * The core principal is to have at least 2 points (ex: Scene Components, Spline points) 
 * and each point can have an infinite number of links to other points.
 * 
 * Examples of links:
 * - Scene Component A can be connected to another Scene Component B.
 * - Actor (Implicitly Root Component) can be connected to another Scene Component C.
 * - Spline point at index 2 can be connected to another Scene Component D.
 * - Spline point at index 2 can be connected to another Spline point at index 5.
 * 
 * Those "points" and "links" here are called Source Points and Source Links 
 * to differenciate them with Graph Points and Graph Point Links since those are generate with unique ids for fast lookup.
 */

	
	/*----------------------------------------------------------------------------
		Points
	----------------------------------------------------------------------------*/
/**
 * A 3D point in the world represented by an world element (ex: Scene Component, Spline point).
 */
USTRUCT(BlueprintType, DisplayName="Graph Source Point")
struct FPGPGraphSourcePoint
{
	GENERATED_BODY()
	
	FPGPGraphSourcePoint();
	virtual ~FPGPGraphSourcePoint() = default;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<TInstancedStruct<FPGPGraphSourcePoint>> LinkedPoints;
	
	virtual bool IsEqual(const TInstancedStruct<FPGPGraphSourcePoint>& OtherSourcePoint) const;
	virtual FVector GetWorldLocation() const;
};

/** 
 *  Uses a Vector.
 */
USTRUCT(BlueprintType, DisplayName="Graph Source Point Vector")
struct FPGPGraphSourcePointVector : public FPGPGraphSourcePoint
{
	GENERATED_BODY()
	
	FPGPGraphSourcePointVector();
	
	UPROPERTY(BlueprintReadWrite)
	FVector Vector;
	
	virtual bool IsEqual(const TInstancedStruct<FPGPGraphSourcePoint>& OtherSourcePoint) const override;
	
	virtual FVector GetWorldLocation() const override;
};

/** 
 *  Uses a Scene Component.
 */
USTRUCT(BlueprintType, DisplayName="Graph Source Point Scene Component")
struct FPGPGraphSourcePointSceneComponent : public FPGPGraphSourcePoint
{
	GENERATED_BODY()
	
	FPGPGraphSourcePointSceneComponent();
	
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<USceneComponent> SceneComponent;
	
	virtual bool IsEqual(const TInstancedStruct<FPGPGraphSourcePoint>& OtherSourcePoint) const override;
	
	virtual FVector GetWorldLocation() const override;
};

/** 
 *  Uses an Actor root component.
 */
USTRUCT(BlueprintType, DisplayName="Graph Source Point Actor")
struct FPGPGraphSourcePointActor : public FPGPGraphSourcePoint
{
	GENERATED_BODY()
	
	FPGPGraphSourcePointActor();
	
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> Actor;
	
	virtual bool IsEqual(const TInstancedStruct<FPGPGraphSourcePoint>& OtherSourcePoint) const override;
	
	virtual FVector GetWorldLocation() const override;
};
