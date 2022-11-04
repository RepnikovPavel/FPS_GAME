// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionFX.h"


void AExplosionFX::SetWindDirection(int x, int y)
{
	_wind_direction[0] = x;
	_wind_direction[1] = y;
}
void AExplosionFX::MakeSphere()
{
	
	//EVertexes.Add();
	//EIndexes.Add();
}

AExplosionFX::AExplosionFX()
{
	PrimaryActorTick.bCanEverTick = true;
	ExplosionPMC_ptr = CreateDefaultSubobject<UProceduralMeshComponent>("ExplosionPMC_ptr");
	ExplosionPMC_ptr->SetupAttachment(GetRootComponent());
}


void AExplosionFX::BeginPlay()
{
	Super::BeginPlay();
	
}

void AExplosionFX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


