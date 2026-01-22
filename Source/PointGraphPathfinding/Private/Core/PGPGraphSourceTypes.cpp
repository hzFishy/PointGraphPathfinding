// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PGPGraphSourceTypes.h"
#include "Asserts/FUAsserts.h"
#include "Core/PGPGraphSubsystem.h"

	
	/*----------------------------------------------------------------------------
		Point - Base
	----------------------------------------------------------------------------*/
FPGPGraphSourcePoint::FPGPGraphSourcePoint()
{}

bool FPGPGraphSourcePoint::IsEqual(const TInstancedStruct<FPGPGraphSourcePoint>& OtherSourcePoint) const
{
	unimplemented();
	return false;
}

FVector FPGPGraphSourcePoint::GetWorldLocation() const
{
	unimplemented();
	return FVector();
}

	
	/*----------------------------------------------------------------------------
		Point - Vector
	----------------------------------------------------------------------------*/
FPGPGraphSourcePointVector::FPGPGraphSourcePointVector():
	Vector(FVector::ZeroVector)
{}

bool FPGPGraphSourcePointVector::IsEqual(const TInstancedStruct<FPGPGraphSourcePoint>& OtherSourcePoint) const
{
	auto& Self = OtherSourcePoint.Get<FPGPGraphSourcePointVector>();
	return Vector == Self.Vector;
}

FVector FPGPGraphSourcePointVector::GetWorldLocation() const
{
	return Vector;
}

	
	/*----------------------------------------------------------------------------
		Point - Scene Component
	----------------------------------------------------------------------------*/
FPGPGraphSourcePointSceneComponent::FPGPGraphSourcePointSceneComponent()
{}

bool FPGPGraphSourcePointSceneComponent::IsEqual(const TInstancedStruct<FPGPGraphSourcePoint>& OtherSourcePoint) const
{
	auto& Self = OtherSourcePoint.Get<FPGPGraphSourcePointSceneComponent>();
	return SceneComponent == Self.SceneComponent;
}

FVector FPGPGraphSourcePointSceneComponent::GetWorldLocation() const
{
	if (FU_ENSURE_WEAKVALID_MSG(SceneComponent, "SceneComponent of FPGPGraphSourcePointSceneComponent is invalid"))
	{
		return SceneComponent->GetComponentLocation();
	}
	return FVector();
}

	
	/*----------------------------------------------------------------------------
		Point - Actor
	----------------------------------------------------------------------------*/
FPGPGraphSourcePointActor::FPGPGraphSourcePointActor()
{}

bool FPGPGraphSourcePointActor::IsEqual(const TInstancedStruct<FPGPGraphSourcePoint>& OtherSourcePoint) const
{
	auto& Self = OtherSourcePoint.Get<FPGPGraphSourcePointActor>();
	return Actor == Self.Actor;
}

FVector FPGPGraphSourcePointActor::GetWorldLocation() const
{
	if (FU_ENSURE_WEAKVALID_MSG(Actor, "Actor of FPGPGraphSourcePointActor is invalid"))
	{
		return Actor->GetActorLocation();
	}
	return FVector();
}
