// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Character.h"
#include "MasterItem.h"
#include "Container.h"
#include "InventoryItems.h"
#include "Weapon.h"
#include "WeaponProjectile.h"
#include "GameCharacter.generated.h"

#define TRACE_INVENTORY ECC_GameTraceChannel2

UCLASS(config=Game)
class AGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FirstPersonMesh;
public:
	AGameCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UFUNCTION(BlueprintImplementableEvent, Category = "OpenContainer")
	void OpenContainer();

	UFUNCTION(BlueprintImplementableEvent, Category = "OpenContainer")
	void AttatchItem(AInventoryItems* InventoryItem);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Container")
	AContainer *SavedContainer;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//Toggle View Function that is public
	void ToggleView();

	//Sprinting Function
	void Sprint();
	void Walking();

	UFUNCTION()
	void Tick(float DeltaTime) override;

	//Is FPS boolean
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	bool bIsFPS;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float RunSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CurrentSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float Stamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool isBleeding;


	/*Inventory Data Structs and Variables*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	bool bIsInventoryOpen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	FItemDataStruct InventoryItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FItemDataStruct> InventoryItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FItemDataStruct HeadGear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		AInventoryItems *ShirtGear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		AInventoryItems *ChestGear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		AInventoryItems *PantsGear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		FItemDataStruct ShoesGear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		AInventoryItems *Backpack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FItemDataStruct Hand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 ItemSelected;
	

	/*Inventory Functions*/
	UFUNCTION(BlueprintCallable, Category = "Enable Disable Keys")
	bool EnableDisableKeys();

	UFUNCTION(BlueprintCallable, Category = "SetActor")
	void Drop(AActor *Referenced);

	void Interact();
	void Pickup();
	void Container();

	//Life System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
	float Thirst;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
	float Hunger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life")
	float MaxThirst;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life")
	float MaxHunger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life")
	float MaxWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life")
	float CurrentWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
		float SaturationLevel;

	void HealthFunc(float DeltaTime);
	void SprintFunc(float DeltaTime);

	//Level System
	void SprintLevelFunc(int32 SprintLevel, float MaxWalkSpeed);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 SprintLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxSprintLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	float CurrentId;

	void Debug();


	void TacLookOn();
	void TacLookOff();

	void BleedingFunc(float DeltaTime);


protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	FHitResult PickupTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	FHitResult ContainerTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessResults(const FHitResult &Impact);

	void PickupItemLineTrace();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

