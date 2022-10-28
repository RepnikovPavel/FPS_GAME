#include "Person.h"
#include <string>
#include <Math/Rotator.h>
#include <cmath>
#include "LoggingSystem.h"
#include "../../Plugins/Developer/RiderLink/Source/RD/thirdparty/variant/include/mpark/lib.hpp"


#define M_ACM_Jump					"Jump"
#define M_ACM_MoveForward			"MoveForward"
#define M_ACM_MoveBack				"MoveBack"
#define M_ACM_MoveRight				"MoveRight"
#define M_ACM_MoveLeft				"MoveLeft"
#define M_ACM_RMB					"RMB"
#define M_ACM_Shift					"Shift"
#define M_ACM_Ctrl					"Ctrl"

#define M_AXM_TurnCameraX			"TurnCameraX"
#define M_AXM_TurnCameraY			"TurnCameraY"

#define M_AXM_ChangeSpringArmLength	"ChangeSpringArmLength"

#define PATH_SKELETAL_MESH_ARMS "/Game/FPS_Content/Assets/Characters/Arms/Mesh/Mannequin/Mesh/SK_Mannequin_Arms"
#define PATH_ANIMATION_ASSET_FPS_IDLE "/Game/FPS_Content/Assets/Characters/Arms/Arms_Animations/Arms_idle"

#define ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
#define M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM 0.125

#define CHECK_SUCCESS_OF_LOAD_DATA

APerson::APerson()
{
	PrimaryActorTick.bCanEverTick = true;

	// CapsuleComponentPtr = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent",true);
	// CapsuleComponentPtr->SetupAttachment(GetRootComponent());
	
	SkeletalMeshComponentPtr = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent",true);
	SkeletalMeshComponentPtr->SetupAttachment(GetRootComponent());
	
	SpringArmComponentPtr=CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent",true);
	SpringArmComponentPtr->SetupAttachment(GetMyMesh());
	
	CameraComponentPtr = CreateDefaultSubobject<UCameraComponent>("CameraComponent",true);
	CameraComponentPtr->SetupAttachment(GetMySpringArm());
	
	LoadAndSetSkeletalMesh(PATH_SKELETAL_MESH_ARMS);
	
	LoadAnimationAsset(PATH_ANIMATION_ASSET_FPS_IDLE);
	
	// AutoPossessPlayer = EAutoReceiveInput::Player0;
	
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
void APerson::AssignSpringArmVaribles(const FVector& RelLocation,const FRotator& RelRotation,const float ArmLength, const float CameraLagSpeed, const float CameraRotationLagSpeed)
{
	SpringArmComponentPtr->SetRelativeLocationAndRotation(RelLocation,RelRotation);
	SpringArmComponentPtr->TargetArmLength = ArmLength;
	if (CameraLagSpeed != 0.0)
	{
		SpringArmComponentPtr->bEnableCameraLag = true;
		SpringArmComponentPtr->CameraLagSpeed= CameraLagSpeed;
	}
	if (CameraRotationLagSpeed!=0.0)
	{
		SpringArmComponentPtr->bEnableCameraRotationLag=true;
		SpringArmComponentPtr->CameraRotationLagSpeed = CameraRotationLagSpeed;
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

	// CapsuleComponentPtr = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent",true);
	// CapsuleComponentPtr->SetupAttachment(GetRootComponent());
	//
	// SkeletalMeshComponentPtr = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent",true);
	// SkeletalMeshComponentPtr->SetupAttachment(GetRootComponent());
	//
	// SpringArmComponentPtr=CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent",true);
	// SpringArmComponentPtr->SetupAttachment(GetMyMesh());
	//
	// CameraComponentPtr = CreateDefaultSubobject<UCameraComponent>("CameraComponent",true);
	// CameraComponentPtr->SetupAttachment(GetMySpringArm());
	//
	// LoadAndSetSkeletalMesh(PATH_SKELETAL_MESH_ARMS);
	// LoadAnimationAsset(PATH_ANIMATION_ASSET_FPS_IDLE);
	
	AssignSpringArmVaribles(FVector(0.0f, 0.0f, 50.0f), FRotator(-30.0f, 0.0f, 0.0f),camera_state.lenghts[camera_state.current_pos],camera_lag_speed,camera_rotation_lag_speed);

	// //bebug
	// CapsuleComponentPtr->SetVisibility(true);
	// CapsuleComponentPtr->SetHiddenInGame(false);

}

void APerson::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//movement physics part
	signed short x = movement_state._xy_pair[0];
	signed short y = movement_state._xy_pair[1];
	if ((x==0&&y==0)==false)
	{
		if (movement_state.is_v_changed==false)
		{
			AddActorLocalOffset(movement_state.GetCPVM()*(GetActorForwardVector().RotateAngleAxis(movement_state.current_angle,FVector(0,0,-1)))*DeltaTime);
		}
		else
		{
			movement_state.UpdateCurrentAngle();
			AddActorLocalOffset(movement_state.GetCPVM()*(GetActorForwardVector().RotateAngleAxis(movement_state.current_angle,FVector(0,0,-1)))*DeltaTime);
			movement_state.is_v_changed=false;	
		}
	}

}

void APerson::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// movement callback functions:
	PlayerInputComponent->BindAction(M_ACM_Jump,IE_Pressed,this,&APerson::ACM_Jump);
	PlayerInputComponent->BindAction(M_ACM_MoveForward,IE_Pressed,this,&APerson::ACM_MoveForward);
	PlayerInputComponent->BindAction(M_ACM_MoveForward,IE_Released,this,&APerson::ACM_StopMoveForward);
	PlayerInputComponent->BindAction(M_ACM_MoveBack,IE_Pressed,this,&APerson::ACM_MoveBack);
	PlayerInputComponent->BindAction(M_ACM_MoveBack,IE_Released,this,&APerson::ACM_StopMoveBack);
	PlayerInputComponent->BindAction(M_ACM_MoveRight,IE_Pressed,this,&APerson::ACM_MoveRight);
	PlayerInputComponent->BindAction(M_ACM_MoveRight,IE_Released,this,&APerson::ACM_StopMoveRight);
	PlayerInputComponent->BindAction(M_ACM_MoveLeft,IE_Pressed,this,&APerson::ACM_MoveLeft);
	PlayerInputComponent->BindAction(M_ACM_MoveLeft,IE_Released,this,&APerson::ACM_StopMoveLeft);
	PlayerInputComponent->BindAction(M_ACM_Shift,IE_Pressed,this,&APerson::ACM_Shift);
	PlayerInputComponent->BindAction(M_ACM_Shift,IE_Released,this,&APerson::ACM_StopShift);
	PlayerInputComponent->BindAction(M_ACM_Ctrl,IE_Pressed,this,&APerson::ACM_Ctrl);
	PlayerInputComponent->BindAction(M_ACM_Ctrl,IE_Released,this,&APerson::ACM_StopCtrl);
	
	//camera controll callback fucntions:
	PlayerInputComponent->BindAxis(M_AXM_ChangeSpringArmLength,this,&APerson::AXM_ChangeSpringArmLength);
	PlayerInputComponent->BindAxis(M_AXM_TurnCameraX,this,&APerson::AXM_TurnCameraX);
	PlayerInputComponent->BindAxis(M_AXM_TurnCameraY,this,&APerson::AXM_TurnCameraY);
	// animations callback funstions functionality
	PlayerInputComponent->BindAction("IDLE_full_stamina",IE_Pressed,this,&APerson::PlayFPSIdle);

	PlayerInputComponent->BindAction(M_ACM_RMB, IE_Pressed,this,&APerson::RMB_pressed);
	PlayerInputComponent->BindAction(M_ACM_RMB, IE_Released,this,&APerson::RMB_released);
} 

double APerson::MovementState::GetCPVM()
{
	return _possible_speeds[_current_possible_veclocity_index];
}
void APerson::MovementState::ChangeCPVM(size_t new_v_state_index)
{
	if (new_v_state_index<_possible_speeds.size()&&new_v_state_index >= 0)
	{
		_current_possible_veclocity_index = new_v_state_index;
	}	
}

void APerson::MovementState::UpdateCurrentAngle()
{
	signed short x = _xy_pair[0];
	signed short y = _xy_pair[1];
	if (x==0&&y==1)
	{
		current_angle = 0.0;
	}
	else if (x==1&&y==1)
	{
		current_angle=-45.0;
	}
	else if (x==-1&&y==1)
	{
		current_angle=45.0;
	}
	else if (x==1&&y==0)
	{
		current_angle=-90.0;
	}
	else if (x==-1&&y==0)
	{
		current_angle=90.0;
	}
	else if(x==1&&y==-1)
	{
		current_angle=-135.0;
	}
	else if(x==-1&&y==-1)
	{
		current_angle=135.0;
	}
	else if(x==0&&y==-1)
	{
		current_angle=180.0;
	}
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
	movement_state._xy_pair[1]+=1;
	movement_state.is_v_changed=true;
}
void APerson::ACM_StopMoveForward()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(FString("stop ")+M_ACM_MoveForward, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
	movement_state._xy_pair[1]-=1;
	movement_state.is_v_changed=true;
}
void APerson::ACM_MoveBack()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(M_ACM_MoveBack, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
	movement_state._xy_pair[1]-=1;
	movement_state.is_v_changed=true;
}
void APerson::ACM_StopMoveBack()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(FString("stop ")+M_ACM_MoveBack, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
	movement_state._xy_pair[1]+=1;
	movement_state.is_v_changed=true;
}

void APerson::ACM_MoveRight()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(M_ACM_MoveRight, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
	movement_state._xy_pair[0]+=1;
	movement_state.is_v_changed=true;
}
void APerson::ACM_StopMoveRight()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(FString("stop")+M_ACM_MoveRight, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
	movement_state._xy_pair[0]-=1;
	movement_state.is_v_changed=true;
}
void APerson::ACM_MoveLeft()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(M_ACM_MoveLeft, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
	movement_state._xy_pair[0]-=1;
	movement_state.is_v_changed=true;
}
void APerson::ACM_StopMoveLeft()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(FString("stop")+M_ACM_MoveLeft, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
	movement_state._xy_pair[0]+=1;
	movement_state.is_v_changed=true;
}
void APerson::ACM_Shift()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(M_ACM_Shift, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
	
}
void APerson::ACM_StopShift()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(FString("stop")+M_ACM_Shift, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
	
}
void APerson::ACM_Ctrl()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(M_ACM_Ctrl, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
	
}
void APerson::ACM_StopCtrl()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(FString("stop")+M_ACM_Ctrl, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
	
}
//camera controll callback functions:
	//axis mappings:
void APerson::AXM_ChangeSpringArmLength(float AxisValue)
{
	
	if(AxisValue==0.0){return;}
	if (AxisValue>0.0)
	{
		if (camera_state.current_pos>0)
		{
			camera_state.current_pos -= 1;
			SpringArmComponentPtr->TargetArmLength=camera_state.lenghts[camera_state.current_pos];
			screen_log.PrintMessage(FString::SanitizeFloat(SpringArmComponentPtr->TargetArmLength),0.125f,FColor::White);
		}
	}
	if(AxisValue<0.0)
	{
		if (camera_state.current_pos<camera_state.lenghts.size()-1)
		{
			camera_state.current_pos+=1;
			SpringArmComponentPtr->TargetArmLength=camera_state.lenghts[camera_state.current_pos];
			screen_log.PrintMessage(FString::SanitizeFloat(SpringArmComponentPtr->TargetArmLength),0.125f,FColor::White);
		}
	}
	auto current_theta_rotation = SpringArmComponentPtr->GetRelativeRotation().Pitch;
	if (current_theta_rotation<camera_state.min_thetas[camera_state.current_pos])
	{
		SpringArmComponentPtr->SetRelativeRotation(FRotator(camera_state.min_thetas[camera_state.current_pos],0.0f,0.0f));
	}
	else if(current_theta_rotation>camera_state.max_thetas[camera_state.current_pos])
	{
		SpringArmComponentPtr->SetRelativeRotation(FRotator(camera_state.max_thetas[camera_state.current_pos],0.0f,0.0f));
	}
	
}
void APerson::AXM_TurnCameraX(float AxisValue)
{
	if (AxisValue!=0.0f)
	{
		if (camera_state.free_camera_mod==true)
		{
			SpringArmComponentPtr->AddRelativeRotation(FRotator(0.0f,AxisValue*mouse_sensitivity,0.0f));
		}
		else
		{
			SkeletalMeshComponentPtr->AddLocalRotation(FRotator(0.0f,AxisValue*mouse_sensitivity,0.0f));
			// SkeletalMeshComponentPtr->AddRelativeRotation(FRotator(0.0f,AxisValue*mouse_sensitivity,0.0f));
			// SpringArmComponentPtr->AddLocalRotation(FRotator(0.0f,AxisValue*mouse_sensitivity,0.0f));
		}
	}
}
void APerson::AXM_TurnCameraY(float AxisValue)
{
	if (AxisValue!=0.0f)
	{
		if (camera_state.free_camera_mod==true)
		{
			const FRotator current_camera_pos = SpringArmComponentPtr->GetRelativeRotation();
			if (AxisValue<0.0f)
			{
				if (current_camera_pos.Pitch>camera_state.min_thetas[camera_state.current_pos])
				{
					const double new_camera_pos = current_camera_pos.Pitch+AxisValue*mouse_sensitivity;
					if (new_camera_pos>camera_state.min_thetas[camera_state.current_pos])
					{
						SpringArmComponentPtr->AddRelativeRotation(FRotator(AxisValue*mouse_sensitivity,0.0f,0.0f));
					}
					else
					{
						SpringArmComponentPtr->SetRelativeRotation(FRotator(camera_state.min_thetas[camera_state.current_pos],current_camera_pos.Yaw,current_camera_pos.Roll));
					}
				}
			}
			else if (AxisValue>0.0f)
			{
				if (current_camera_pos.Pitch<camera_state.max_thetas[camera_state.current_pos])
				{
					const double new_camera_pos = current_camera_pos.Pitch+AxisValue*mouse_sensitivity;
					if (new_camera_pos<camera_state.max_thetas[camera_state.current_pos])
					{
						SpringArmComponentPtr->AddRelativeRotation(FRotator(AxisValue*mouse_sensitivity,0.0f,0.0f));
					}
					else
					{
						SpringArmComponentPtr->SetRelativeRotation(FRotator(camera_state.max_thetas[camera_state.current_pos],current_camera_pos.Yaw,current_camera_pos.Roll));
					}
				}
			}
		}
		else
		{
			
		}
	}
	
}

void APerson::RMB_pressed()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(M_ACM_RMB, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
	camera_state.free_camera_mod = true;
}
void APerson::RMB_released()
{
#ifdef ENABLE_LOGGING_ON_SCREEN_ACTION_MAPPINGS
	screen_log.PrintMessage(FString("stop ")+M_ACM_RMB, M_DURATION_OF_SCREEN_LOG_MESSAGES_FOR_ACM,FColor::Green);
#endif
	camera_state.free_camera_mod = false;
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


