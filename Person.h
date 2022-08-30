// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Person.generated.h"


UCLASS()
class FPS_API APerson : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APerson();
	
	//ctor functions:
private:
	void LoadAndSetSkeletalMesh(const char* PathToSkeletalMesh);
	void AssignSpringArmVaribles(const FVector& RelLocation,const FRotator& RelRotation,const double ArmLength, const double CameraLagSpeed);
	void LoadAnimationAsset(const char* PathToAnimationAsset);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	//movement callback functions:
		//action mappings:
	void ACM_Jump();
		//axis mappings:
	void AXM_MoveForward(float AxisValue);
	void AXM_MoveRight(float AxisValue);
	
	//camera controll callback fucntions:
		//axis mappings:	
	void AXM_ChangeSpringArmLength(float AxisValue);
	void AXM_TurnCameraX(float AxisValue);
	void AXM_TurnCameraY(float AxisValue);
	
	//animation callback functions:
	void PlayFPSIdle();

	
	
	//getters:
private:
	USkeletalMeshComponent* GetMyMesh() const;
	USpringArmComponent* GetMySpringArm() const;
private:
	//components:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponentPtr;
	
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArmComponentPtr; 

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponentPtr;

	//assets:
	UPROPERTY(VisibleAnywhere)
	UAnimationAsset* AnimationAssetPtr;
};
