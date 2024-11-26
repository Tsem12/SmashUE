// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterJammusSpecial.h"

#include "SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"


ESmashCharacterStateID USmashCharacterJammusSpecial::GetStateID()
{
	return ESmashCharacterStateID::Special;
}

void USmashCharacterJammusSpecial::EnterState(ESmashCharacterStateID PreviousStateID)
{
	Super::EnterState(PreviousStateID);
	Character->PlayAnimMontage(SpecialAnim);
	Character->InputSpecialReleased.AddDynamic(this, &USmashCharacterJammusSpecial::OnSpecialReleased);
}

void USmashCharacterJammusSpecial::ExitState(ESmashCharacterStateID NextStateID)
{
	Super::ExitState(NextStateID);
	Character->InputSpecialReleased.RemoveDynamic(this, &USmashCharacterJammusSpecial::OnSpecialReleased);
}

void USmashCharacterJammusSpecial::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void USmashCharacterJammusSpecial::OnSpecialReleased()
{
	StateMachine->ChangeState(PreviousState);
}
