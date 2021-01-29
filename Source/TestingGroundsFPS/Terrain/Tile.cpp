// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "DrawDebugHelpers.h"
#include "Math/Quat.h"
#include "../ActorPool.h"
//#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"
//#include "NavigationSystem.h"
//#include "Engine/World.h"
//#include "AI/NavigationSystemBase.h"
// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(4000, 2000, 0);
	NavigationBoundsOffset = FVector(2000, 0, 0);
}
void ATile::SetPool(UActorPool* PoolToSet) {
	Pool = PoolToSet;
	PositionNavMeshBoundsVolume();
}
void ATile::PositionNavMeshBoundsVolume() {
	NavMeshBoundsVolume = Pool->CheckOut();
	if (NavMeshBoundsVolume == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Not Enough Actor in pool"));
		return;
	}
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	//GetWorld()->GetNavigationSystem()->Build();
	FNavigationSystem::Build(*GetWorld());
}
// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}
void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Pool->Return(NavMeshBoundsVolume);
}
void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale) {
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}
void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius) {
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, 1, 1);
}
template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale , float MaxScale)
{
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++) {
		FSpawnPositon SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		bool Found = FindEmptyLocation(SpawnPosition.SpawnPoint, Radius * SpawnPosition.Scale);
		if (Found) {
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}
bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius) {
	FBox Bounds(MinExtent, MaxExtent);
	const int MAX_ATTEMPS = 100;
	for (size_t i = 0; i < MAX_ATTEMPS; i++) {
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius)) {
			OutLocation = CandidatePoint;
			return true;
		}
	}
	return false;
}
void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPositon SpawnPosition) {
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if (Spawned) {
		Spawned->SetActorRelativeLocation(SpawnPosition.SpawnPoint);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	}

}
void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPositon SpawnPosition) {
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
	if (Spawned) {
		Spawned->SetActorRelativeLocation(SpawnPosition.SpawnPoint);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->SpawnDefaultController();
		Spawned->Tags.Add(FName("Enemy"));
	}

}
// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
bool ATile::CanSpawnAtLocation(FVector Location, float Radius) {
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, GlobalLocation, GlobalLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius));
	//FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	//DrawDebugSphere(GetWorld(), Location, Radius, 4, ResultColor, true, 100);
	//DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);
	return !HasHit;
}
