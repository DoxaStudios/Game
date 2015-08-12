// Fill out your copyright notice in the Description page of Project Settings.

#include "Game.h"
#include "MasterItem.h"


// Sets default values
AMasterItem::AMasterItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "Mesh");
	//Mesh->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void AMasterItem::BeginPlay()
{
	Super::BeginPlay();
	
}