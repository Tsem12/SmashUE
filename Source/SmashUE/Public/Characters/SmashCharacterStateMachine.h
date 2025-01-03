// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SmashCharacterStateMachine.generated.h"

enum class ESmashCharacterStateID : uint8;
class ASmashCharacter;
class USmashCharacterState;
/**
 * 
 */
UCLASS()
class SMASHUE_API USmashCharacterStateMachine : public UObject
{
	GENERATED_BODY()

public:
	void Init(ASmashCharacter* InCharacter);

	void Tick(float DeltaTime);
	ASmashCharacter* GetCharacter() const;

	UFUNCTION(BlueprintCallable)
	void ChangeState(ESmashCharacterStateID NextState);

	USmashCharacterState* GetState(ESmashCharacterStateID StateID);

protected:
	UPROPERTY()
	TObjectPtr<ASmashCharacter> Character;

	UPROPERTY()
	TMap<ESmashCharacterStateID, USmashCharacterState*> AllStates;

	UPROPERTY(BlueprintReadOnly)
	ESmashCharacterStateID CurrentStateID;

	UPROPERTY()
	TObjectPtr<USmashCharacterState> CurrentState;
	
	void CreateState();

	void InitStates();
		
	bool IsStateOverrideByCharacter(ESmashCharacterStateID StateID);
};
