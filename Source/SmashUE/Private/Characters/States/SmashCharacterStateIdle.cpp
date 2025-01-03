// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateIdle.h"

#include "SmashCharacter.h"
#include "Characters/SmashCharacterStateID.h"
#include "Characters/SmashCharacterStateMachine.h"


ESmashCharacterStateID USmashCharacterStateIdle::GetStateID()
{
	return ESmashCharacterStateID::Idle;
}

void USmashCharacterStateIdle::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}

void USmashCharacterStateIdle::OnInputMoveJump(float InputMoveJump)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}

void USmashCharacterStateIdle::OnInputSpecialPressed()
{
	StateMachine->ChangeState(ESmashCharacterStateID::Special);
}


void USmashCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, TEXT("TickStateIdle"));

	if(FMath::Abs(Character->GetInputMoveX()) > Character->GetInputMoveXTreshold())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
}

void USmashCharacterStateIdle::EnterState(ESmashCharacterStateID PreviousStateID)
{
	Super::EnterState(PreviousStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString(TEXT("Enter StateIdle")));
	Character->PlayAnimMontage(IdleAnim);

	Character->InputMoveXFastEvent.AddDynamic(this,  &USmashCharacterStateIdle::OnInputMoveXFast);
	Character->InputJumpEvent.AddDynamic(this,  &USmashCharacterStateIdle::OnInputMoveJump);
	Character->InputSpecialPressed.AddDynamic(this,  &USmashCharacterStateIdle::OnInputSpecialPressed);
}

void USmashCharacterStateIdle::ExitState(ESmashCharacterStateID NextStateID)
{
	Super::ExitState(NextStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString(TEXT("Exit StateIdle")));

	Character->InputMoveXFastEvent.RemoveDynamic(this,  &USmashCharacterStateIdle::OnInputMoveXFast);
	Character->InputJumpEvent.RemoveDynamic(this,  &USmashCharacterStateIdle::OnInputMoveJump);
	Character->InputSpecialPressed.RemoveDynamic(this,  &USmashCharacterStateIdle::OnInputSpecialPressed);
}

