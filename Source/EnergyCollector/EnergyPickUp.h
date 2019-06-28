// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "EnergyPickUp.generated.h"

/**
 * 
 */
UCLASS()
class ENERGYCOLLECTOR_API AEnergyPickUp : public APickUp
{
	GENERATED_BODY()

public:
	AEnergyPickUp();

	void WasCollected_Implementation() override;

	float GetPower();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float BatteryPower;
	
};
