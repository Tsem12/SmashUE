// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterState.h"

#include "SmashCharacter.h"
#include "Characters/SmashCharacterStateID.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"


// Sets default values for this component's properties
USmashCharacterState::USmashCharacterState()
{
}

ESmashCharacterStateID USmashCharacterState::GetStateID()
{
	return ESmashCharacterStateID::None;
}

void USmashCharacterState::InitState(USmashCharacterStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;
	Character = StateMachine->GetCharacter();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Magenta, FString::Printf(TEXT("init State %d"), GetStateID()));
}

void USmashCharacterState::EnterState(ESmashCharacterStateID PreviousStateID)
{
	PreviousState = PreviousStateID;
}
void USmashCharacterState::ExitState(ESmashCharacterStateID NextStateID){}
void USmashCharacterState::StateTick(float DeltaTime){}


