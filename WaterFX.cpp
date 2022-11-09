// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterFX.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"


// Sets default values
AWaterFX::AWaterFX()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled=true;
	MeshPtr = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("MeshPtr"));
	SetRootComponent(MeshPtr);
	MeshPtr->SetMobility(EComponentMobility::Static);
	MeshPtr->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	MeshPtr->SetGenerateOverlapEvents(false);
	
	// auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/MyContent/water_fx/Pyramid"));
	// SMeshPtr = MeshAsset.Object;
	// // MeshPtr->SetStaticMesh(MeshAsset.Object);
	// if (MeshAsset.Object != nullptr)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Mesh  found!"));
	// }
	//
	// // MeshPtr = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HierarchicalInstancedStaticMesh"));
	// // MeshPtr->SetupAttachment(GetRootComponent());
	// // MeshPtr->SetCullDistances(StartCullDistance,EndCullDistance);
	// // MeshPtr->SetStaticMesh(SMeshPtr);
	//
	// MeshPtr = NewObject<UHierarchicalInstancedStaticMeshComponent>(this,TEXT("name"));
	// MeshPtr->SetupAttachment(GetRootComponent());
	// MeshPtr->SetCullDistances(StartCullDistance,EndCullDistance);
	// MeshPtr->SetStaticMesh(SMeshPtr);
	// for(int k=0;k<Nz;k++)
	// {
	// 	for(int i=0;i<Nx;i++)
	// 	{
	// 		for(int j=0;j<Ny;j++)
	// 		{
	// 			FTransform pos_of_particle(UE::Math::TRotator<double>(0.0,0.0,0.0),
	// 			UE::Math::TVector<double>(ax+i*hx,ay+j*hy,az+k*hz),
	// 			UE::Math::TVector<double>(100.0,100.0,100.0));
	// 			int index_of_particle = MeshPtr->AddInstance(pos_of_particle,false);
	// 			
	// 		}
	// 	}
	// }
	
}

// Called when the game starts or when spawned
void AWaterFX::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWaterFX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaterFX::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Lx = bx-ax;
	Ly=by-ay;
	Lz=bz-az;
	V = Lx*Ly*Lz;
	N = Nx*Ny*Nz;
	hx = Lx/((double)Nx);
	hy = Lx/((double)Ny);
	hz = Lx/((double)Nz);
	V_kij=V/((double)N);
	m_kij = ro*V_kij;

	ReserveMemoryForI();
	
	for(int k=0;k<Nz;k++)
	{
		for(int i=0;i<Nx;i++)
		{
			for(int j=0;j<Ny;j++)
			{
				I[k][i][j] = MeshPtr->AddInstance(FTransform(FVector(ax+i*hx,ay+j*hy,az+k*hz)));
			}
		}
	}
	
}

void AWaterFX::ReserveMemoryForI()
{
	I.resize(Nz);
	for (int k=0;k<Nz;k++)
	{
		I[k].resize(Nx);
		for (int i=0;i<Nx;i++)
		{
			I[k][i].resize(Ny);
		}
	}
}

