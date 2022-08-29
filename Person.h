#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <string>
#include "LoggingSystem.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Person.generated.h"

#define CHECK_SUCCESS_OF_LOAD_DATA

#define PATH_SKELETAL_MESH_ARMS "/Game/FPS_Content/Assets/Characters/Arms/Mesh/Mannequin/Mesh/SK_Mannequin_Arms"
#define PATH_ANIMATION_ASSET_FPS_IDLE "/Game/FPS_Content/Assets/Characters/Arms/Arms_Animations/Arms_idle"


const char* BoolToCStr(bool bool_value);


UCLASS()
class FPS_API APerson : public ACharacter
{
	GENERATED_BODY()
private:
	//components:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponentPtr;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponentPtr; 

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponentPtr;

	//assets:
	UPROPERTY(VisibleAnywhere)
	UAnimationAsset* AnimationAssetPtr; 

	//getters:
protected:
	USkeletalMeshComponent* GetMyMesh() const;
	USpringArmComponent* GetMySpringArm() const;
public:
	APerson();
	//ctor functions:
private:
	void LoadAndSetSkeletalMesh(const char* PathToSkeletalMesh);
	void AssignSpringArmVaribles(const FVector& RelLocation,const FRotator& RelRotation,const double ArmLength, const double CameraLagSpeed);
	void LoadAnimationAsset(const char* PathToAnimationAsset);
	//begin play fucntions:
private:
	//animation callback functions:
	void PlayFPSIdle();
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

template<typename TypeToLoad>
class Loader
{
public:
	Loader(const FString& SourcePath)
	{
		ConstructorHelpers::FObjectFinder<TypeToLoad> Finder(*SourcePath);
		if (Finder.Succeeded())
		{
			_DataPtr = Finder.Object;
		}
#ifdef CHECK_SUCCESS_OF_LOAD_DATA
		LoggingSystem* LogPtr = LoggingSystem::Instance(LOG_FILE_PATH);
		std::string OutputString;
		OutputString += TCHAR_TO_ANSI(*SourcePath);
		OutputString+=" loading ";
		OutputString+=BoolToCStr(Finder.Succeeded());
		OutputString+="\n";
		LogPtr->WriteToLog(OutputString);
#endif
	}
public:
	TypeToLoad* GetDataPtr()
	{
		return _DataPtr;
	}
private:
	TypeToLoad* _DataPtr;
};