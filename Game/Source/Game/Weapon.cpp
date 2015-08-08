// Fill out your copyright notice in the Description page of Project Settings.

#include "Game.h"
#include "GameCharacter.h"
#include "Weapon.h"


AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	CollisionComp = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, "CollisionComp");
	RootComponent = CollisionComp;

	Mesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, "Mesh");
	Mesh->AttachTo(RootComponent);

	ItemInfo.bIsWeapon = true;
}

void AWeapon::Fire()
{
	//if the player isnt firing
	if (!bWantsToFire)
	{
		//fire weapon
		bWantsToFire = true;
		//TIMER HANDLER FOR TIME BETWEEN SHOTS
		FTimerHandle MyHandle;
		switch (ProjType)
		{
		case EProjectile::E_Bullet:
			Instant_Fire();
			break;
		case EProjectile::E_Projectile:
			ProjectileFire();
			break;
		case EProjectile::E_Melee:
			MeleeFire();
		default:
			break;
		}
	}
	else
	{

	}
}

void AWeapon::StopFire()
{
	//if wanting to fire is on
	if (bWantsToFire)
	{
		//turn bWantsToFire off
		bWantsToFire = false;
		//clear weapon fire timers
		GetWorldTimerManager().ClearTimer(this, &AWeapon::Instant_Fire);
		GetWorldTimerManager().ClearTimer(this, &AWeapon::ProjectileFire);
	}
}

void AWeapon::OnEquip()
{
	//turn off collisions for both the box and mesh
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//attach weapon to player
	AttachtoPlayer();
	//turn on isEquipped
	WeapConfig.bIsEquipped = true;
}

void AWeapon::OnUnEquip()
{
	//detach from player's hand into holster
	DetachFromPlayer();
	//turn off bIsEquipped
	WeapConfig.bIsEquipped = false;
}

void AWeapon::AttachtoPlayer()
{
	//if The weapon has an owner
	if (MyPawn)
	{
		//detach (for debugging purposes)
		//DetachFromPlayer();

		//if it's a right handed weapon
		if (WeapConfig.bIsPrimary)
		{
			if (!WeapConfig.bIsTwoHanded)
			{
				USkeletalMeshComponent *Character = MyPawn->GetMesh();
				SetActorHiddenInGame(false);
				SetActorEnableCollision(true);
				Mesh->SetHiddenInGame(false);
				Mesh->Activate(true);
				Mesh->AttachTo(Character, "WeapSocketR");
			}
			else
			{
				USkeletalMeshComponent *Character = MyPawn->GetMesh();
				SetActorHiddenInGame(false);
				SetActorEnableCollision(true);
				Mesh->SetHiddenInGame(false);
				Mesh->Activate(true);
				Mesh->AttachTo(Character, "WeapSocketR");
			}
		}
		//if it's a left handed weapon
		if (!WeapConfig.bIsPrimary)
		{
			USkeletalMeshComponent *Character = MyPawn->GetMesh();
			Mesh->SetHiddenInGame(false);
			Mesh->AttachTo(Character, "WeapSocketL");
		}
		//if it's a Two Handed weapon(WIP)
		else
		{

		}

	}

	/*if (MyEnemy)
	{
		//detach (for debugging purposes)
		DetachFromPlayer();

		//if it's a right handed weapon
		if (WeapConfig.bIsPrimary)
		{
			USkeletalMeshComponent *Character = MyEnemy->GetMesh();
			Mesh->SetHiddenInGame(false);
			Mesh->AttachTo(Character, "Weapon_r", EAttachLocation::SnapToTarget, true);
		}
		//if it's a left handed weapon
		if (!WeapConfig.bIsPrimary)
		{
			USkeletalMeshComponent *Character = MyEnemy->GetMesh();
			Mesh->SetHiddenInGame(false);
			Mesh->AttachTo(Character, "Weapon_l", EAttachLocation::SnapToTarget, true);
		}
		//if it's a Two Handed weapon(WIP)
		else
		{
			USkeletalMeshComponent *Character = MyEnemy->GetMesh();
			Mesh->SetHiddenInGame(false);
			Mesh->AttachTo(Character, "Weapon_r", EAttachLocation::SnapToTarget, true);
		}
	}
	*/
}

void AWeapon::DetachFromPlayer()
{
	//if the weapon has an owner
	if (MyPawn)
	{
		//if weapon is right handed
		if (WeapConfig.bIsPrimary)
		{
			//if Weapon is NOT TwoHanded
			if (!WeapConfig.bIsTwoHanded)
			{
				USkeletalMeshComponent *Character = MyPawn->GetMesh();
				Mesh->SetHiddenInGame(false);
				Mesh->AttachTo(Character, "HolsterL");
			}
			else
			{
				USkeletalMeshComponent *Character = MyPawn->GetMesh();
				Mesh->SetHiddenInGame(false);
				Mesh->AttachTo(Character, "TwoHandedHolster");
			}
		}
		//if weapon is Left handed
		if (!WeapConfig.bIsPrimary)
		{
			USkeletalMeshComponent *Character = MyPawn->GetMesh();
			Mesh->SetHiddenInGame(false);
			Mesh->AttachTo(Character, "HolsterR");
		}
		//if weapon is Two Handed(WIP)
		else
		{

		}

	}

	/*if (MyEnemy)
	{
		//if weapon is right handed
		if (WeapConfig.bIsPrimary)
		{
			Mesh->SetHiddenInGame(false);
		}
		//if weapon is Left handed
		if (!WeapConfig.bIsPrimary)
		{
			Mesh->SetHiddenInGame(false);
		}
		//if weapon is Two Handed(WIP)
		else
		{

		}
	}
	*/
}

//PUTS AWAY WEAPON IN BACKPACK(REMOVES FROM SCENE)
void AWeapon::OnBackpack()
{
	//unattach weapon from socket
	Mesh->DetachFromParent();
	//hide the weapon
	Mesh->SetHiddenInGame(true);
}

void AWeapon::SetOwningPawn(AGameCharacter *NewOwner)
{
	//if MyPawn does not equal argument parameter
	if (MyPawn != NewOwner)
	{
		//make the player the new owner
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

/*void AWeapon::SetOwningEnemy(AEnemy *NewOwner)
{
	//if MyPawn does not equal argument parameter
	if (MyEnemy != NewOwner)
	{
		//make the player the new owner
		Instigator = NewOwner;
		MyEnemy = NewOwner;
	}
}
*/

void AWeapon::ReloadAmmo()
{
	if (CurrentClip > 0)
	{
		if (CurrentAmmo < WeapConfig.MaxClip)
		{
			CurrentAmmo = WeapConfig.MaxAmmo;
			CurrentClip -= 1;
		}
		else if (CurrentAmmo == WeapConfig.MaxAmmo)
		{
			return;
		}
		else
		{
			CurrentAmmo = WeapConfig.MaxAmmo;
			CurrentClip -= 1;
		}
		PlayWeaponSound(ReloadStartSound);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "No Ammo Left");
		PlayWeaponSound(EmptySound);
	}
}

FHitResult AWeapon::WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const
{
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingle(Hit, TraceFrom, TraceTo, TRACE_WEAPON, TraceParams);

	return Hit;
}

void AWeapon::ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread)
{
	/*if (MyEnemy)
	{
		const FVector EndTrace = Origin + ShootDir * WeapConfig.WeaponRange;
		const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;
		DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Black, true, 10000, 10.f);

		AGameCharacter *Player = Cast<AGameCharacter>(Impact.GetActor());
		if (Player)
		{
			Player->Health -= 10;
			if (Player->Health <= 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT A PLAYER!!");
				Player->Death();
				Player->GetMesh()->AddImpulseAtLocation(EndPoint * 2, EndPoint);
			}
		}
	}
	*/
	if (MyPawn)
	{
		const FVector EndTrace = Origin + ShootDir * WeapConfig.WeaponRange;
		const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;
		DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Black, true, 10000, 10.f);

		/*AEnemy *Enemy = Cast<AEnemy>(Impact.GetActor());
		if (Enemy)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT AN ENEMY!!");
			Enemy->GetMesh()->SetSimulatePhysics(true);
			Enemy->GetMesh()->AddImpulseAtLocation(EndPoint * 100, EndPoint);
		}
		*/
	}


}

void AWeapon::Instant_Fire()
{
	if (MyPawn)
	{
		if (CurrentAmmo > 0)
		{
			// Get the camera transform
			FVector CameraLoc;
			FRotator CameraRot;
			GetActorEyesViewPoint(CameraLoc, CameraRot);
			const int32 RandomSeed = FMath::Rand();
			FRandomStream WeaponRandomStream(RandomSeed);
			const float CurrentSpread = WeapConfig.WeaponSpread;
			const float SpreadCone = FMath::DegreesToRadians(WeapConfig.WeaponSpread * 0.5);
			const FVector AimDir = MyPawn->GetFollowCamera()->GetForwardVector();
			const FVector StartTrace = Mesh->GetSocketLocation("MF");
			const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
			const FVector EndTrace = StartTrace + ShootDir * WeapConfig.WeaponRange;
			const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

			ProcessInstantHit(Impact, StartTrace, ShootDir, RandomSeed, CurrentSpread);
			CurrentAmmo -= WeapConfig.ShotCost;

			GetWorldTimerManager().SetTimer(this, &AWeapon::Instant_Fire, WeapConfig.TimeBetweenShots, false);
			PlayWeaponSound(FireSound);
		}
		else
		{
			ReloadAmmo();
		}
	}
	/*if (MyEnemy)
	{
		// Get the camera transform
		FVector CameraLoc;
		FRotator CameraRot;
		GetActorEyesViewPoint(CameraLoc, CameraRot);
		const int32 RandomSeed = FMath::Rand();
		FRandomStream WeaponRandomStream(RandomSeed);
		const float CurrentSpread = WeapConfig.WeaponSpread;
		const float SpreadCone = FMath::DegreesToRadians(WeapConfig.WeaponSpread * 0.5);
		const FVector AimDir = Mesh->GetSocketRotation("MF").Vector();
		const FVector StartTrace = Mesh->GetSocketLocation("MF");
		const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
		const FVector EndTrace = StartTrace + ShootDir * WeapConfig.WeaponRange;
		const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

		ProcessInstantHit(Impact, StartTrace, ShootDir, RandomSeed, CurrentSpread);
		CurrentClip -= WeapConfig.ShotCost;

		GetWorldTimerManager().SetTimer(this, &AWeapon::Instant_Fire, WeapConfig.TimeBetweenShots, false);
		PlayWeaponSound(FireSound);
	}
	else
	{
		ReloadAmmo();
	}
	*/
}

void AWeapon::ProjectileFire()
{
	/*if (MyPawn)
	{
		if (CurrentClip > 0)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// spawn the projectile at the muzzle
			AWeaponProjectile* const Projectile = GetWorld()->SpawnActor<AWeaponProjectile>(WeaponProj, Mesh->GetSocketLocation("MF"), MyPawn->GetFollowCamera()->GetForwardVector().Rotation(), SpawnParams);
			if (Projectile)
			{

			}
			CurrentClip -= WeapConfig.ShotCost;
			GetWorldTimerManager().SetTimer(this, &AWeapon::ProjectileFire, WeapConfig.TimeBetweenShots, false);
			PlayWeaponSound(FireSound);
		}
		else
		{
			ReloadAmmo();
		}
	}
	*/
}

void AWeapon::MeleeFire()
{

}

//PLAYS WEAPON SOUND
UAudioComponent* AWeapon::PlayWeaponSound(USoundCue *Sound)
{
	UAudioComponent* AC = NULL;
	if (Sound && MyPawn)
	{
		AC = UGameplayStatics::PlaySoundAttached(Sound, MyPawn->GetRootComponent());
	}
	
	/*if (Sound && MyEnemy)
	{
		AC = UGameplayStatics::PlaySoundAttached(Sound, MyEnemy->GetRootComponent());
	}
	*/

	return AC;
}