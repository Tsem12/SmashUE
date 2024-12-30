// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JammusEnergieBall.generated.h"

class UProjectileMovementComponent;

UCLASS(Blueprintable)
class SMASHUE_API AJammusEnergieBall : public AActor
{
	GENERATED_BODY()
	
#pragma region Default
public:
	// Sets default values for this actor's properties
	AJammusEnergieBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion
	
#pragma region EnergieBall Behaviour
public:
	void Init(float Power, float OrientX);

	UFUNCTION(BlueprintImplementableEvent)
	void SetEnergieBallVisibility(bool isOn);
	
protected:
	UPROPERTY()
	int EnergieBallStrenght;

private:
	bool IsThrowed;
	float Direction;
	float LifeTime = 5.f;

	
	
#pragma endregion 
};