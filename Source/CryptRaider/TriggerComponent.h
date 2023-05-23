// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor.h"
#include "Components/BoxComponent.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:

	UTriggerComponent();
		
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnCollisionStart(UPrimitiveComponent* OverlappedComponent,
						AActor* OtherActor,
						UPrimitiveComponent* OtherComponent,
						int32 OtherBodyIndex,
						bool bFromSweep,
						const FHitResult& Hit);

	UFUNCTION()
	void OnCollisionEnd(UPrimitiveComponent* OverlappedComponent,
						AActor* OtherActor,
	                    UPrimitiveComponent* OtherComponent,
	                    int32 OtherBodyIndex);
	void HandleOverlapping(AActor* OtherActor, UPrimitiveComponent* OtherComponent);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	FName UnlockTag;
	
	UPROPERTY(EditAnywhere)
	FName BlockingTag;

	UPROPERTY()
	AActor* OverlappingActor;
};