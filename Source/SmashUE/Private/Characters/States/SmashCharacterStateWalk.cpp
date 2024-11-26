// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateWalk.h"

#include "SmashCharacter.h"
#include "Characters/SmashCharacterStateID.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateWalk::GetStateID()
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateWalk::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}

void USmashCharacterStateWalk::OnInputMoveJump(float InputMoveJump)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}


void USmashCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green,TEXT("TickStateWalk"));
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

void USmashCharacterStateWalk::EnterState(ESmashCharacterStateID PreviousStateID)
{
	Super::EnterState(PreviousStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString(TEXT("Enter StateWalk")));
	Character->PlayAnimMontage(WalkAnim);
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	Character->GetCharacterMovement()->MaxWalkSpeed = MoveSpeedMax;
	Character->InputMoveXFastEvent.AddDynamic(this,  &USmashCharacterStateWalk::OnInputMoveXFast);
	Character->InputJumpEvent.AddDynamic(this,  &USmashCharacterStateWalk::OnInputMoveJump);
}

void USmashCharacterStateWalk::ExitState(ESmashCharacterStateID NextStateID)
{
	Super::ExitState(NextStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString(TEXT("Exit StateWalk")));
	Character->InputMoveXFastEvent.RemoveDynamic(this,  &USmashCharacterStateWalk::OnInputMoveXFast);
	Character->InputJumpEvent.RemoveDynamic(this,  &USmashCharacterStateWalk	::OnInputMoveJump);
}
