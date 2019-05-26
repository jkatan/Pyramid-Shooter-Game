#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "PyramidShooterCharacter.h"
#include "PyramidCube.generated.h"

UCLASS()
class PYRAMIDSHOOTER_API APyramidCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APyramidCube();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PyramidCubeMesh;

	UPROPERTY(EditAnywhere)
	TArray<UMaterial*> RandomColorsToAssign;

	void DestroyOnCascade(APyramidShooterCharacter* PlayerWhoShotTheCube);

private:
	// Index to select a color from RandomColorsToAssign array
	UPROPERTY(Replicated)
	int ColorMaterialIndex;

	//Trigger to detect adyacent cubes
	UPROPERTY()
	UBoxComponent* PyramidCubeCollider;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateCubeCollider();

	void CreateMaterialsForRandomColors();

	void DestroyOnFibonacciCascade(int PreviousFibonacciTerm, int CurrentFibonacciTerm, APyramidShooterCharacter* PlayerWhoShotTheCube);

	void ExecuteFibonacciCascadeDestruction(int PreviousFibonacciTerm, int CurrentFibonacciTerm,
		APyramidShooterCharacter* PlayerWhoShotTheCube, TArray<AActor*> &AdyacentCubes);

	void NotifyGameStateOfCubeDestruction() const;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
