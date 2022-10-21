#include "Person.h"
#include <string>
#include "LoggingSystem.h"


#define M_ACM_Jump "Jump"
#define M_ACM_MoveForward "MoveForward"
#define M_ACM_MoveBack "MoveBack"
#define M_ACM_MoveRight "MoveRight"
#define M_ACM_MoveLeft "MoveLeft"

#define M_AXM_TurnCameraX "TurnCameraX"
#define M_AXM_TurnCameraY "TurnCameraY"
#define M_AXM_ChangeSpringArmLength "ChangeSpringArmLength"

#define PATH_SKELETAL_MESH_ARMS "/Game/FPS_Content/Assets/Characters/Arms/Mesh/Mannequin/Mesh/SK_Mannequin_Arms"
#define PATH_ANIMATION_ASSET_FPS_IDLE "/Game/FPS_Content/Assets/Characters/Arms/Arms_Animations/Arms_idle"

#define ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
#define M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM 0.125

#define CHECK_SUCCESS_OF_LOAD_DATA

APerson::APerson()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponentPtr = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SkeletalMeshComponentPtr->SetupAttachment(GetRootComponent());
	
	SpringArmComponentPtr=CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponentPtr->SetupAttachment(GetMyMesh());
	
	CameraComponentPtr = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponentPtr->SetupAttachment(GetMySpringArm());
	
	LoadAndSetSkeletalMesh(PATH_SKELETAL_MESH_ARMS);
	AssignSpringArmVaribles(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f),init_arm_lenght,3.0f);
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

const char* BoolToCStr(bool bool_value)
{
	return (bool_value?"true":"false");
}

void APerson::ScreenLog::PrintMessage(FString message, float duration, FColor color)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, duration, color, message);
	}
}


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
	PlayerInputComponent->BindAction(M_ACM_MoveForward,IE_Pressed,this,&APerson::ACM_MoveForward);
	PlayerInputComponent->BindAction(M_ACM_MoveBack,IE_Pressed,this,&APerson::ACM_MoveBack);
	PlayerInputComponent->BindAction(M_ACM_MoveRight,IE_Pressed,this,&APerson::ACM_MoveRight);
	PlayerInputComponent->BindAction(M_ACM_MoveLeft,IE_Pressed,this,&APerson::ACM_MoveLeft);
	
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
	screen_log.PrintMessage(M_ACM_Jump, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
	
}
	//axis mappings:
void APerson::ACM_MoveForward()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(M_ACM_MoveForward, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
}
void APerson::ACM_MoveBack()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(M_ACM_MoveBack, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
}
void APerson::ACM_MoveRight()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(M_ACM_MoveRight, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
}
void APerson::ACM_MoveLeft()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(M_ACM_MoveLeft, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
}

//camera controll callback functions:
	//axis mappings:
void APerson::AXM_ChangeSpringArmLength(float AxisValue)
{
	if(AxisValue==0.0){return;}
	if (AxisValue>0.0)
	{
		if(SpringArmComponentPtr->TargetArmLength<(min_arm_lenght+50.0))
		{
			SpringArmComponentPtr->TargetArmLength = 0.0;
		}
		else
		{
			SpringArmComponentPtr->TargetArmLength = (SpringArmComponentPtr->TargetArmLength <= min_arm_lenght? min_arm_lenght:SpringArmComponentPtr->TargetArmLength/speed_of_changing_arm_length);
		}
	}
	if(AxisValue<0.0)
	{
		if (SpringArmComponentPtr->TargetArmLength<(min_arm_lenght+50.0))
		{
			SpringArmComponentPtr->TargetArmLength += 50.0;
		}
		else
		{
			SpringArmComponentPtr->TargetArmLength = (SpringArmComponentPtr->TargetArmLength >= max_arm_lenght? max_arm_lenght:SpringArmComponentPtr->TargetArmLength*speed_of_changing_arm_length);
		}
		
	}
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


