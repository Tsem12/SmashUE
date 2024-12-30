// Fill out your copyright notice in the Description page of Project Settings.


#include "SmashCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraWorldSubsytem.h"
#include "Characters/SmashCharacterInputData.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterState.h"

#include "Characters/SmashCharacterStateMachine.h"


// Sets default values
ASmashCharacter::ASmashCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();
	InitStateMachine();
	UCameraWorldSubsytem* CameraWorldSubsytem = Cast<UCameraWorldSubsytem>(GetWorld()->GetSubsystem<UCameraWorldSubsytem>());
	if(!CameraWorldSubsytem) return;
	CameraWorldSubsytem->AddFollowTarget(this);
}

// Called every frame
void ASmashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateMeshUsingOrientX();
	TickStateMachine(DeltaTime);
}

// Called to bind functionality to input
void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(!EnhancedInputComponent) return;

	BindInputMoveXAxisAndActions(EnhancedInputComponent);
	BindInputMoveYAxisAndActions(EnhancedInputComponent);
	BindInputButtons(EnhancedInputComponent);
}

float ASmashCharacter::GetOrientX() const
{
	return OrientX;
}

float ASmashCharacter::GetInputMoveX()
{
	return InputMoveX;
}

float ASmashCharacter::GetInputMoveXTreshold()
{
	return InputMoveXTreshold;
}

void ASmashCharacter::SetInputMoveXTreshold(float value)
{
	InputMoveXTreshold = value;
}


void ASmashCharacter::BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if(!InputData) return;
	
	if(InputData->InputActionMoveX)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionMoveX, ETriggerEvent::Started, this, &ASmashCharacter::OnInputMoveX);
		EnhancedInputComponent->BindAction(InputData->InputActionMoveX, ETriggerEvent::Triggered, this, &ASmashCharacter::OnInputMoveX);
		EnhancedInputComponent->BindAction(InputData->InputActionMoveX, ETriggerEvent::Completed, this, &ASmashCharacter::OnInputMoveX);
	}

	if(InputData->InputActionMoveXFast)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionMoveXFast, ETriggerEvent::Started, this, &ASmashCharacter::OnInputMoveXFast);
	}
}

void ASmashCharacter::BindInputMoveYAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if(!InputData) return;
	
	if(InputData->InputActionMoveX)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionJump, ETriggerEvent::Started, this, &ASmashCharacter::OnInputMoveY);
		EnhancedInputComponent->BindAction(InputData->InputActionJump, ETriggerEvent::Triggered, this, &ASmashCharacter::OnInputMoveY);
		EnhancedInputComponent->BindAction(InputData->InputActionJump, ETriggerEvent::Completed, this, &ASmashCharacter::OnInputMoveY);
	}
}

void ASmashCharacter::BindInputButtons(UEnhancedInputComponent* EnhancedInputComponent)
{
	if(!InputData) return;
	
	if(InputData->InputActionMoveX)
	{
		EnhancedInputComponent->BindAction(InputData->InputSpecial, ETriggerEvent::Started, this, &ASmashCharacter::OnSpecialPressed);
		EnhancedInputComponent->BindAction(InputData->InputSpecial, ETriggerEvent::Completed, this, &ASmashCharacter::OnSpecialReleased);
	}
}

void ASmashCharacter::OnSpecialPressed()
{
	InputSpecialPressed.Broadcast();
}

void ASmashCharacter::OnSpecialReleased()
{
	InputSpecialReleased.Broadcast();
}


void ASmashCharacter::OnInputMoveX(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
}

void ASmashCharacter::OnInputMoveY(const FInputActionValue& InputActionValue)
{
	InputMoveY = InputActionValue.Get<float>();
	InputJumpEvent.Broadcast(InputMoveX);
}


void ASmashCharacter::OnInputMoveXFast(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
	InputMoveXFastEvent.Broadcast(InputMoveX);
}


void ASmashCharacter::SetOrientX(float NewOrientX)
{
	OrientX = NewOrientX;
}

void ASmashCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();
	Rotation.Yaw = -90 * OrientX;
	GetMesh()->SetRelativeRotation(Rotation);
}


void ASmashCharacter::CreateStateMachine()
{
	StateMachine = NewObject<USmashCharacterStateMachine>(this);
}

void ASmashCharacter::InitStateMachine()
{
	if(!StateMachine) return;
	StateMachine->Init(this);
}

void ASmashCharacter::TickStateMachine(float DeltaTime) const
{
	if(!StateMachine) return;
	StateMachine->Tick(DeltaTime);
}

void ASmashCharacter::SetupMappingContextIntoController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if(!PlayerController) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if(!Player) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(!InputData) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
}

#pragma region FollowTarget

FVector ASmashCharacter::GetFollowPosition()
{
	return GetActorLocation();
}

bool ASmashCharacter::IsFollowable()
{
	return true;
}


void ASmashCharacter::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if(OtherActor->ActorHasTag("DeathBounds"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString(TEXT("Mort")));
		DeathDelegateBluePrint.Broadcast();
	}
}

#pragma endregion 





