// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameMode.generated.h"

class UInputMappingContext;
class USmashCharacterInputData;
class ASmashCharacter;
class AArenaPlayerStart;
/**
 * 
 */
UCLASS()
class SMASHUE_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
protected:
	UPROPERTY()
	TArray<ASmashCharacter*> CharacterInsideArena;
	
	UPROPERTY()
	TArray<AArenaPlayerStart*> PlayerStartsPoints;

private:
	USmashCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContetFromConfig();
	float LoadInputTresholdFromConfig();

	void FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultActors);

	void SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints);

	UFUNCTION(BlueprintCallable)
	void RespawnCharacter(ASmashCharacter* Character);

	AArenaPlayerStart* FindSafestSpawnPoint();
	
	TSubclassOf<ASmashCharacter> GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const;

	void CreateAndInitPlayer() const;
};
