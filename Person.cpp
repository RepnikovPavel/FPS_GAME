
#include "Person.h"

APerson::APerson()
{
	PrimaryActorTick.bCanEverTick = true;
}

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
