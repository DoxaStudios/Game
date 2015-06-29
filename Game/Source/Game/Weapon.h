// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MasterItem.h"
#include "Weapon.generated.h"

/**
 * 
 */

/** CUSTOM TRACE CHANNEL FOR THE INSTANT HIT PORTION */
#define TRACE_WEAPON ECC_GameTraceChannel1



/** forward declaration of character.
* couldn't use #include "GameCharacter.h" because of circular dependancy
*/
class AGameCharacter;

/** forward declaration of character.
couldn't use #include "Enemy.h" because of circular dependancy
*/
//class AEnemy;


UENUM(BlueprintType)
/** ENUM OF PROJECTILE TYPES.
* Can be selected in blueprint for Gameplay adjustments
*/
namespace EProjectile
{
	enum Type
	{
		E_Bullet			UMETA(DisplayName = "Bullet"),
		E_Projectile		UMETA(DisplayName = "Projectile"),
		E_Melee				UMETA(DisplayName = "Melee"),
	};
}

USTRUCT(BlueprintType)
/** STRUCT THAT HOLDS BASE VARIABLES FOR THE WEAPON */
struct FWeaponConfig
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	/** Max Ammo of weapon */
	int32 MaxAmmo;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	/** Max Clip of weapon */
	int32 MaxClip;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	/** Range of the weapon's projectile */
	float WeaponRange;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	/** Time between firing the weapon(even if trigger held down) */
	float TimeBetweenShots;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		/** How much does each shot cost */
		int32 ShotCost;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		/** Spread of the weapon for each shot */
		float WeaponSpread;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		/** Name of the weapon */
		FString Name;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		/** Splash art for the UI */
		UTexture2D* SplashArt;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		/** Check to see if it's the primary weapon */
		bool bIsPrimary;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		/** Checks to see if this weapon is Two Handed */
		bool bIsTwoHanded;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		/** Checks to see if weapon is equipped */
		bool bIsEquipped;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		/** Use of Priority Makes the picking up of weapon and equiping them
		* automatically sort them in GameCharacter (SORTING WIP)
		*/
		int32 Priority;
};


UCLASS()
class GAME_API AWeapon : public AMasterItem
{
	GENERATED_BODY()
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	/** Skeletal Mesh of the weapon */
	class USkeletalMeshComponent *Mesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	/** Dynamic collision for the weapon */
	class UBoxComponent *CollisionComp;


	UPROPERTY(EditDefaultsOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
	/** Fire Sound for weapon */
	class USoundCue* FireSound;


	UPROPERTY(EditDefaultsOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
	/** Reload Start Sound for weapon */
	class USoundCue* ReloadStartSound;


	UPROPERTY(EditDefaultsOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
	/** Reload Finish Sound for weapon */
	class USoundCue* ReloadFinishSound;


	UPROPERTY(EditDefaultsOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
	/** Reload Finish Sound for weapon */
	class USoundCue* EmptySound;

public:
	AWeapon(const FObjectInitializer& ObjectInitializer);


	//UPROPERTY(EditDefaultsOnly, Category = Config)
	/** Weapon Projectile(Like rocket, grenade, etc) */
	//TSubclassOf<AWeaponProjectile> WeaponProj;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	/** Weapon Configuration Struct object */
	FWeaponConfig WeapConfig;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	/** Projectile Type Enum Object */
	TEnumAsByte<EProjectile::Type> ProjType;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	/** Current Ammo for weapon */
	int32 CurrentAmmo;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	/** Current Ammo in clip */
	int32 CurrentClip;


	/** bool to check if weapon wants to fire(for automatic and TimeBetweenShots enabling) */
	bool bWantsToFire;


	UFUNCTION(BlueprintCallable, Category = Firing)
	/** Fires Weapon and checks ProjType Enum to decide Projectile type */
	void Fire();


	UFUNCTION(BlueprintCallable, Category = Firing)
	/** Stops firing weapon and empties the Timer Handles */
	void StopFire();

	/** Reloads the Weapon */
	void ReloadAmmo();


	/** virtual Instant_Fire functions.
	* Reason for virtual is in case if the firing function behaves different with different weapons
	* Fires Line Trace From Weapon
	*/
	virtual void Instant_Fire();

	/** virtual ProjectileFire functions.
	* Reason for virtual is in case if the firing function behaves different with different weapons
	* Fires Projectile From Weapon
	*/
	virtual void ProjectileFire();

	/** virtual MeleeFire functions.
	* Reason for virtual is in case if the firing function behaves different with different weapons
	* Turns on Collision Damage from Weapon (WIP)
	*/
	virtual void MeleeFire();


	/** SetOwningPawn Function
	* Retrieves owner for this weapon for the MyPawn variable
	* @param NewOwner  Only allows AGameCharacter Class type
	*/
	void SetOwningPawn(AGameCharacter *NewOwner);


	UFUNCTION(BlueprintCallable, Category = Weapon)
	/** SetOwningPawn Function
	* Retrieves owner for this weapon
	* @param NewOwner  Only allows Enemy Class type
	*/
	//void SetOwningEnemy(AEnemy *NewOwner);


	/** Attaches weapon to player */
	void AttachtoPlayer();

	/** Detaches weapon from player */
	void DetachFromPlayer();


	UFUNCTION(BlueprintCallable, Category = Equip)
	/** Does statements to Equip Weapon */
	void OnEquip();


	UFUNCTION(BlueprintCallable, Category = Equip)
	/** Does statements to UnEquip Weapon */
	void OnUnEquip();


	UFUNCTION(BlueprintCallable, Category = Equip)
	/** Puts the Weapon back in the alternate inventory(WeaponBackpack) */
	void OnBackpack();


	/** Returns a weapon sound to play(Firing, Reloading, etc)
	* @param Sound  SoundCue sound for the weapon to play
	* @return  Returns the Weapon Sound to be played
	*/
	UAudioComponent* PlayWeaponSound(USoundCue *Sound);

protected:

	/** retrieves the trace line points between the socket and the hit object */
	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;


	/** processes the instant hit trace */
	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);


	/** a pointer instance to store the Character class (PART OF THE FORWARD DECLARATOR) */
	AGameCharacter *MyPawn;


	/** a pointer instance to store the Character class (PART OF THE FORWARD DECLARATOR) */
	//AEnemy *MyEnemy;

	FORCEINLINE USkeletalMeshComponent *GetMesh() { return Mesh; }

	FORCEINLINE UBoxComponent *GetCollisionComp() { return CollisionComp; }

	FORCEINLINE USoundCue* GetFireSound() { return FireSound; }

	FORCEINLINE USoundCue* GetReloadStartSound() { return ReloadStartSound; }

	FORCEINLINE USoundCue* GetReloadFinishSound() { return ReloadFinishSound; }

	FORCEINLINE USoundCue* GetEmptySound() { return EmptySound; }
	
	
	
};
