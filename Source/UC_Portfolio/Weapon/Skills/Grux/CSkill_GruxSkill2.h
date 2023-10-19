#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSkill.h"
#include "CSkill_GruxSkill2.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSkill_GruxSkill2 : public UCSkill
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Skill_Action")
		FDoActionData SkillData;

	UPROPERTY(EditAnywhere, Category = "Skill_Action")
		TArray<FHitData> HitDatas;

public:
	void Skill_Pressed(ESkillType InSkillIndex) override;
	void Skill_Released(ESkillType InSkillIndex) override;

	void Tick_Implementation(float InDeltaTime) override;

	void Begin_Skill_Implementation() override;
	void End_Skill_Implementation() override;

private:
	TArray<class ACharacter*> Hitted;
	int32 HitIndex;

private:
	UFUNCTION()
		void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);

	UFUNCTION()
		void OnAttachmentEndCollision();
	
};
