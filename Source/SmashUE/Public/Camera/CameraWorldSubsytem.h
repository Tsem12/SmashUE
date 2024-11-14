// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CameraWorldSubsytem.generated.h"

/**
 * 
 */
class UCameraComponent;

UCLASS()
class SMASHUE_API UCameraWorldSubsytem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

#pragma region Subsystem Ovverides
public:
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override {return TStatId();};
#pragma endregion

#pragma region Main Camera
protected:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraMain;

	void TickUpdateCameraZoom(float DeltaTime);
	
	void TickUpdateCameraPosition(float DeltaTime);
#pragma endregion

#pragma region FollowTarget
public:
	void AddFollowTarget(UObject* FollowTarget);
	void RemoveFollowTarget(UObject* FollowTarget);
protected:
	UPROPERTY()
	TArray<UObject*> FollowTargets;

	
	FVector CalculateAveragePositionBetweenTargets();
	float CalculateGreatestDistanceBetweenTargets();
#pragma endregion

#pragma region Misc
protected:
	UCameraComponent* FindCameraByTag(const FName& Tag) const;
#pragma endregion

#pragma region Bounds
protected:
	UPROPERTY()
	FVector2D CameraBoundMin;

	UPROPERTY()
	FVector2D CameraBoundMax;

	UPROPERTY()
	float CameraBoundYprojectionCenter;

	AActor* FindCameraBounds(FName Tag);

	void InitCameraBounds(AActor* CameraBoundsActor);

	void ClampPositionIntoCameraBounds(FVector& Position);

	void GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax);

	FVector CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition);
	
#pragma endregion

#pragma region Zoom
	UPROPERTY()
	float CameraZoomYMin = 0.f;
	
	UPROPERTY()
	float CameraZoomYMax = 0.f;

	UPROPERTY()
	float CameraZoomDistanceBetweenTargetsMin = 300.f;

	UPROPERTY()
	float CameraZoomDistanceBetweenTargetsMax = 1500.f;

	UFUNCTION()
	void InitCameraZoomParameters();
#pragma endregion 
};
