// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor.h"
#include "Mover.h"
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

	AActor* GetAcceptableActor() const;

	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* MoverComponent);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere,Category="Trigger Component|Tags")
	FName UnlockTag;
	
	UPROPERTY(EditAnywhere,Category="Trigger Component|Tags")
	FName BlockingTag;

	UPROPERTY(EditAnywhere,Category="Trigger Component|Mass")
	bool IsPressurePlate;
	
	UPROPERTY(EditAnywhere,Category="Trigger Component|Mass")
	float Mass;
	
	UPROPERTY(EditAnywhere,Category="Trigger Component|Mass")
	float MassDelta;

	UPROPERTY()
	UMover* Mover;
};