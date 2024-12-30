// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterJammusSpecial.generated.h"


class UJammusEnergieBallHolder;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterJammusSpecial : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;
	virtual void InitState(USmashCharacterStateMachine* InStateMachine) override;
	virtual void EnterState(ESmashCharacterStateID PreviousStateID) override;
	virtual void ExitState(ESmashCharacterStateID NextStateID) override;
	virtual void StateTick(float DeltaTime) override;
	
	UFUNCTION()
	void OnSpecialReleased();

protected:
	UPROPERTY()
	TObjectPtr<UJammusEnergieBallHolder> JammusEnergieBallHolder;
	
	UPROPERTY(EditAnywhere)
	float TimeToCharge;
	float CurrentChargeValue;

	UPROPERTY(EditAnywhere)
	float ChargeSpeed;

	UPROPERTY()
	bool IsCharged;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> SpecialAnim;

	UFUNCTION(BlueprintImplementableEvent)
	void TestSpecial();
	
	UFUNCTION(BlueprintImplementableEvent)
	void SpecialTick(float Percentage);

	void ThrowBall();
};
