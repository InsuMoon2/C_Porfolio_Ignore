#include "Character/Enemies/CEnemy_Boss.h"
#include "Global.h"
#include "Character/CAnimInstance.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CInGameUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CStatusComponent.h"
#include "Weapon/CEquipment.h"

ACEnemy_Boss::ACEnemy_Boss()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.3));

	//USkeletalMesh* mesh;
	//CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Meshs3/Grux_Body.Grux_Body'");
	//GetMesh()->SetSkeletalMesh(mesh);
	//GetMesh()->SetSkeletalMeshWithoutResettingAnimation(mesh);
	//
	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/_Game/ABP_Grux.ABP_Grux_C'");
	GetMesh()->SetAnimClass(animInstance);
}

void ACEnemy_Boss::BeginPlay()
{
	Super::BeginPlay();

	Weapon->SetGruxWeaponMode();
	Enemy_Status->InitBossHUD();
}

void ACEnemy_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Enemy_Status->UpdateBossHUD();
	//CLog::Print(State->IsAirActionMode());
}

void ACEnemy_Boss::Hitted()
{
	//Super::Hitted();

	// Apply Damage
	{
		Status->Damage(Damage.Power);
		// Power ����ֱ�
		Damage.Power = 0;
	}

	// Montage, HitStop, SoundWave
	{
		if (!!Damage.Event && !!Damage.Event->HitData)
		{
			FHitData* data = Damage.Event->HitData;

			data->PlayHitStop(GetWorld());
			data->PlayCameraShake(GetWorld());
			data->PlaySoundWave(this);
			data->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

			// Incapacitation
			if (Status->IsIncapacitation())
			{
				if (State->IsExecution())
					return;

				State->SetIncapacitationMode();

				return;
			}

			// ��Ÿ�ְ� ����
			if (!data->IsValidMontage())
			{
				// ���� ���� �ƴ϶��
				if (!State->IsActionMode())
				{
					// Dead
					if (Status->IsDead())
					{
						State->SetDeadMode();

						return;
					}

					State->SetIdleMode();

					Damage.Character = nullptr;
					Damage.Casuer = nullptr;
					Damage.Event = nullptr;

					return;
				}
			}

			// ��ų ���� ���� ������ ��Ÿ�� �÷��� X
			if (!State->IsSkillMode() && data->IsValidMontage())
				data->PlayMontage(this);

			FVector start = GetActorLocation();
			FVector target = Damage.Character->GetActorLocation();

			//���⺤��
			FVector direction = target - start;
			direction.Normalize();

			FVector launch_XY = (-direction * data->Launch.X) + (-direction * data->Launch.Y);
			FVector launch_Z = GetActorUpVector() * data->Launch.Z;

			// ���� �ʾ��� ���� Launch�� �ǰ�
			if (Status->IsDead() == false && !Status->IsIncapacitation())
			{
				LaunchCharacter(launch_XY + launch_Z, false, true);

				// TODO :: Launch �� ���
				//CLog::Print(FVector(launch_XY + launch_Z));
				FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(start, target);

				// Yaw �����θ� ȸ���ϰ�
				SetActorRotation(FRotator(0.0f, targetRotation.Yaw, 0.0f));
			}
		}
	}

	// Dead
	if (Status->IsDead())
	{
		State->SetDeadMode();

		return;
	}

	Damage.Character = nullptr;
	Damage.Casuer = nullptr;
	Damage.Event = nullptr;
}

void ACEnemy_Boss::End_Hitted()
{
	Super::End_Hitted();

}

void ACEnemy_Boss::End_Incapacitation()
{
	Super::End_Incapacitation();

}
