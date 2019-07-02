// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EnergyCollectorGameMode.h"
#include "EnergyCollectorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PawnMovementComponent.h"

AEnergyCollectorGameMode::AEnergyCollectorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = true;

	DecayRate = 0.01f;
}

void AEnergyCollectorGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}

	SetCurrentState(EEnergyPlayState::EPlaying);

	AEnergyCollectorCharacter* MyCharacter = Cast<AEnergyCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		PowerToWin = (MyCharacter->GetInitialPower())*1.25f;
	}

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}

}

void AEnergyCollectorGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AEnergyCollectorCharacter* MyCharacter = Cast<AEnergyCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		if (MyCharacter->GetCurrentPower() > PowerToWin)
		{
			SetCurrentState(EEnergyPlayState::EWon);
		}

		if (MyCharacter->GetCurrentPower() > 0)
		{
			MyCharacter->UpdatePower(-DeltaTime * DecayRate*(MyCharacter->GetInitialPower()));
		}
		else
		{
			SetCurrentState(EEnergyPlayState::EGameOver);
		}
		
	}
}

float AEnergyCollectorGameMode::GetPowerToWin() const
{
	return PowerToWin;
}

EEnergyPlayState AEnergyCollectorGameMode::GetCurrentState() const
{
	return CurrentState;
}

void AEnergyCollectorGameMode::SetCurrentState(EEnergyPlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void AEnergyCollectorGameMode::HandleNewState(EEnergyPlayState NewState)
{
	switch (NewState)
	{
	case EEnergyPlayState::EPlaying:
	{
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(true);
		}
	}
	break;
	case EEnergyPlayState::EWon:
	{
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(false);
		}
	}
	break;
	case EEnergyPlayState::EGameOver:
	{
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(false);
		}

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			PlayerController->SetCinematicMode(true, false, false, true, true);
		}
		ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (MyCharacter)
		{
			MyCharacter->GetMesh()->SetSimulatePhysics(true);
			MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
		}
	}
	break;
	default:
	case EEnergyPlayState::EUnknown:
	{

	}
	break;
	}
}
