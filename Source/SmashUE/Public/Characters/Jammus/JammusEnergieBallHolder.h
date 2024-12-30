// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JammusEnergieBallHolder.generated.h"

class AJammusEnergieBall;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class SMASHUE_API UJammusEnergieBallHolder : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AJammusEnergieBall> JammusEnergieBallBP;

	UPROPERTY(EditAnywhere)
	FName SocketName;

	UPROPERTY(EditAnywhere)
	FVector2f EnergieBallMinMaxSize;
	
	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* SpaceGunMesh;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AJammusEnergieBall> JammusEnergieBall;

	void CreateEnergieBall();

	void UpdateEnergieBallVisual(float ChargePercentage);

	void ReleaseEnergieBall();

	
};
