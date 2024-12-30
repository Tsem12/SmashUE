// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Jammus/JammusEnergieBall.h"

#include "SmashCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AJammusEnergieBall::AJammusEnergieBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AJammusEnergieBall::BeginPlay()
{
	Super::BeginPlay();
}

void AJammusEnergieBall::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(!IsThrowed)
		return;
	
	Super::NotifyActorBeginOverlap(OtherActor);
	ASmashCharacter* Chara = Cast<ASmashCharacter>(OtherActor);
	if(Chara)
	{
		Chara->LaunchCharacter(FVector(Direction * EnergieBallStrenght, 0, EnergieBallStrenght) * 3, false, false);
	}
	Destroy();
}



// Called every frame
void AJammusEnergieBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!IsThrowed)
		return;
	
	LifeTime -= DeltaTime;
	
	if(LifeTime <= 0)
	{
		Destroy();
	}
	
	SetActorLocation(GetActorLocation() + FVector::ForwardVector * Direction * DeltaTime * EnergieBallStrenght);
	
}

void AJammusEnergieBall::Init(float Power, float OrientX)
{
	IsThrowed = true;
	EnergieBallStrenght = Power;
	Direction = OrientX;
	
}


