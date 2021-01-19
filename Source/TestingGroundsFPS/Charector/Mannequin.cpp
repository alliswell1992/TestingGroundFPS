// Fill out your copyright notice in the Description page of Project Settings.


#include "Mannequin.h"
#include "../Weapons/MyGun.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); //-39.56f, 1.75f, 64.f positon the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	//create a mesh component that will be used when being viewed from "1st person" view when controling this pawn
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f)); // -0.5f -4.4f, -155.7f
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f)); // 1.9f -19.19f, 5.2f
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	if (GunBlueprint == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Gun Blueprint Missing"))
	}
	Gun = GetWorld()->SpawnActor<AMyGun>(GunBlueprint);
	if (IsPlayerControlled()) {
		Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	else {
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	Gun->AnimInstance = GetMesh()->GetAnimInstance();
	if (InputComponent != NULL) {
		InputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);
	}
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void AMannequin::UnPossessed() {
	Super::UnPossessed();
	if (Gun != nullptr) {
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
}
void AMannequin::PullTrigger() {
	Gun->OnFire();
}