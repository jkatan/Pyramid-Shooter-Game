#include "PyramidShooterCharacter.h"
#include "PyramidShooterProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "PyramidShooterGameMode.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"
#include "PyramidShooterPlayerState.h"

APyramidShooterCharacter::APyramidShooterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	CreateCameraComponent();
	CreateFirstPersonMeshComponent();
	CreateGunMeshComponent();
	DefineGunMuzzleLocation();

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);
}

void APyramidShooterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	
	Mesh1P->SetHiddenInGame(false, true);
}

void APyramidShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsLocallyControlled())
	{
		FRotator NewRotation = FirstPersonCameraComponent->RelativeRotation;
		NewRotation.Pitch = RemoteViewPitch * 360.0f / 255.0f;

		FirstPersonCameraComponent->SetRelativeRotation(NewRotation);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void APyramidShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APyramidShooterCharacter::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APyramidShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APyramidShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void APyramidShooterCharacter::IncrementPlayerScore(int ScoreToIncrement)
{
	if (ScoreToIncrement > 0)
	{
		PlayerState->Score += ScoreToIncrement;

		APyramidShooterPlayerState* PyramidShooterPlayerState = Cast<APyramidShooterPlayerState>(PlayerState);

		if (PyramidShooterPlayerState != nullptr)
		{
			PyramidShooterPlayerState->OnPlayerScoreModified().Broadcast(PlayerState->Score);
		}
	}
}

void APyramidShooterCharacter::OnFire()
{
	if (Role < ROLE_Authority)
	{
		PlayCosmeticEffects();
		ServerOnFire();
		return;
	}

	ENetMode NetMode = GetNetMode();

	if (NetMode == NM_Standalone || NetMode == NM_ListenServer)
	{
		PlayCosmeticEffects();
	}

	if (ProjectileClass != nullptr)
	{
		FireProjectile();
	}
}

void APyramidShooterCharacter::ServerOnFire_Implementation()
{
	OnFire();
}

bool APyramidShooterCharacter::ServerOnFire_Validate()
{
	return true;
}

void APyramidShooterCharacter::FireProjectile()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.Instigator = this;

		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

		//Set Spawn Collision Handling Override
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		APyramidShooterProjectile* Projectile = World->SpawnActor<APyramidShooterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	}
}

void APyramidShooterCharacter::PlayCosmeticEffects()
{
	PlayFireSound();
	PlayFireAnimation();
}

void APyramidShooterCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APyramidShooterCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APyramidShooterCharacter::CreateCameraComponent()
{
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
}

void APyramidShooterCharacter::CreateFirstPersonMeshComponent()
{
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}

void APyramidShooterCharacter::CreateGunMeshComponent()
{
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);
}

void APyramidShooterCharacter::DefineGunMuzzleLocation()
{
	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
}

void APyramidShooterCharacter::PlayFireSound()
{
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

void APyramidShooterCharacter::PlayFireAnimation()
{
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

