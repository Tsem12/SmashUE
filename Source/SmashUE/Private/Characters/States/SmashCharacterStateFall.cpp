// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateFall.h"

#include "SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


void USmashCharacterStateFall::OnInputSpecialPressed()
{
	StateMachine->ChangeState(ESmashCharacterStateID::Special);
}

ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}

void USmashCharacterStateFall::EnterState(ESmashCharacterStateID PreviousStateID)
{
	Super::EnterState(PreviousStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString(TEXT("Enter StateFall")));
	Character->PlayAnimMontage(FallAnim);
	
	Character->InputSpecialPressed.AddDynamic(this,  &USmashCharacterStateFall::OnInputSpecialPressed);
}


void USmashCharacterStateFall::ExitState(ESmashCharacterStateID NextStateID)
{
	Super::ExitState(NextStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString(TEXT("Exit StateFall")));
	
	Character->InputSpecialPressed.RemoveDynamic(this,  &USmashCharacterStateFall::OnInputSpecialPressed);
}

void USmashCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	if(!Character->GetCharacterMovement()->IsFalling())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
}
