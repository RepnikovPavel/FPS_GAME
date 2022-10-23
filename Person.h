// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <array>
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
	void AssignSpringArmVaribles(const FVector& RelLocation,const FRotator& RelRotation,const float ArmLength, const float CameraLagSpeed, const float CameraRotationLagSpeed);
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

	//movement physics
	class MovementState
	{
		//CPVM is current possible velocity modulus
	public:
		double GetCPVM();
		void ChangeCPVM(size_t new_v_state_index);//
		std::array<signed short,2> _xy_pair{0,0};
		bool is_v_changed{false};
		double current_angle{0.0};
		void UpdateCurrentAngle();
	private:
		std::array<double,3> _possible_speeds{194.444,250.000,361.111};//sm/sek
		size_t _current_possible_veclocity_index{1};
	};
	MovementState movement_state;

	
	
	//movement callback functions:
		//action mappings:
	void ACM_Jump();
	void ACM_MoveForward();
	void ACM_StopMoveForward();
	void ACM_MoveBack();
	void ACM_StopMoveBack();
	void ACM_MoveRight();
	void ACM_StopMoveRight();
	void ACM_MoveLeft();
	void ACM_StopMoveLeft();
	void ACM_Shift();
	void ACM_StopShift();
	void ACM_Ctrl();
	void ACM_StopCtrl();
	
	//camera controll callback fucntions:
	void AXM_ChangeSpringArmLength(float AxisValue);
	// mixed actions:
	void AXM_TurnCameraX(float AxisValue);
	void AXM_TurnCameraY(float AxisValue);
	void RMB_pressed();
	void RMB_released();

public:
	UPROPERTY(EditAnywhere,Category="SpringArmLag")
	float camera_lag_speed = 10.0f;

	UPROPERTY(EditAnywhere, Category="SpringArmLag")
	float camera_rotation_lag_speed = 10.0f;
private:

	class CameraState
	{
	public:
		const std::array<float,5> lenghts{200.0,450.0,650.0,850.0,1050.0};
		size_t current_pos{1};
		const std::array<float,5> max_thetas{45.0f,15.0f,10.0f,5.0f,0.0f};
		const std::array<float,5> min_thetas{-85.0f,-85.0f,-85.0f,-85.0f,-85.0f};
	};
	CameraState camera_state;
	// mouse state
	UPROPERTY(EditAnywhere,Category="RotateSpringArm")
	float mouse_sensitivity = 1.0f;
	
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

private:
	struct ScreenLog
	{
		void PrintMessage(FString message, float duration, FColor color);
	};
	ScreenLog screen_log;
};
