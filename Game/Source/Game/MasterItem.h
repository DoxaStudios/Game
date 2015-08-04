// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MasterItem.generated.h"

USTRUCT(blueprintable)
struct FItemDataStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D *Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bCanBeHeld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bIsWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bIsConsumable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bIsWearable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		bool bIsShoes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		bool bIsHeadWear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		bool bIsShirt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		bool bIsPants;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		bool bIsBackpack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float AddHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float AddHunger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float AddThirst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TSubclassOf<AActor> SpawnItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		AActor *Reference;
	

};

UCLASS(Blueprintable)
class GAME_API AMasterItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterItem(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FItemDataStruct ItemInfo;


protected:


	
};
