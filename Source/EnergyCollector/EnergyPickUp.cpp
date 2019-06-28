// Fill out your copyright notice in the Description page of Project Settings.

#include "EnergyPickUp.h"

AEnergyPickUp::AEnergyPickUp()
{
	GetMesh()->SetSimulatePhysics(true);

	BatteryPower = 150.0f;

}

void AEnergyPickUp::WasCollected_Implementation()
{
	Super::WasCollected_Implementation();

	Destroy();
}

float AEnergyPickUp::GetPower()
{
	return BatteryPower;
}