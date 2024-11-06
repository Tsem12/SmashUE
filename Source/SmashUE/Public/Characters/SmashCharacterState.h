// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmashCharacterStateID.h"
#include "Components/ActorComponent.h"
#include "SmashCharacterState.generated.h"

class SmashCharacterStateID;
class ASmashCharacter;
class USmashCharacterStateMachine;
UCLASS(Abstract)
class SMASHUE_API USmashCharacterState : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashCharacterState();

	virtual ESmashCharacterStateID GetStateID();
	void InitState(USmashCharacterStateMachine* InStateMachine);

	virtual void EnterState(ESmashCharacterStateID PreviousStateID);
	
	virtual void ExitState(ESmashCharacterStateID NextStateID);
	
protected:
	UPROPERTY()
	TObjectPtr<ASmashCharacter> Character;

	UPROPERTY()
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
};
