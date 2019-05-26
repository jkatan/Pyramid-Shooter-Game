#include "PyramidShooterPlayerState.h"

void APyramidShooterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();
	
	OnPlayerScoreModified().Broadcast(Score);
}

FPlayerScoreModifiedDelegate& APyramidShooterPlayerState::OnPlayerScoreModified()
{
	return PlayerScoreModifiedEvent;
}
