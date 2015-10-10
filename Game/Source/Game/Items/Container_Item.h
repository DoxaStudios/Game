// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Item.h"
#include "Container_Item.generated.h"

class AGameCharacter;

UENUM(BlueprintType)
namespace EContainerItemType
{
	enum Type
	{
		E_Hat				UMETA(DisplayName = "Hat"),
		E_Top				UMETA(DisplayName = "Top"),
		E_Bottom			UMETA(DisplayName = "Bottom"),
		E_Shoes				UMETA(DisplayName = "Shoes"),
		E_Backpack			UMETA(DisplayName = "Backpack"),
		E_Gloves			UMETA(DisplayName = "Gloves"),
	};
}

UCLASS()
class GAME_API AContainer_Item : public AItem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
		/** Mesh */
		class USkeletalMeshComponent *Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
			/** Dynamic collision for the weapon */
		class UBoxComponent *CollisionComp;
	
public:

	AContainer_Item(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
		TEnumAsByte<EContainerItemType::Type> ContainerItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
		TArray<AItem*> Items;

	void Drop();

	void Pickup();

	void SetOwningPawn(AGameCharacter *NewOwner);

	AGameCharacter *Owner;
	
};
