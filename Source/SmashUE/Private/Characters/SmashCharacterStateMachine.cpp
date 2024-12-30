// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateMachine.h"

#include "SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterState.h"
#include "Characters/SmashCharacterStateID.h"

void USmashCharacterStateMachine::Init(ASmashCharacter* InCharacter)
{
	Character = InCharacter;
	CreateState();
	InitStates();

	ChangeState(ESmashCharacterStateID::Idle);
}

void USmashCharacterStateMachine::Tick(float DeltaTime)
{
	if(!CurrentState) return;
	CurrentState->StateTick(DeltaTime);
}

ASmashCharacter* USmashCharacterStateMachine::GetCharacter() const
{
	return Character;
}

void USmashCharacterStateMachine::ChangeState(ESmashCharacterStateID NextStateID)
{
	USmashCharacterState* NextState = GetState(NextStateID);
	if(!NextState) return;

	if(CurrentState != nullptr)
	{
		CurrentState->ExitState(NextStateID);
	}

	ESmashCharacterStateID PreviousStateID = CurrentStateID;
	CurrentStateID = NextStateID;
	CurrentState = NextState;
	if(CurrentState)
	{
		NextState->EnterState(PreviousStateID);
	}
}

USmashCharacterState* USmashCharacterStateMachine::GetState(ESmashCharacterStateID StateID)
{
	for (TTuple<ESmashCharacterStateID, USmashCharacterState*> State : AllStates)
	{
		if(StateID == State.Key)
			return State.Value;
	}
	return nullptr;
}

bool USmashCharacterStateMachine::IsStateOverrideByCharacter(ESmashCharacterStateID StateID)
{
	for (TSubclassOf<USmashCharacterState> State : Character->CharacterStates)
	{
		USmashCharacterState* CharacterState = State.GetDefaultObject();
		if(!CharacterState) continue;
		
		if(CharacterState->GetStateID() == StateID)
			return true;
	}
	return false;
}

void USmashCharacterStateMachine::CreateState()
{
	const USmashCharacterSettings* SmashCharacterSettings = GetDefault<USmashCharacterSettings>();
	for (TSubclassOf<USmashCharacterState> State : Character->CharacterStates)
	{
		USmashCharacterState* Obj = NewObject<USmashCharacterState>(this, State);
		AllStates.Add(Obj->GetStateID(), Obj);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::Printf(TEXT("Create State %d"), Obj ->GetStateID()));
	}
	for (TSubclassOf<USmashCharacterState> State : SmashCharacterSettings->GenericStates)
	{
		USmashCharacterState* CharacterState = State.GetDefaultObject();
		if(!CharacterState) continue;
		
		if(!AllStates.Contains(CharacterState->GetStateID()))
		{
			USmashCharacterState* Obj = NewObject<USmashCharacterState>(this, State);
			AllStates.Add(Obj->GetStateID(), Obj);
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::Printf(TEXT("Create Default State %d"), Obj ->GetStateID()));
		}
	}
}

void USmashCharacterStateMachine::InitStates()
{
	for (TTuple<ESmashCharacterStateID, USmashCharacterState*> State : AllStates)
	{
		State.Value->InitState(this);
	}
}
