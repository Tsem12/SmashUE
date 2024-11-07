// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateWalk.h"

#include "SmashCharacter.h"
#include "Characters/SmashCharacterStateID.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateWalk::GetStateID()
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateWalk::ExitState(ESmashCharacterStateID NextStateID)
{
	Super::ExitState(NextStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString(TEXT("Exit StateWalk")));
}

void USmashCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	Character->AddMovementInput(Character->GetActorForwardVector(), 1);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("TickStateWalk"));
}

void USmashCharacterStateWalk::EnterState(ESmashCharacterStateID PreviousStateID)
{
	Super::EnterState(PreviousStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString(TEXT("Enter StateWalk")));
	Character->PlayAnimMontage(WalkAnim);
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	Character->GetCharacterMovement()->MaxWalkSpeed = MoveSpeedMax;
}
