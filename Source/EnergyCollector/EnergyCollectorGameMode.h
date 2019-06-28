// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnergyCollectorGameMode.generated.h"

UCLASS(minimalapi)
class AEnergyCollectorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEnergyCollectorGameMode();

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power")
	float DecayRate;
};



