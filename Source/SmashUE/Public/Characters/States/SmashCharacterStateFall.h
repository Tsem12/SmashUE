// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterStateFall.generated.h"

class SmashCharacterStateID;

UCLASS(ClassGroup=(SmashCharacterState), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateFall : public USmashCharacterState
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnInputSpecialPressed();
	
	virtual ESmashCharacterStateID GetStateID() override;
	virtual void EnterState(ESmashCharacterStateID PreviousStateID) override;
	virtual void ExitState(ESmashCharacterStateID NextStateID) override;
	virtual void StateTick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	float FallHorizontalMoveSpeed = 400;
	
	UPROPERTY(EditAnywhere)
	float FallAirControl = 1;

	UPROPERTY(EditAnywhere)
	float FallGravityScale = 3;

	UPROPERTY(EditAnywhere)
	float FallFastGravityScale = 10;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> FallAnim; 
};
