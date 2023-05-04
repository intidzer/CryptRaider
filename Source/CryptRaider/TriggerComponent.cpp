// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

#include "Mover.h"
#include "Engine/StaticMeshActor.h"

UTriggerComponent::UTriggerComponent()
{
	// Set bCanEverTick = true if you want this Component to tick
	PrimaryComponentTick.bCanEverTick = false;
	OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnCollisionStart);
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("Trigger component BeginPlay"))
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType,	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTriggerComponent::OnCollisionStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	for (auto& Tag : OtherActor->Tags)
		UE_LOG(LogTemp, Display, TEXT("TAG %s"), *Tag.ToString())
	UE_LOG(LogTemp, Display, TEXT("BlockingTag TAG %s"), *BlockingTag.ToString())
	UE_LOG(LogTemp, Display, TEXT("UnlockTag TAG %s"), *UnlockTag.ToString())
	
	if (OtherActor->ActorHasTag(UnlockTag) && !OtherActor->ActorHasTag(BlockingTag))
	{
		UE_LOG(LogTemp, Display, TEXT("Unlocking the Gate with: %s"), *OtherActor->GetActorNameOrLabel())
		if (auto* MoverComponent = GetOwner()->FindComponentByClass<UMover>())
		{
			OtherComponent->SetSimulatePhysics(false);
			OtherActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
			
			MoverComponent->StartMovement();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Mover Component is not set for Actor %s"), *GetOwner()->GetFName().ToString())
		}
	}
}
