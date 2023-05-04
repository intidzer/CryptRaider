// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "CryptRaiderCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Bind Grabber Action
	/*UE_LOG(LogTemp, Display, TEXT("OWNER OF GRABBER: %s"), *GetOwner()->GetActorNameOrLabel())*/
	if (const ACryptRaiderCharacter* Player = Cast<ACryptRaiderCharacter>(GetOwner()))
		if (const APlayerController* Controller = Cast<APlayerController>(Player->GetController()))
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(Controller->InputComponent))
				if (GetPhysicsHandleComponent())
				{
					EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Triggered, this, &UGrabber::Grab);
					EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Completed, this, &UGrabber::Release);
				}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandleComponent = GetPhysicsHandleComponent();
	if (!PhysicsHandleComponent)
		return;

	if (FHitResult HitResult; HasCollision(HitResult))
	{
		HoldItem(PhysicsHandleComponent, HitResult);
	}
}

bool UGrabber::HasCollision(FHitResult& OutHitResult) const
{
	const FVector Start = GetComponentLocation();
	const FVector End = Start + GetForwardVector() * MaxGrabDistance;
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);
}

void UGrabber::HoldItem(UPhysicsHandleComponent* PhysicsHandleComponent, const FHitResult& HitResult)
{
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();
	HitComponent->WakeAllRigidBodies();

	if (Released)
	{
		UE_LOG(LogTemp, Display, TEXT("UGrabber Grab"));
		PhysicsHandleComponent->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
		HitResult.GetActor()->Tags.Add("Grabbed");
		Released = false;
	}
	PhysicsHandleComponent->SetTargetLocationAndRotation(GetComponentLocation() + GetForwardVector() * HoldDistance, GetComponentRotation());
}

void UGrabber::Release()
{
	Released = true;
	if (UPhysicsHandleComponent* PhysicsHandleComponent = GetPhysicsHandleComponent())
		if (UPrimitiveComponent* PrimitiveComponent = PhysicsHandleComponent->GetGrabbedComponent())
		{
			UE_LOG(LogTemp, Display, TEXT("UGrabber Release"));
			PrimitiveComponent->WakeAllRigidBodies();
			PhysicsHandleComponent->ReleaseComponent();
			PrimitiveComponent->GetOwner()->Tags.Remove("Grabbed");
		}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandleComponent() const
{
	UPhysicsHandleComponent* PhysicsHandleComponent = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandleComponent)
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandleComponent for UGrabber"))
	return PhysicsHandleComponent;
}
