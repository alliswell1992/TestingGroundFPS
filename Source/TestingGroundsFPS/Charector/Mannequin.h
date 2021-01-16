// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Mannequin.generated.h"

UCLASS()
class TESTINGGROUNDSFPS_API AMannequin : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMannequin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PullTrigger();
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class AMyGun> GunBlueprint;
private:
	// Spawn Mesh First person view (Arms, seen only by self)
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* Mesh1P;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class AMyGun* Gun;
};
