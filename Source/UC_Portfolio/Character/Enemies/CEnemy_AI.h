#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()

private:
	// ĳ���ͺ��� BehaviorTree�� ���� �ֱ� ���ؼ�
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Text")
		class UTextRenderComponent* AIState;

	UPROPERTY(EditDefaultsOnly, Category = "Text")
		class UTextRenderComponent* TeamID_Text;

protected:
	//// 0 ~ 255������ ��� ����, 255���� �߸��̴�.
	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	//	uint8 TeamID = 2;

	UPROPERTY(EditAnywhere, Category = "Patrol")
		class ACPatrolPath* PatrolPath;

	UPROPERTY(VisibleDefaultsOnly)
		class UCAIBehaviorComponent* Behavior;

	UPROPERTY(VisibleDefaultsOnly)
		class UCInGameUIComponent* Enemy_Status;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* LabelWidget;

public:
	//UFUNCTION(BlueprintCallable)
	//	FORCEINLINE uint8 GetTeamID() { return TeamID; }

	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

	FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath; }

public:
	ACEnemy_AI();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	void Hitted() override;

public:
	void End_Hitted() override;
	void End_Incapacitation() override;

};
