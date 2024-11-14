// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsytem.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

void UCameraWorldSubsytem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsytem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag("CameraMain");

	AActor* CameraBoundsActor = FindCameraBounds("CameraBounds");
	if(CameraBoundsActor)
	{
		InitCameraBounds(CameraBoundsActor);
	}
}

void UCameraWorldSubsytem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}

void UCameraWorldSubsytem::AddFollowTarget(AActor* FollowTarget)
{
	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsytem::RemoveFollowTarget(AActor* FollowTarget)
{
	FollowTargets.Remove(FollowTarget);
}

void UCameraWorldSubsytem::TickUpdateCameraPosition(float DeltaTime)
{
	if(!CameraMain) return;
	FVector CameraDestination = CalculateAveragePositionBetweenTargets();
	CameraDestination.Y = CameraMain->GetOwner()->GetActorLocation().Y;
	ClampPositionIntoCameraBounds(CameraDestination);
	CameraMain->GetOwner()->SetActorLocation(CameraDestination);
	
}

FVector UCameraWorldSubsytem::CalculateAveragePositionBetweenTargets()
{
	FVector AveragePosition;
	for (AActor* Target : FollowTargets)
	{
		AveragePosition += Target->GetTransform().GetLocation();
	}
	AveragePosition /= FollowTargets.Num();
	return AveragePosition;
}


UCameraComponent* UCameraWorldSubsytem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> FoundedActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundedActors);
	UCameraComponent* Result = nullptr;
	for (AActor* Actor : FoundedActors)
	{
		Result = Cast<UCameraComponent>(Actor->GetComponentByClass(UCameraComponent::StaticClass()));
		if(Result) return Result;
	}
	return nullptr;
}

#pragma region Bounds
AActor* UCameraWorldSubsytem::FindCameraBounds(FName Tag)
{
	TArray<AActor*> FoundedActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag , FoundedActors);
	if(FoundedActors.Num() > 0)
	{
		return FoundedActors[0];
	}
	return nullptr;
}

void UCameraWorldSubsytem::InitCameraBounds(AActor* CameraBoundsActor)
{
	FVector	BoundsCenter;
	FVector	BoundsExtends;
	CameraBoundsActor->GetActorBounds(false, BoundsCenter, BoundsExtends);
	CameraBoundMin = FVector2D(BoundsCenter.X - BoundsExtends.X, BoundsCenter.Z - BoundsExtends.Z);
	CameraBoundMax = FVector2D(BoundsCenter.X + BoundsExtends.X, BoundsCenter.Z + BoundsExtends.Z);
	CameraBoundYprojectionCenter = BoundsCenter.Y; 
	
}

void UCameraWorldSubsytem::ClampPositionIntoCameraBounds(FVector& Position)
{
	FVector2D ViewportBoundsMin, ViewportBoundsMax;
	GetViewportBounds(ViewportBoundsMin, ViewportBoundsMax);
	FVector WorldBoundMin = CalculateWorldPositionFromViewportPosition(ViewportBoundsMin);
	FVector WorldBoundMax = CalculateWorldPositionFromViewportPosition(ViewportBoundsMax);

	float XRect = (WorldBoundMax.X - WorldBoundMin.X) / 2;
	float ZRect = (WorldBoundMin.Z - WorldBoundMax.Z) / 2;
	
	Position.X = FMath::Clamp(Position.X, CameraBoundMin.X + XRect, CameraBoundMax.X - XRect);
	Position.Z = FMath::Clamp(Position.Z, CameraBoundMin.Y + ZRect, CameraBoundMax.Y - ZRect);
}

void UCameraWorldSubsytem::GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax)
{
	//Find viewport
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if(!ViewportClient) return;

	//Calculate viewport according to Camera aspect ratio and viewport Viewrect
	FViewport* Viewport = ViewportClient->Viewport;
	if(!Viewport) return;

	FIntRect ViewRect (Viewport->GetInitialPositionXY(), Viewport->GetInitialPositionXY() + Viewport->GetSizeXY());
	FIntRect ViewportRect = Viewport->CalculateViewExtents(CameraMain->AspectRatio, ViewRect);

	//Fill output  param with viewport rect
	OutViewportBoundsMin.X = ViewportRect.Min.X;
	OutViewportBoundsMin.Y = ViewportRect.Min.Y;
	
	OutViewportBoundsMax.X = ViewportRect.Max.X;
	OutViewportBoundsMax.Y = ViewportRect.Max.Y;
}

FVector UCameraWorldSubsytem::CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition)
{
	if(!CameraMain) return FVector::Zero();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(!PlayerController) return FVector::Zero();

	float YDistanceToCenter = CameraMain->GetOwner()->GetActorLocation().Y - CameraBoundYprojectionCenter;

	FVector CameraWorldProjectDir;
	FVector WorldPosition;
	UGameplayStatics::DeprojectScreenToWorld(
		PlayerController,
		ViewportPosition,
		WorldPosition,
		CameraWorldProjectDir
		);
	WorldPosition += CameraWorldProjectDir + YDistanceToCenter;

	return WorldPosition;
}
#pragma endregion region 

