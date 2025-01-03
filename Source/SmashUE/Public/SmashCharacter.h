// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Camera/CameraFollowTarget.h"
#include "GameFramework/Character.h"
#include "SmashCharacter.generated.h"

class USmashCharacterState;
class USmashCharacterInputData;
class UInputMappingContext;
class USmashCharacterStateMachine;
class UEnhancedInputComponent;

UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter, public ICameraFollowTarget
{
	GENERATED_BODY()

#pragma region Unreal Default
public:
	// Sets default values for this character's properties
	ASmashCharacter();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion

#pragma region Orient
public:
	float GetOrientX() const;
	void SetOrientX(float NewOrientX);

protected:
	UPROPERTY(BlueprintReadOnly)
	float OrientX = 1.f;

	void RotateMeshUsingOrientX() const;
#pragma endregion

#pragma region StateMachine
public:
	
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const; 

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<USmashCharacterState>> CharacterStates;
	
protected:
	UPROPERTY()
	TArray<UObject*> InstanciatedStates;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USmashCharacterStateMachine> StateMachine;



#pragma endregion
	
#pragma region Input Data / Mapping Context
public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;
	
protected:
	void SetupMappingContextIntoController() const;	
#pragma endregion

#pragma region Input Move X

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);
public:
	float GetInputMoveX();
	float GetInputMoveXTreshold();
	void SetInputMoveXTreshold(float value);

	UPROPERTY()
	FInputMoveXEvent InputMoveXFastEvent;
protected:
	UPROPERTY()
	float InputMoveX = 0.f;

	UPROPERTY()
	float InputMoveXTreshold = 0.f;
private:
	void OnInputMoveXFast(const FInputActionValue& InputActionValue);
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);
	void OnInputMoveX(const FInputActionValue& InputActionValue);
#pragma endregion

#pragma region InputY
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputJumpEvent, float, InputMoveY);
	void BindInputMoveYAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);
	void OnInputMoveY(const FInputActionValue& InputActionValue);
	
	UPROPERTY()
	FInputJumpEvent InputJumpEvent;
protected:
	UPROPERTY()
	float InputMoveY = 0.f;
	
#pragma endregion

#pragma region InputButtons
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputSpecialPressed);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputSpecialReleased);
	void OnSpecialPressed();
	void OnSpecialReleased();
	
	void BindInputButtons(UEnhancedInputComponent* EnhancedInputComponent);

	UPROPERTY()
	FInputSpecialPressed InputSpecialPressed;
	
	UPROPERTY()
	FInputSpecialReleased InputSpecialReleased;

protected:

#pragma endregion 

#pragma region FollowTarget
public:
	virtual FVector GetFollowPosition() override;
	virtual bool IsFollowable() override;
#pragma endregion

#pragma region Collision

protected:
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	
#pragma endregion



#pragma region Death
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathDelegate);

	UPROPERTY(BlueprintAssignable)
	FDeathDelegate DeathDelegateBluePrint;
#pragma endregion 
};
