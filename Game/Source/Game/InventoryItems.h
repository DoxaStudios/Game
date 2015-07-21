// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MasterItem.h"
#include "InventoryItems.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API AInventoryItems : public AMasterItem
{
	GENERATED_BODY()
	
public:
	AInventoryItems(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FItemDataStruct> ItemInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHoldingWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float CurrentHoldingWeight;

protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		//FItemDataStruct ItemInfo;
	
	
};
