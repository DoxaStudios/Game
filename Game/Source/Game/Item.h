#pragma once

#include "GameFramework/Actor.h"
#include "Item.generated.h"

USTRUCT(blueprintable)
struct FItemDataStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		UTexture2D *Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		float ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		bool bCanBeHeld;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		bool bStackable;



};


UCLASS()
class GAME_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FItemDataStruct ItemInfo;

	
	
};
