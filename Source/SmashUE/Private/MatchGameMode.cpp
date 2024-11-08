// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameMode.h"
#include "Arena/ArenaPlayerStart.h"
#include "ArenaSettings.h"
#include "SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterInputData.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AArenaPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActorsInArena(PlayerStartsPoints);

	for (AArenaPlayerStart* point : PlayerStartsPoints)
	{
		EAutoReceiveInput::Type InputType = point->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if(SmashCharacterClass == nullptr) continue;
		
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, SmashCharacterClass->GetFName().ToString());
	}
	SpawnCharacters(PlayerStartsPoints);
}

USmashCharacterInputData* AMatchGameMode::LoadInputDataFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	if (!CharacterSettings) return nullptr;
	return CharacterSettings->InputData.LoadSynchronous();
	
}

UInputMappingContext* AMatchGameMode::LoadInputMappingContetFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	if (!CharacterSettings) return nullptr;
	return CharacterSettings->InputMappingContext.LoadSynchronous();
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

void AMatchGameMode::SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints)
{
	USmashCharacterInputData* InputData = LoadInputDataFromConfig();
	UInputMappingContext* InputMappingContext = LoadInputMappingContetFromConfig();
	for (AArenaPlayerStart* SpawnPoint : SpawnPoints)
	{
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if(!SmashCharacterClass) continue;

		ASmashCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ASmashCharacter>(SmashCharacterClass, SpawnPoint->GetTransform());

		if(!NewCharacter) continue;
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
		NewCharacter->InputData = InputData;
		NewCharacter->InputMappingContext = InputMappingContext;
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

		CharacterInsideArena.Add(NewCharacter);
	}
}

TSubclassOf<ASmashCharacter> AMatchGameMode::GetSmashCharacterClassFromInputType(
	EAutoReceiveInput::Type InputType) const
{
	const UArenaSettings* ArenaSettings = GetDefault<UArenaSettings>();
	
	switch (InputType)
	{
		case EAutoReceiveInput::Player0:
			return ArenaSettings->SmashCharacterClassP0;
		case EAutoReceiveInput::Player1:
			return ArenaSettings->SmashCharacterClassP1;
		case EAutoReceiveInput::Player2:
			return ArenaSettings->SmashCharacterClassP2;
		case EAutoReceiveInput::Player3:
			return ArenaSettings->SmashCharacterClassP3;
		default:
			return nullptr;
	}
}
