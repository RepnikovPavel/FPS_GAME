// Fill out your copyright notice in the Description page of Project Settings.


#include "Person.h"
#include <string>
#include "LoggingSystem.h"

#define ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
#define CHECK_SUCCESS_OF_LOAD_DATA

#define M_ACM_Jump "Jump"
#define M_AXM_MoveForward "MoveForward"
#define M_AXM_MoveRight "MoveRight"
#define M_AXM_TurnCameraX "TurnCameraX"
#define M_AXM_TurnCameraY "TurnCameraY"
#define M_AXM_ChangeSpringArmLength "ChangeSpringArmLength"

#define PATH_SKELETAL_MESH_ARMS "/Game/FPS_Content/Assets/Characters/Arms/Mesh/Mannequin/Mesh/SK_Mannequin_Arms"
#define PATH_ANIMATION_ASSET_FPS_IDLE "/Game/FPS_Content/Assets/Characters/Arms/Arms_Animations/Arms_idle"

APerson::APerson()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponentPtr = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SkeletalMeshComponentPtr->SetupAttachment(GetRootComponent());
	
	SpringArmComponentPtr=CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponentPtr->SetupAttachment(GetMyMesh());
	
	CameraComponentPtr = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponentPtr->SetupAttachment(GetMySpringArm());
	
	LoadAndSetSkeletalMesh(PATH_SKELETAL_MESH_ARMS);
	AssignSpringArmVaribles(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f),400.0f,3.0f);
	LoadAnimationAsset(PATH_ANIMATION_ASSET_FPS_IDLE);
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// helper classes:
const char* BoolToCStr(bool bool_value);

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

// Ctor functions
void APerson::LoadAndSetSkeletalMesh(const char* PathToSkeletalMesh)
{
	Loader<USkeletalMesh> Loader(FString(strlen(PathToSkeletalMesh),PathToSkeletalMesh));
	GetMyMesh()->SetSkeletalMesh(Loader.GetDataPtr());
}
void APerson::AssignSpringArmVaribles(const FVector& RelLocation,const FRotator& RelRotation,const double ArmLength, const double CameraLagSpeed)
{
	SpringArmComponentPtr->SetRelativeLocationAndRotation(RelLocation,RelRotation);
	SpringArmComponentPtr->TargetArmLength = ArmLength;
	if (CameraLagSpeed != 0.0)
	{
		SpringArmComponentPtr->bEnableCameraLag = true;
		SpringArmComponentPtr->CameraLagSpeed= CameraLagSpeed;
	}
	
}
void APerson::LoadAnimationAsset(const char* PathToAnimationAsset)
{
	Loader<UAnimationAsset> Loader(FString(strlen(PathToAnimationAsset),PathToAnimationAsset));
	AnimationAssetPtr = Loader.GetDataPtr();
}
//


void APerson::BeginPlay()
{
	Super::BeginPlay();
	
}

void APerson::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APerson::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// movement callback functions:
	PlayerInputComponent->BindAction(M_ACM_Jump,IE_Pressed,this,&APerson::ACM_Jump);
	PlayerInputComponent->BindAxis(M_AXM_MoveForward,this,&APerson::AXM_MoveForward);
	PlayerInputComponent->BindAxis(M_AXM_MoveRight,this,&APerson::AXM_MoveRight);

	//camera controll callback fucntions:
	PlayerInputComponent->BindAxis(M_AXM_ChangeSpringArmLength,this,&APerson::AXM_ChangeSpringArmLength);
	PlayerInputComponent->BindAxis(M_AXM_TurnCameraX,this,&APerson::AXM_TurnCameraX);
	PlayerInputComponent->BindAxis(M_AXM_TurnCameraY,this,&APerson::AXM_TurnCameraY);
	
	// animations callback funstions functionality
	PlayerInputComponent->BindAction("IDLE_full_stamina",IE_Pressed,this,&APerson::PlayFPSIdle);

}

//movement callback functions:
	//action mappings:
void APerson::ACM_Jump()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,"ACM Jump");
	}
#endif
}
	//axis mappings:
void APerson::AXM_MoveForward(float AxisValue)
{
	
}
void APerson::AXM_MoveRight(float AxisValue)
{
	
}

//camera controll callback functions:
	//axis mappings:
void APerson::AXM_ChangeSpringArmLength(float AxisValue)
{
	
}
void APerson::AXM_TurnCameraX(float AxisValue)
{
	
}
void APerson::AXM_TurnCameraY(float AxisValue)
{

}

//animations callback functions:
void APerson::PlayFPSIdle()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,"playing FPS idle");
	}
#endif
	GetMyMesh()->PlayAnimation(AnimationAssetPtr,true);
}

//getters:
USkeletalMeshComponent* APerson::GetMyMesh() const
{
	return SkeletalMeshComponentPtr;
}
USpringArmComponent* APerson::GetMySpringArm() const
{
	return SpringArmComponentPtr;
}

const char* BoolToCStr(bool bool_value)
{
	return (bool_value?"true":"false");
}