// Fill out your copyright notice in the Description page of Project Settings.

#include "Game.h"
#include "Pickup_Item.h"

APickup_Item::APickup_Item(const FObjectInitializer& ObjectInitializer)
{
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "Mesh");
	RootComponent = Mesh;
}


