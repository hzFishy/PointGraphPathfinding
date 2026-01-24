// By hzFishy - 2026 - Do whatever you want with it.


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
		Point - Location
	----------------------------------------------------------------------------*/
FPGPGraphSourcePointLocation::FPGPGraphSourcePointLocation():
	Location(FVector::ZeroVector)
{}

FPGPGraphSourcePointLocation::FPGPGraphSourcePointLocation(FVector InLocation):
	Location(InLocation)
{}

bool FPGPGraphSourcePointLocation::IsEqual(const TInstancedStruct<FPGPGraphSourcePoint>& OtherSourcePoint) const
{
	auto& Self = OtherSourcePoint.Get<FPGPGraphSourcePointLocation>();
	return Location == Self.Location;
}

FVector FPGPGraphSourcePointLocation::GetWorldLocation() const
{
	return Location;
}

	
	/*----------------------------------------------------------------------------
		Point - Scene Component
	----------------------------------------------------------------------------*/
FPGPGraphSourcePointSceneComponent::FPGPGraphSourcePointSceneComponent()
{}

FPGPGraphSourcePointSceneComponent::FPGPGraphSourcePointSceneComponent(USceneComponent* InComponent)
{
	SceneComponent = InComponent;
}

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

FPGPGraphSourcePointActor::FPGPGraphSourcePointActor(AActor* InActor)
{
	Actor = InActor;
}

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
