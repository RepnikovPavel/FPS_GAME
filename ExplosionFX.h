// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include <array>
#include <vector>
#include "ExplosionFX.generated.h"

UCLASS()
class FPS_API AExplosionFX : public AActor
{
	GENERATED_BODY()
	
public:	
	AExplosionFX();
	
		
protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ExplosionPMC_ptr;
public:
	UFUNCTION(BlueprintCallable)
	void SetWindDirection(int x, int y);

	UPROPERTY(EditAnywhere)
	double _R;
	UPROPERTY(EditAnywhere)
	int _depth;
private:
	void MakeSphere();
	
	std::array<int,2> _wind_direction{0,0};
	TArray<FVector> EVertexes;
	TArray<int> EIndexes;
	std::vector<int> NIndexes;
};
