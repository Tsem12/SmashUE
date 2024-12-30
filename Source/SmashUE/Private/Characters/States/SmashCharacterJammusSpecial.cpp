// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterJammusSpecial.h"

#include "SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "Characters/Jammus/JammusEnergieBall.h"
#include "Characters/Jammus/JammusEnergieBallHolder.h"


ESmashCharacterStateID USmashCharacterJammusSpecial::GetStateID()
{
	return ESmashCharacterStateID::Special;
}

void USmashCharacterJammusSpecial::InitState(USmashCharacterStateMachine* InStateMachine)
{
	Super::InitState(InStateMachine);
	JammusEnergieBallHolder = Character->GetComponentByClass<UJammusEnergieBallHolder>();
}

void USmashCharacterJammusSpecial::EnterState(ESmashCharacterStateID PreviousStateID)
{
	Super::EnterState(PreviousStateID);
	Character->PlayAnimMontage(SpecialAnim);
	Character->InputSpecialReleased.AddDynamic(this, &USmashCharacterJammusSpecial::OnSpecialReleased);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString(TEXT("Enter State Special")));
	CurrentChargeValue = 0;
	if(!IsCharged)
	{
		JammusEnergieBallHolder->CreateEnergieBall();
	}
}

void USmashCharacterJammusSpecial::ExitState(ESmashCharacterStateID NextStateID)
{
	Super::ExitState(NextStateID);
	Character->InputSpecialReleased.RemoveDynamic(this, &USmashCharacterJammusSpecial::OnSpecialReleased);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString(TEXT("Exit State Special")));

	if(!IsCharged)
	{
		ThrowBall();
	}
}

void USmashCharacterJammusSpecial::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	SpecialTick(DeltaTime);

	CurrentChargeValue += DeltaTime;
	if(!IsCharged)
	{
		JammusEnergieBallHolder->UpdateEnergieBallVisual(CurrentChargeValue / TimeToCharge);
		if(CurrentChargeValue >= TimeToCharge)
		{
			IsCharged = true;
			JammusEnergieBallHolder->JammusEnergieBall->SetEnergieBallVisibility(false);
			StateMachine->ChangeState(ESmashCharacterStateID::Idle);
		}
	}
	else
	{
		if(CurrentChargeValue >= 0.2f)
		{
			StateMachine->ChangeState(PreviousState);
			JammusEnergieBallHolder->JammusEnergieBall->SetEnergieBallVisibility(true);
			ThrowBall();
		}
	}
}

void USmashCharacterJammusSpecial::OnSpecialReleased()
{
	StateMachine->ChangeState(PreviousState);
}

void USmashCharacterJammusSpecial::ThrowBall()
{
	JammusEnergieBallHolder->ReleaseEnergieBall();
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("Pan"));

	float Power = IsCharged? 100 : CurrentChargeValue / TimeToCharge * 100; 
	JammusEnergieBallHolder->JammusEnergieBall->Init(Power, Character->GetOrientX());
	IsCharged = false;
}
