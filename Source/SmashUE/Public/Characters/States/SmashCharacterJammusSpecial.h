// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterJammusSpecial.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterJammusSpecial : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;
	virtual void EnterState(ESmashCharacterStateID PreviousStateID) override;
	virtual void ExitState(ESmashCharacterStateID NextStateID) override;
	virtual void StateTick(float DeltaTime) override;
	
	UFUNCTION()
	void OnSpecialReleased();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> SpecialAnim; 
};
