// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimulatedWorldStateComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTGENERATOR_API USimulatedWorldStateComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, bool> IsCharacterAliveMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, bool> PlayerHasItemMap;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, bool> PlayerHasUsedItemMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, bool> PlayerWasDetectedByCharacterMap;
};
