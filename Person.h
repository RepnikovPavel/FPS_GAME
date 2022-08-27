#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <string>
#include "LoggingSystem.h"
#include "Person.generated.h"

#define CHECK_SUCCESS_OF_LOAD_DATA

const char* BoolToCStr(bool bool_value);

template<typename TypeToLoad>
class Loader
{
public:
	Loader(std::string FileName)
	{
		ConstructorHelpers::FObjectFinder<TypeToLoad> Finder(FileName);
		if (Finder.Succeeded())
		{
			_DataPtr = Finder.Object;		
		}
#ifdef CHECK_SUCCESS_OF_LOAD_DATA
		LoggingSystem* LogPtr = LoggingSystem::Instance(std::string(LOG_FILE_PATH));
		LogPtr->WriteToLog(std::string("Loading ")+FileName+std::string(" ")+std::string(BoolToCStr(Finder.Succeeded()))+std::string("\n"));
#endif
	}
private:
	TypeToLoad* _DataPtr;
};

UCLASS()
class FPS_API APerson : public ACharacter
{
	GENERATED_BODY()
private:

	
public:
	APerson();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

