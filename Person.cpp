
#include "Person.h"

APerson::APerson()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponentPtr = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComp"); //analog of the new operator
	SkeletalMeshComponentPtr->SetupAttachment(GetRootComponent());
	LoadAndSetSkeletalMesh();
	
}
// Ctor functions
void APerson::LoadAndSetSkeletalMesh()
{
	const char* bytes= PATH_SKELETAL_MESH_ARMS;
	Loader<USkeletalMesh> Loader(FString(strlen(bytes),bytes));
	SkeletalMeshComponentPtr->SetSkeletalMesh(Loader.GetDataPtr());
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

}

const char* BoolToCStr(bool bool_value)
{
	return (bool_value?"true":"false");
}

