// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateJump.h"

#include "SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::EnterState(ESmashCharacterStateID PreviousStateID)
{
	Super::EnterState(PreviousStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString(TEXT("Enter StateJump")));
	Character->PlayAnimMontage(JumpAnim);
	
	float HalfDuration = JumpDuration;
	float BaseVelocityY = 2 * JumpMaxHeight / HalfDuration;
	float ComputeGravity = - 2 * JumpMaxHeight / (HalfDuration * HalfDuration);
	float GravityScale = - ComputeGravity / 981;
	
	Character->GetCharacterMovement()->GravityScale = GravityScale;
	Character->GetCharacterMovement()->MaxWalkSpeed = JumpWalkSpeed;
	Character->GetCharacterMovement()->AirControl = JumpAirControl;
	Character->GetCharacterMovement()->JumpZVelocity = BaseVelocityY;
	Character->Jump();
}


void USmashCharacterStateJump::ExitState(ESmashCharacterStateID NextStateID)
{
	Super::ExitState(NextStateID);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString(TEXT("Exit StateJump")));
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	MoveHorizontaly();
	if(Character->GetVelocity().Z < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}

void USmashCharacterStateJump::MoveHorizontaly()
{
	if (FMath::Abs(Character->GetInputMoveX()) >= GetDefault<USmashCharacterSettings>()->InputMoveXThreshold)
	{
		Character->AddMovementInput(FVector::ForwardVector * Character->GetOrientX(), 1);
		Character->SetOrientX(Character->GetInputMoveX());
	}
}
