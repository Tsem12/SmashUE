// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameMode.h"

#include "Arena/ArenaPlayerStart.h"
#include "Kismet/GameplayStatics.h"


void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AArenaPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActorsInArena(PlayerStartsPoints);

	for (AArenaPlayerStart* point : PlayerStartsPoints)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, point->GetFName().ToString());
	}
}


void AMatchGameMode::FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResulActors)
{
	TArray<AActor*> FoundActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaPlayerStart::StaticClass(), FoundActor);

	for (int i = 0; i < FoundActor.Num(); ++i)
	{
		AArenaPlayerStart* ArenaPlayerStartActor = Cast<AArenaPlayerStart>(FoundActor[i]);
		if(ArenaPlayerStartActor == nullptr) continue;

		ResulActors.Add(ArenaPlayerStartActor);
	}
}
