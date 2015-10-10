// Fill out your copyright notice in the Description page of Project Settings.

#include "Game.h"
#include "../GameCharacter.h"
#include "Container_Item.h"

AContainer_Item::AContainer_Item(const FObjectInitializer& ObjectInitializer)
{
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, "CollisionComp");
	RootComponent = CollisionComp;

	Mesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, "Mesh");
	Mesh->AttachTo(RootComponent);
}

void AContainer_Item::SetOwningPawn(AGameCharacter *NewOwner)
{
	//if MyPawn does not equal argument parameter
	if (Owner != NewOwner)
	{
		//make the player the new owner
		Instigator = NewOwner;
		Owner = NewOwner;
	}
}

void AContainer_Item::Drop()
{

}

void AContainer_Item::Pickup()
{
	if (ContainerItemType == EContainerItemType::E_Backpack)
	{
		USkeletalMeshComponent *Character = Owner->GetMesh();
		Mesh->AttachTo(Character, "backpack");
	}
}