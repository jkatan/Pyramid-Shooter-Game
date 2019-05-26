#include "PyramidShooterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "PyramidCube.h"
#include "PyramidShooterCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "PyramidShooterCharacter.h"
#include "Net/UnrealNetwork.h"

APyramidShooterProjectile::APyramidShooterProjectile() 
{
	CreateSphereCollider();

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	SetProjectileMovementComponent();

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	//Replication
	SetReplicates(true);
	SetReplicateMovement(true);
}

void APyramidShooterProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Role == ROLE_Authority)
	{
		ProcessHitOnAuthoritativeRole(OtherActor);
	}
}

void APyramidShooterProjectile::CreateSphereCollider()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &APyramidShooterProjectile::OnHit);		// set up a notification for when this component hits something blocking
}

void APyramidShooterProjectile::SetProjectileMovementComponent()
{
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
}

void APyramidShooterProjectile::ProcessHitOnAuthoritativeRole(AActor* ActorWhoWasHit)
{
	APyramidCube* PyramidCube = Cast<APyramidCube>(ActorWhoWasHit);
	APyramidShooterCharacter* PlayerWhoInflictedTheHit = Cast<APyramidShooterCharacter>(Instigator);

	if (PyramidCube != nullptr && PlayerWhoInflictedTheHit != nullptr)
	{
		PyramidCube->DestroyOnCascade(PlayerWhoInflictedTheHit);
	}

	if ((ActorWhoWasHit != nullptr) && (ActorWhoWasHit != this))
	{
		Destroy();
	}
}