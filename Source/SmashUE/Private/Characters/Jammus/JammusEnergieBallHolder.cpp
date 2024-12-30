// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Jammus/JammusEnergieBallHolder.h"

#include "Characters/Jammus/JammusEnergieBall.h"
#include "Kismet/GameplayStatics.h"

void UJammusEnergieBallHolder::CreateEnergieBall()
{
	const FTransform SpawnTranform = SpaceGunMesh->GetSocketTransform(SocketName);
	if(JammusEnergieBall)
	{
		JammusEnergieBall->Destroy();
		JammusEnergieBall = nullptr;
	}
	JammusEnergieBall = GetWorld()->SpawnActorDeferred<AJammusEnergieBall>(JammusEnergieBallBP, SpawnTranform, GetOwner());
	JammusEnergieBall->FinishSpawning(SpawnTranform);
	JammusEnergieBall->K2_AttachToComponent(SpaceGunMesh, SocketName,EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
}

void UJammusEnergieBallHolder::UpdateEnergieBallVisual(float ChargePercentage)
{
	if(!JammusEnergieBall)
		return;

	JammusEnergieBall->SetActorScale3D(static_cast<FVector>(FMath::Lerp(EnergieBallMinMaxSize.X, EnergieBallMinMaxSize.Y, ChargePercentage)));
}

void UJammusEnergieBallHolder::ReleaseEnergieBall()
{
	FDetachmentTransformRules Rules (EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld, false);
	JammusEnergieBall->DetachFromActor(Rules);
}

