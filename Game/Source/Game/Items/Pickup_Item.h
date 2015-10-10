// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "Pickup_Item.generated.h"

UENUM(BlueprintType)
namespace EPickupItemType
{
	enum Type
	{
		E_Pickup			UMETA(DisplayName = "Pickup"),
		E_Container			UMETA(DisplayName = "Container"),
	};
}

UCLASS()
class GAME_API APickup_Item : public AItem
{
	GENERATED_BODY()
	

public:
	APickup_Item(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
		TEnumAsByte<EPickupItemType::Type> PickupItemType;
	
	//Objects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
		/** Mesh */
		class UStaticMeshComponent *Mesh;
};
