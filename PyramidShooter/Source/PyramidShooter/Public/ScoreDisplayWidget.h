#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreDisplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDSHOOTER_API UScoreDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UScoreDisplayWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PlayerScoreContent;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UVerticalBox* ScoreboardContent;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ScoreBoardTitle;

	void UpdateScore(int InScore);

	void DisplayPlayersScoreboard();

protected:
	virtual void NativeConstruct() override;

private:
	TSubclassOf<class UUserWidget> ScoreboardRowWidgetClass;
};
