// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateJump.h"

#include "SmashCharacter.h"

ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::EnterState(ESmashCharacterStateID PreviousStateID)
{
	Super::EnterState(PreviousStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString(TEXT("Enter StateJump")));
	Character->PlayAnimMontage(JumpAnim);
}


void USmashCharacterStateJump::ExitState(ESmashCharacterStateID NextStateID)
{
	Super::ExitState(NextStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString(TEXT("Exit StateJump")));
}
