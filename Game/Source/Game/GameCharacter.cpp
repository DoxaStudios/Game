// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Game.h"
#include "GameCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AGameCharacter

AGameCharacter::AGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(Mesh,"head");
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->AttachTo(RootComponent);

	//Player Variables
	bIsFPS = false;
	bIsSprinting = false;
	bIsInventoryOpen = false;

	WalkSpeed = 300.0f;
	RunSpeed = 700.0f;

	Stamina = 1000.0f;
	MaxStamina = 1000.0f;

	Health = 700.0f;
	MaxHealth = 1000.0f;

	Hunger = 500.0f;
	MaxHunger = 1000.0f;

	Thirst = 500.0f;
	MaxThirst = 1000.0f;

	CurrentSpeed = WalkSpeed;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	ToggleView();

	//ItemSelected = 0;
	//CurrentId = 0;

	isBleeding = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HealthFunc(DeltaTime);

	SprintFunc(DeltaTime);

	if (isBleeding == true)
	{
		BleedingFunc(DeltaTime);
	}
	SaturationLevel = (Health / MaxHealth + 0.3f);
	if (SaturationLevel > 1)
	{
		SaturationLevel = 1;
	}


	FollowCamera->PostProcessSettings.ColorSaturation = FVector(SaturationLevel, SaturationLevel, SaturationLevel);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, GetCharacterMovement()->IsWalking() ? TEXT("true") : TEXT("false"));
}

void AGameCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &AGameCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AGameCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AGameCharacter::LookUpAtRate);

	//Toggle Player View
	InputComponent->BindAction("ViewToggle", IE_Pressed, this, &AGameCharacter::ToggleView);

	//Sprinting
	InputComponent->BindAction("Sprint", IE_Pressed, this, &AGameCharacter::Sprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AGameCharacter::Walking);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &AGameCharacter::CrouchDown);
	InputComponent->BindAction("Crouch", IE_Released, this, &AGameCharacter::CrouchUp);

	InputComponent->BindAction("ADS", IE_Pressed, this, &AGameCharacter::ADSOn);
	InputComponent->BindAction("ADS", IE_Released, this, &AGameCharacter::ADSOff);


	//Debug Key
	InputComponent->BindAction("DebugKey", IE_Pressed, this, &AGameCharacter::Debug);

	InputComponent->BindAction("TacLook", IE_Pressed, this, &AGameCharacter::TacLookOn);
	InputComponent->BindAction("TacLook", IE_Released, this, &AGameCharacter::TacLookOff);

	InputComponent->BindAction("Interact", IE_Pressed, this, &AGameCharacter::Interact);

}

void AGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGameCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGameCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AGameCharacter::ToggleView()
{
	if (bIsFPS)
	{
		//Setting Camera Boom
		CameraBoom->TargetArmLength = 300.0f;
		CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		bIsFPS = false;
		//GetMesh()->bOwnerNoSee = false;
		//GetMesh()->MarkRenderStateDirty();
		//FirstPersonMesh->bOwnerNoSee = true;
		//FirstPersonMesh->MarkRenderStateDirty();
		TacLookOn();
	}
	else
	{
		//Setting Camera Boom
		CameraBoom->TargetArmLength = 0.0f;
		CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		//CameraBoom->SetRelativeLocation(FVector(20.0f, 0.0f, 60.0f));
		bIsFPS = true;
		//GetMesh()->bOwnerNoSee = true;
		//GetMesh()->MarkRenderStateDirty();
		//FirstPersonMesh->bOwnerNoSee = false;
		//FirstPersonMesh->MarkRenderStateDirty();
		TacLookOff();
	}

}

void AGameCharacter::Sprint()
{
	if (Stamina > 0)
	{
		if (GetCharacterMovement()->IsWalking())
		{
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
			bIsSprinting = true;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			bIsSprinting = false;
		}

	}

}

void AGameCharacter::Walking()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsSprinting = false;
}

bool AGameCharacter::EnableDisableKeys()
{
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	if (bIsInventoryOpen)
	{
		bIsInventoryOpen = false;
		MyController->bShowMouseCursor = false;
		MyController->bEnableClickEvents = false;
		MyController->bEnableMouseOverEvents = false;
		MyController->SetIgnoreLookInput(false);
		MyController->SetInputMode(FInputModeGameOnly());
		GetFocus();
		FollowCamera->PostProcessSettings.bOverride_DepthOfFieldMethod = false;
	}
	else
	{
		bIsInventoryOpen = true;
		MyController->bShowMouseCursor = true;
		MyController->bEnableClickEvents = true;
		MyController->bEnableMouseOverEvents = true;
		MyController->SetIgnoreLookInput(true);
		MyController->SetInputMode(FInputModeGameAndUI());
		FollowCamera->PostProcessSettings.bOverride_DepthOfFieldMethod = true;
	}

	return bIsInventoryOpen;
}

void AGameCharacter::Debug()
{
	Thirst += 100;
}

void AGameCharacter::HealthFunc(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::SanitizeFloat(Health));

	if (Health > 0 && Health < 1000)
	{
		if (Thirst >= (MaxThirst * 0.7f) && Hunger >= (MaxHunger * 0.7f))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Health Gain");
			Health += (DeltaTime * 3);
		}
		else if (Thirst <= (MaxThirst * 0.05f) && Hunger <= (MaxHunger * 0.05f))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Health Loss");
			Health -= (DeltaTime * 3);
		}
	}

	if (Health <= 1)
	{
		Health = 0;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "You is dead");
	}

	if (Hunger > MaxHunger)
	{
		Hunger = MaxHunger;
	}
	if (Thirst > MaxThirst)
	{
		Thirst = MaxThirst;
	}
}

void AGameCharacter::TacLookOn()
{
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	bUseControllerRotationYaw = false;
}

void AGameCharacter::TacLookOff()
{
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
	bUseControllerRotationYaw = true;
}

void AGameCharacter::SprintFunc(float DeltaTime)
{
	if (bIsSprinting)
	{
		Stamina -= (DeltaTime * 5);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(CurrentSpeed));
	}
	else
	{
		if (Stamina < MaxStamina)
		{
			Stamina += (DeltaTime * 2);
		}
	}
}

void AGameCharacter::Interact()
{
	Pickup();
	//Container();

}

void AGameCharacter::Pickup()
{
	PickupItemLineTrace();

}

FHitResult AGameCharacter::PickupTrace(const FVector &TraceFrom, const FVector &TraceTo) const
{
	static FName PickupTag = FName(TEXT("PickupTrace"));

	FCollisionQueryParams TraceParams(PickupTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingle(Hit, TraceFrom, TraceTo, TRACE_INVENTORY, TraceParams);

	return Hit;
}

FHitResult AGameCharacter::ContainerTrace(const FVector &TraceFrom, const FVector &TraceTo) const
{
	static FName PickupTag = FName(TEXT("ContainerTrace"));

	FCollisionQueryParams TraceParams(PickupTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingle(Hit, TraceFrom, TraceTo, TRACE_INVENTORY, TraceParams);

	return Hit;
}

void AGameCharacter::ProcessResults(const FHitResult &Impact)
{
	APickup_Item *PickupItem = Cast<APickup_Item>(Impact.GetActor());
	AContainer_Item *ContainerItem = Cast<AContainer_Item>(Impact.GetActor());

	if (ContainerItem)
	{
		if (ContainerItem->ContainerItemType == EContainerItemType::E_Backpack)
		{
			
			if (BackpackGear == NULL)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Backpack: " + ContainerItem->ItemInfo.Name);
				ContainerItem->SetOwningPawn(this);
				ContainerItem->Pickup();
				ContainerItem->ItemInfo.ItemID += itemId;
				BackpackGear = ContainerItem;
			}
			
		}
	}
	else if (PickupItem)
	{
		if (BackpackGear != NULL)
		{
			PickupItem->ItemInfo.ItemID += itemId;
			BackpackGear->Items.Add(PickupItem);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Item: " + PickupItem->ItemInfo.Name);
		}
	}
	itemId += 0.1f;

}

void AGameCharacter::PickupItemLineTrace()
{
	FVector CameraLoc;
	FRotator CameraRot;
	GetActorEyesViewPoint(CameraLoc, CameraRot);
	const FVector Dir = GetFollowCamera()->GetForwardVector();
	const FVector StartTrace = CameraLoc;
	const FVector EndTrace = StartTrace + Dir * 300;

	const FHitResult Impact = PickupTrace(StartTrace, EndTrace);

	DrawDebugLine(this->GetWorld(), StartTrace, EndTrace, FColor::Black, true, 10000, 10.f);


	ProcessResults(Impact);

}

void AGameCharacter::BleedingFunc(float DeltaTime)
{
	if (Health < MaxHealth)
	{
		Health -= (DeltaTime * 2);
	}
	else if (Health < 0)
	{
		Health = 0;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, FString::SanitizeFloat(Health));
}

void AGameCharacter::CrouchUp()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 98.0f);
}

void AGameCharacter::CrouchDown()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 68.0f);
}

void AGameCharacter::Death()
{
	GetMesh()->SetSimulatePhysics(true);
	//DeathScreen();
	//GetCapsuleComponent()->InitCapsuleSize(1.0f, 1.0f);
}

void AGameCharacter::ADSOn()
{
	CameraBoom->SetRelativeLocation(FVector(4.0f, 10.0f, 6.5f));
}

void AGameCharacter::ADSOff()
{
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
}
/*
//AMasterItem *Item = Cast<AMasterItem>(Impact.GetActor());
AInventoryItems *InventoryItem = Cast<AInventoryItems>(Impact.GetActor());
AWeapon *Weapon = Cast<AWeapon>(Impact.GetActor());
AContainer *ContainerItem = Cast<AContainer>(Impact.GetActor());
SavedContainer = ContainerItem;



if (InventoryItem)
{
InventoryItem->ItemInfo.ItemCache += CurrentId;
if (InventoryItem->ItemInfo.bIsShirt == true)
{
if (ShirtGear == NULL)
{
ShirtGear = InventoryItem;
//ShirtGear->ItemInfo.Name = InventoryItem->ItemInfo.Name;
//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, "Added Shirt");
}
else
{
//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, "Another Shirt in place");
}
}
else if (InventoryItem->ItemInfo.bIsPants == true)
{
if (PantsGear == NULL)
{
PantsGear = InventoryItem;
}
else
{

}
}
else if (InventoryItem->ItemInfo.bIsBackpack == true)
{
if (Backpack == NULL)
{
Backpack = InventoryItem;
AttatchItem(InventoryItem);
CurrentId += 0.1f;
return;
}
else
{

}
}
else
{
//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, "Another Shirt in place");
}
//InventoryItems.Add(InventoryItem->ItemInfo);
CurrentId += 0.1f;

InventoryItem->SetActorHiddenInGame(true);
InventoryItem->SetActorEnableCollision(false);
//InventoryItem->SetActorHiddenInGame(false);
//InventoryItem->SetActorEnableCollision(true);
//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, "You just picked up a " + InventoryItem->ItemInfo.Name);

}

else if (Weapon)
{
Weapon->ItemInfo.ItemCache += CurrentId;
Weapon->ItemInfo.Reference = Weapon;

if (ShirtGear != NULL)
{
Weapon->ItemInfo.ItemCache += CurrentId;
ShirtGear->ItemInventory.Add(Item->ItemInfo);
}
else if (PantsGear != NULL)
{
Weapon->ItemInfo.ItemCache += CurrentId;
PantsGear->ItemInventory.Add(Item->ItemInfo);
}
else if (Backpack != NULL)
{
Weapon->ItemInfo.ItemCache += CurrentId;
Backpack->ItemInventory.Add(Item->ItemInfo);
}
else if (Hand.ItemCache == NULL)
{
Weapon->ItemInfo.ItemCache += CurrentId;
Hand = Weapon->ItemInfo;
UpdateHand();
}
else
{
return;
}
class UBoxComponent *Collision = Weapon->GetCollisionComp();
Collision->SetSimulatePhysics(false);
Weapon->SetActorHiddenInGame(true);
Weapon->SetActorEnableCollision(false);

}

else if (Item)
{
//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, "Item")

if (ShirtGear != NULL)
{
Item->ItemInfo.ItemCache += CurrentId;
ShirtGear->ItemInventory.Add(Item->ItemInfo);
}
else if (PantsGear != NULL)
{
Item->ItemInfo.ItemCache += CurrentId;
PantsGear->ItemInventory.Add(Item->ItemInfo);
}
else if (Backpack != NULL)
{
Item->ItemInfo.ItemCache += CurrentId;
Backpack->ItemInventory.Add(Item->ItemInfo);
}
else
{
return;
}

CurrentId += 0.1f;
//Item->ItemInfo.ItemCache = InventoryItems.Find(Item->ItemInfo);
//Item->ItemInfo.ItemCache = CurrentId;

Item->Destroy();
//Item->SetActor

//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, "You just picked up a " + Item->ItemInfo.Name);
}
else if (ContainerItem)
{
//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, "You opened a container");
OpenContainer();
}
*/
