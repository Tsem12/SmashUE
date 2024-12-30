// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameMode.h"
#include "Arena/ArenaPlayerStart.h"
#include "ArenaSettings.h"
#include "SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterInputData.h"
#include "InputMappingContext.h"
#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	CreateAndInitPlayer();
	

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

float AMatchGameMode::LoadInputTresholdFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	if (!CharacterSettings) return -1;
	return CharacterSettings->InputMoveXThreshold;
}


void AMatchGameMode::FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultActors)
{
	TArray<AActor*> FoundActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaPlayerStart::StaticClass(), FoundActor);

	
	for (int i = 0; i < FoundActor.Num(); ++i)
	{
		AArenaPlayerStart* ArenaPlayerStartActor = Cast<AArenaPlayerStart>(FoundActor[i]);
		if(ArenaPlayerStartActor == nullptr) continue;

		ResultActors.Add(ArenaPlayerStartActor);
	}
}

void AMatchGameMode::SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints)
{
	USmashCharacterInputData* InputData = LoadInputDataFromConfig();
	UInputMappingContext* InputMappingContext = LoadInputMappingContetFromConfig();
	float InputXTreshold = LoadInputTresholdFromConfig();
	for (AArenaPlayerStart* SpawnPoint : SpawnPoints)
	{
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if(!SmashCharacterClass) continue;

		ASmashCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ASmashCharacter>(SmashCharacterClass, SpawnPoint->GetTransform());

		if(!NewCharacter) continue;
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
		NewCharacter->SetInputMoveXTreshold(InputXTreshold);
		NewCharacter->InputData = InputData;
		NewCharacter->InputMappingContext = InputMappingContext;
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

		CharacterInsideArena.Add(NewCharacter);
	}
}

void AMatchGameMode::RespawnCharacter(ASmashCharacter* Character)
{
	Character->SetActorLocation(FindSafestSpawnPoint()->GetActorLocation());
}

AArenaPlayerStart* AMatchGameMode::FindSafestSpawnPoint()
{
	float FarestPlayerDistance = -100;
	AArenaPlayerStart* PlayerStartResult = nullptr;
	for (AArenaPlayerStart* Point : PlayerStartsPoints)
	{
		float Distance = 0;
		for (ASmashCharacter* Character : CharacterInsideArena)
		{
			Distance += FVector::Distance(Point->GetActorLocation(), Character->GetActorLocation());	
		}
		
		if(Distance > FarestPlayerDistance)
		{
			PlayerStartResult = Point;
			FarestPlayerDistance = Distance;
		}
	}
	return PlayerStartResult;
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

void AMatchGameMode::CreateAndInitPlayer() const
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if(!GameInstance) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if(!LocalMultiplayerSubsystem) return;

	LocalMultiplayerSubsystem->CreateAndInitPlayer(ELocalMultiplayerInputMappingType::InGame);
}
