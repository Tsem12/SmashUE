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
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
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

void USmashCharacterState::EnterState(ESmashCharacterStateID PreviousStateID){}
void USmashCharacterState::ExitState(ESmashCharacterStateID NextStateID){}
void USmashCharacterState::StateTick(float DeltaTime){}


