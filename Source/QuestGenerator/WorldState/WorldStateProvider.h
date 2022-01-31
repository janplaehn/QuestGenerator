// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WorldStateProvider.generated.h"

class USimulatedWorldStateComponent;

UINTERFACE(MinimalAPI)
class UWorldStateProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class QUESTGENERATOR_API IWorldStateProvider
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	USimulatedWorldStateComponent* GetWorldStateComponent() const;
};
