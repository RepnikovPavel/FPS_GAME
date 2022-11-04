// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterFX.h"

// Sets default values
AWaterFX::AWaterFX()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaterFX::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaterFX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

