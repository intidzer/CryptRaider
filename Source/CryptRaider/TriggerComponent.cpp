// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

#include "Mover.h"
#include "Engine/StaticMeshActor.h"

UTriggerComponent::UTriggerComponent()
{
	// Set bCanEverTick = true if you want this Component to tick
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	SetMover(GetOwner()->FindComponentByClass<UMover>());
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType,	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(Mover))
	{
		UE_LOG(LogTemp, Error, TEXT("Mover Component is not set for Actor %s"), *GetOwner()->GetFName().ToString())
		return;
	}
	
	if (AActor* Actor = GetAcceptableActor(); IsValid(Actor))
	{
		UE_LOG(LogTemp, Display, TEXT("Unlocking the Gate with: %s"), *Actor->GetActorNameOrLabel())
		{
			if (UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent()))
				Component->SetSimulatePhysics(false);
			Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	
			Mover->SetShouldMove(true);
		}
	}
	else
	{
		Mover->SetShouldMove(false);
	}
		
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
	for (AActor* Actor : Actors)
		if (Actor->ActorHasTag(UnlockTag) && !Actor->ActorHasTag(BlockingTag))
			return Actor;
	return nullptr;
}

void UTriggerComponent::SetMover(UMover* NewMoverComponent)
{
	Mover = NewMoverComponent;
}
