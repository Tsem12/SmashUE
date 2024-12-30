// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmashCharacterStateID.h"
#include "Components/ActorComponent.h"
#include "SmashCharacterState.generated.h"

class SmashCharacterStateID;
class ASmashCharacter;
class USmashCharacterStateMachine;
UCLASS(Abstract, Blueprintable)
class SMASHUE_API USmashCharacterState : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashCharacterState();

	virtual ESmashCharacterStateID GetStateID();
	virtual void InitState(USmashCharacterStateMachine* InStateMachine);

	virtual void EnterState(ESmashCharacterStateID PreviousStateID);
	
	virtual void ExitState(ESmashCharacterStateID NextStateID);

	virtual void StateTick(float DeltaTime);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ASmashCharacter> Character;

	UPROPERTY()
	TObjectPtr<USmashCharacterStateMachine> StateMachine;

	ESmashCharacterStateID PreviousState;
};
