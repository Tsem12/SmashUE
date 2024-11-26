// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsytem.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraFollowTarget.h"
#include "Camera/CameraSettings.h"
#include "Kismet/GameplayStatics.h"

void UCameraWorldSubsytem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsytem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraSettings = GetDefault<UCameraSettings>();
	CameraMain = FindCameraByTag(CameraSettings->CameraMainTag);

	AActor* CameraBoundsActor = FindCameraBounds(CameraSettings->CameraBoundsTag);
	if(CameraBoundsActor)
	{
		InitCameraBounds(CameraBoundsActor);
	}

	InitCameraZoomParameters();
}

void UCameraWorldSubsytem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
	TickUpdateCameraZoom(DeltaTime);
}

void UCameraWorldSubsytem::AddFollowTarget(UObject* FollowTarget)
{
	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsytem::RemoveFollowTarget(UObject* FollowTarget)
{
	FollowTargets.Remove(FollowTarget);
}

void UCameraWorldSubsytem::TickUpdateCameraPosition(float DeltaTime)
{
	if(!CameraMain) return;
	FVector CameraDestination = CalculateAveragePositionBetweenTargets();
	CameraDestination.Y = CameraMain->GetOwner()->GetActorLocation().Y;
	ClampPositionIntoCameraBounds(CameraDestination);
	CameraDestination = FMath::Lerp(CameraMain->GetOwner()->GetActorLocation(), CameraDestination, DeltaTime * CameraSettings->PositionDampingFactor);
	CameraMain->GetOwner()->SetActorLocation(CameraDestination);
	
}

FVector UCameraWorldSubsytem::CalculateAveragePositionBetweenTargets()
{
	FVector AveragePosition;
	int FollowableCount = 0;
	for (UObject* Target : FollowTargets)
	{
		ICameraFollowTarget* CamTarget = Cast<ICameraFollowTarget>(Target);
		if(CamTarget && CamTarget->IsFollowable())
		{
			AveragePosition += CamTarget->GetFollowPosition();
			FollowableCount++;
		}
	}
	AveragePosition /= FollowableCount;
	return AveragePosition;
}


void UCameraWorldSubsytem::TickUpdateCameraZoom(float DeltaTime)
{
	if(!CameraMain) return;
	float GreatestDistanceBetweenTargets = CalculateGreatestDistanceBetweenTargets();

	float Percentage = (GreatestDistanceBetweenTargets - CameraSettings->DistanceBetweenTargetsMin) / (CameraZoomDistanceBetweenTargetsMax - CameraSettings->DistanceBetweenTargetsMin);
	Percentage = FMath::Clamp(Percentage, 0, 1);

	FVector NewPos = CameraMain->GetOwner()->GetActorLocation();
	NewPos.Y = FMath::Lerp(CameraZoomYMin, CameraZoomYMax, Percentage);
	NewPos.Y = FMath::Lerp(CameraMain->GetOwner()->GetActorLocation().Y, NewPos.Y, DeltaTime * CameraSettings->SizeDampingFactor);
	CameraMain->GetOwner()->SetActorLocation(NewPos); 
}

float UCameraWorldSubsytem::CalculateGreatestDistanceBetweenTargets()
{
	float GreatestDistance = 0.f;
	int Lenght = FollowTargets.Num();
	for (int i = 0; i < Lenght; ++i)
	{
		ICameraFollowTarget* CurrentTarget = Cast<ICameraFollowTarget>(FollowTargets[i]);
		if(!CurrentTarget) continue;
		for (int j = 0; j < Lenght; ++j)
		{
			if(i == j) continue;
			ICameraFollowTarget* SubTarget =  Cast<ICameraFollowTarget>(FollowTargets[j]);
			if(!SubTarget) continue;
			float Distance = FVector::Dist(SubTarget->GetFollowPosition(), CurrentTarget->GetFollowPosition());
			if(Distance > GreatestDistance)
			{
				GreatestDistance = Distance;
			}
		}
	}
	return GreatestDistance;
}


void UCameraWorldSubsytem::InitCameraZoomParameters()
{
	TArray<AActor*> FoundedCamMin;
	TArray<AActor*> FoundedCamMax;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(CameraSettings->CameraDistanceMinTag) , FoundedCamMin);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(CameraSettings->CameraDistanceMaxTag) , FoundedCamMax);
	if(FoundedCamMin.Num() <= 0 || FoundedCamMax.Num() <= 0)
		return;

	CameraZoomYMax = FoundedCamMax[0]->GetActorLocation().Y;
	CameraZoomYMin = FoundedCamMin[0]->GetActorLocation().Y;
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
	WorldPosition += CameraWorldProjectDir * YDistanceToCenter;

	return WorldPosition;
}

#pragma endregion region 

