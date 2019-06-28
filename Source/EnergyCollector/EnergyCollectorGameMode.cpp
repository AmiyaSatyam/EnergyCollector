// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EnergyCollectorGameMode.h"
#include "EnergyCollectorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AEnergyCollectorGameMode::AEnergyCollectorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryComponentTick.bCanEverTick = true;

	DecayRate = 0.01f;
}

void AEnergyCollectorGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AEnergyCollectorCharacter* MyCharacter = Cast<AEnergyCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		if (MyCharacter->GetCurrentPower() > 0)
		{
			MyCharacter->UpdatePower(-DeltaTime * DecayRate*(MyCharacter->GetInitialPower()));
		}
		
	}
}
