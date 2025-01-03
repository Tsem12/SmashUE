// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/States/SmashCharacterStateRun.h"
#include "SmashCharacter.h"

#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::OnInputMoveJump(float InputMoveJump)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}


void USmashCharacterStateRun::EnterState(ESmashCharacterStateID PreviousStateID)
{
	Super::EnterState(PreviousStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString(TEXT("Enter StateRun")));
	Character->PlayAnimMontage(RunAnim);
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	Character->GetCharacterMovement()->MaxWalkSpeed = MoveSpeedMax;

	Character->InputJumpEvent.AddDynamic(this,  &USmashCharacterStateRun::OnInputMoveJump);
}

void USmashCharacterStateRun::ExitState(ESmashCharacterStateID NextStateID)
{
	Super::ExitState(NextStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString(TEXT("Exit StateRun")));
	Character->InputJumpEvent.RemoveDynamic(this,  &USmashCharacterStateRun::OnInputMoveJump);
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("TickStateWalk"));

	if(FMath::Abs(Character->GetInputMoveX()) < Character->GetInputMoveXTreshold())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else
	{
		Character->SetOrientX(FMath::Sign(Character->GetInputMoveX()));
		Character->AddMovementInput(Character->GetActorForwardVector(), Character->GetOrientX());
	}

	if(Character->GetCharacterMovement()->IsFalling())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}	
}


