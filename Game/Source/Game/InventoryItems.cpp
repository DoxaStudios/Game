// Fill out your copyright notice in the Description page of Project Settings.

#include "Game.h"
#include "InventoryItems.h"

AInventoryItems::AInventoryItems(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}