// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include "WaterFX.generated.h"

UCLASS()
class FPS_API AWaterFX : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaterFX();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	int Nx=10;
	UPROPERTY(EditAnywhere)
	int Ny=10;
	UPROPERTY(EditAnywhere)
	int Nz=10;
	UPROPERTY(EditAnywhere)
	int N_surf=5;
	UPROPERTY(EditAnywhere)
	double ax=-100.0;
	UPROPERTY(EditAnywhere)
	double bx=100.0;
	UPROPERTY(EditAnywhere)
	double ay=-100.0;
	UPROPERTY(EditAnywhere)
	double by=-100.0;
	UPROPERTY(EditAnywhere)
	double az=-100.0;
	UPROPERTY(EditAnywhere)
	double bz=-100.0;
	UPROPERTY(EditAnywhere)
	double ro=1000.0;

	UPROPERTY(EditAnywhere)
	double StartCullDistance=0.0;
	UPROPERTY(EditAnywhere)  
	double EndCullDistance=10000.0;

public:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* MeshPtr;
	
	// UStaticMesh* SMeshPtr;
public:
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	std::vector<std::vector<std::vector<int>>> I;
	void ReserveMemoryForI();
private:
	double Lx;
	double Ly;
	double Lz;
	double hx;
	double hy;
	double hz;
	double V;
	int N;
	double V_kij;
	double m_kij;
};
