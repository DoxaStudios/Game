// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MasterItem.generated.h"

UCLASS()
class GAME_API AMasterItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

protected:
	USTRUCT()
	struct ItemDataStruct
	{
		GENERATED_USTRUCT_BODY()

			UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
			FString Name;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
			int32 ItemID;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
			FTexture Icon;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
			FString Description;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
			float Weight;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
			bool bIsConsumable;




		ItemDataStruct()
		{

		}
	};

	
};
