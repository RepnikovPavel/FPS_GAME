#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <string>
#include "LoggingSystem.h"
#include "Person.generated.h"

#define CHECK_SUCCESS_OF_LOAD_DATA

#define PATH_SKELETAL_MESH_ARMS "/Game/FPS_Assets/Characters/Arms/Mesh/Mesh_Arms"

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

UCLASS()
class FPS_API APerson : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponentPtr;
public:
	APerson();
private:
	void LoadAndSetSkeletalMesh();
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

