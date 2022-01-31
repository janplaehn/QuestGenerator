// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "WorldStateProvider.h"
#include "Components/ActorComponent.h"
#include "GameFramework/GameModeBase.h"

#include "SimulatedWorldStateComponent.generated.h"

/**
* Container for simulating npc inventory
*/
USTRUCT(BlueprintType)
struct QUESTGENERATOR_API FCharacterInventory
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Items;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTGENERATOR_API USimulatedWorldStateComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	static USimulatedWorldStateComponent* TryFindWorldStateComponent(const UObject* Context)
	{
		const UWorld* World = Context->GetWorld();
		if (!ensureMsgf(IsValid(World), TEXT("USimulatedWorldStateComponent*: Could not find valid world")))
		{
			return nullptr;
		}
	
		AGameModeBase* GameMode =  World->GetAuthGameMode();
		if (!ensureMsgf(IsValid(GameMode), TEXT("USimulatedWorldStateComponent*: Could not find valid gamemode")))
		{
			return nullptr;
		}

		if (!ensureMsgf(GameMode->Implements<UWorldStateProvider>(), TEXT("Could not find IWorldStateProvider Interface on gamemode")))
		{
			return nullptr;
		}	
		USimulatedWorldStateComponent* WorldStateComponent =  IWorldStateProvider::Execute_GetWorldStateComponent(GameMode);
		if (!ensureMsgf(IsValid(WorldStateComponent), TEXT("QuestProviderComponent: Could not find quest data creator on gamemode")))
		{
			return nullptr;
		}
		return WorldStateComponent;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, bool> IsCharacterAliveMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, bool> PlayerHasItemMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, FCharacterInventory> CharacterHasItemMap;
};
