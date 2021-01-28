// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"
USTRUCT()
struct FSpawnPositon {
	GENERATED_USTRUCT_BODY()

	FVector SpawnPoint;
	float Rotation;
	float Scale;
};
class UActorPool;
UCLASS()
class TESTINGGROUNDSFPS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500.f, float MinScale = 1, float MaxScale = 1);
	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* PoolToSet);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtent;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent;
	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	void PositionNavMeshBoundsVolume();
	bool CanSpawnAtLocation(FVector Location, float Radius);
	TArray<FSpawnPositon> RandomSpawnPositions(int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale);
	bool FindEmptyLocation(FVector& OutLocation, float Radius);
	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPositon& SpawnPosition);
	UActorPool* Pool;
	AActor* NavMeshBoundsVolume;
};
