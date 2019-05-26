#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PyramidShooterPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerScoreModifiedDelegate, int)
/**
 * 
 */
UCLASS()
class PYRAMIDSHOOTER_API APyramidShooterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void OnRep_Score() override;

	FPlayerScoreModifiedDelegate& OnPlayerScoreModified();

private:
	FPlayerScoreModifiedDelegate PlayerScoreModifiedEvent;
};
