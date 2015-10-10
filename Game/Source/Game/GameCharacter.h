#pragma once

#include "GameFramework/Character.h"
#include "Item.h"
#include "Items/Container_Item.h"
#include "Items/Pickup_Item.h"
#include "Container.h"
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


	/*Variables*/
public:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		float WalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		float RunSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		bool isBleeding;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		bool bIsInventoryOpen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Life")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Life")
		float Thirst;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Life")
		float Hunger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life")
		float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life")
		float MaxThirst;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life")
		float MaxHunger;

	bool bIsFPS;

	bool bIsSprinting;

	float MaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		AContainer_Item *HeadGear;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		AContainer_Item *TopGear;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		AContainer_Item *ChestGear;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		AContainer_Item *BottomGear;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		AContainer_Item *ShoesGear;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		AContainer_Item *BackpackGear;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		AContainer_Item *GlovesGear;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		AItem *HandGear;

	int32 bleedingMultiplier;

	float SaturationLevel;

	float itemId;

	/*Functions*/

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

	FHitResult PickupTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	FHitResult ContainerTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessResults(const FHitResult &Impact);

	void PickupItemLineTrace();

public:
		/*Constructor*/
		AGameCharacter();

		/*--TacMode--*/
		void TacLookOn();
		void TacLookOff();

		/*--Life Functions--*/
		void BleedingFunc(float DeltaTime);
		void HealthFunc(float DeltaTime);
		void SprintFunc(float DeltaTime);

		/*--Movement--*/
		void Sprint();
		void Walking();

		/*Toggle View*/
		void ToggleView();

		/*--ADS--*/
		void ADSOn();
		void ADSOff();

		/*--Crouch--*/
		void CrouchDown();
		void CrouchUp();

		/*Interact with objects*/
		void Interact();

		/*Pickup Items*/
		void Pickup();

		/*Open Container*/
		void Container();

		/*Debug*/
		void Debug();

		/*Blueprint Functions*/

		/*Tick*/
		UFUNCTION()
			void Tick(float DeltaTime) override;

		/*Death Function*/
		UFUNCTION(BlueprintCallable, Category = "Enable Disable Keys")
			void Death();

		/*Enable Keys*/
		UFUNCTION(BlueprintCallable, Category = "Enable Disable Keys")
			bool EnableDisableKeys();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class USkeletalMeshComponent* GetFPS() const { return FirstPersonMesh; }

	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Mesh; }
};

