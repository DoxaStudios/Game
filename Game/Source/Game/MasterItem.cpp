// Fill out your copyright notice in the Description page of Project Settings.

#include "Game.h"
#include "GameCharacter.h"
#include "MasterItem.h"


// Sets default values
AMasterItem::AMasterItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMasterItem::BeginPlay()
{
	Super::BeginPlay();
	
}


