#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// ü��
	UPROPERTY(EditAnywhere, Category = "Health Point")
		float MaxHealth = 1000.0f;
	// ����
	UPROPERTY(EditAnywhere, Category = "Mana Point")
		float MaxMana = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Mana Point")
		float AddMana = 0.1f;

	// ����
	UPROPERTY(EditAnywhere, Category = "Incapacitation Point")
		float MaxIncapacitation = 100.0f;

private:
	float Health;
	float Mana;
	float Incapacitation;

public:
	// ü��
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE bool  IsDead() { return Health <= 0.0f; }

	// ����
	FORCEINLINE float GetMana() { return Mana; }
	FORCEINLINE float GetMaxMana() { return MaxMana; }
	FORCEINLINE float IsManaZero() { return Mana <= 0.0f; }
	FORCEINLINE bool  CompareInMana(float InMana) { return Mana >= InMana; }

	// ����
	FORCEINLINE float GetIncapacitation() { return Incapacitation; }
	FORCEINLINE float GetMaxIncapacitation() { return MaxIncapacitation; }
	FORCEINLINE bool  IsIncapacitation() { return Incapacitation <= 0.0f; }

	// ���� �ʱ�ȭ
	FORCEINLINE float SetMaxIncapacitation() { return Incapacitation = MaxIncapacitation; }

public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Damage(float InAmount);
	void UseMana(float InMana);

	void ChargeMana();

private:
	class ACharacter* OwnerCharacter;
};
