// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterStateJump.generated.h"

class SmashCharacterStateID;

UCLASS(ClassGroup=(SmashCharacterState), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateJump : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;
	virtual void EnterState(ESmashCharacterStateID PreviousStateID) override;
	virtual void ExitState(ESmashCharacterStateID NextStateID) override;
	virtual void StateTick(float DeltaTime) override;


protected:

	void MoveHorizontaly();
	
	UPROPERTY(EditAnywhere)
	float JumpWalkSpeed = 400;
	
	UPROPERTY(EditAnywhere)
	float JumpMaxHeight = 280;

	UPROPERTY(EditAnywhere)
	float JumpDuration = 1;

	UPROPERTY(EditAnywhere)
	float JumpAirControl = 1;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> JumpAnim; 
};
