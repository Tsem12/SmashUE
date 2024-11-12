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

