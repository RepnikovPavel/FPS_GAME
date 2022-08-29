
#include "Person.h"

#include "NavigationSystemTypes.h"

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
	AssignSpringArmVaribles(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 90.0f, 0.0f),400.0f,3.0f);
	LoadAnimationAsset(PATH_ANIMATION_ASSET_FPS_IDLE);
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
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

	// IDLE functionality
	PlayerInputComponent->BindAction("IDLE_full_stamina",IE_Pressed,this,&APerson::PlayFPSIdle);

}

//animations callback functions:
void APerson::PlayFPSIdle()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,"playing FPS idle");
	}
	GetMyMesh()->PlayAnimation(AnimationAssetPtr,true);
}

const char* BoolToCStr(bool bool_value)
{
	return (bool_value?"true":"false");
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
