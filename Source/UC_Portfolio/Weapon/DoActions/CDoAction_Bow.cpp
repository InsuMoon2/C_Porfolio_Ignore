#include "Weapon/DoActions/CDoAction_Bow.h"
#include "Global.h"
#include "Components/CMontagesComponent.h"
#include "Weapon/CEquipment.h"
#include "Weapon/Attachments/CAttachment_Bow.h"
#include "Weapon/AddOns/CArrow.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CWeaponComponent.h"

UCDoAction_Bow::UCDoAction_Bow()
{

}

void UCDoAction_Bow::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner,
                               const TArray<FDoActionData>& InDoActionData, const TArray<FHitData>& InHitData,
                               const TArray<FHitData>& InGuardHitData, const TArray<FHitData>& InBossHitDatas, const TArray<FHitData>& InBossAirHitDatas)
{
	Super::BeginPlay(InAttachment, InEquipment, InOwner, InDoActionData, InHitData, InGuardHitData, InBossHitDatas, InBossAirHitDatas);

	SkeletalMesh = CHelpers::GetComponent<USkeletalMeshComponent>(InAttachment);
	PoseableMesh = CHelpers::GetComponent<UPoseableMeshComponent>(InAttachment);
	 
	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);

	if(!!bow)
		Bending = bow->GetBend();

	OriginLocation = PoseableMesh->GetBoneLocationByName("bow_string_mid", EBoneSpaces::ComponentSpace);
	bEquipped = InEquipment->GetEquipped();
}

void UCDoAction_Bow::DoAction()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(State->IsSubActionMode());

	Super::DoAction();

	State->SetActionMode();

	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Bow::Begin_DoAction()
{
	Super::Begin_DoAction();

	bAttachedString = false;

	*Bending = 0.0f;
	PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);

	CheckNull(ArrowClass);

	TWeakObjectPtr<ACArrow> arrow = GetAttachedArrow();

	CheckNull(arrow);
	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	if (!arrow->OnHit.IsBound())
		arrow->OnHit.AddDynamic(this, &UCDoAction_Bow::OnArrowHit);

	if (!arrow->OnEndPlay.IsBound())
		arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::OnArrowEndPlay);

	FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();

	arrow->Shoot(forward, OwnerCharacter);
}

void UCDoAction_Bow::End_DoAction()
{
	Super::End_DoAction();

	// ��Ƽ���̿��� ����
	//CreateArrow();
}

void UCDoAction_Bow::OnBeginEquip()
{
	Super::OnBeginEquip();

	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ��Ƽ���̿��� ����
	//CreateArrow();
}

void UCDoAction_Bow::OnUnequip()
{
	Super::OnUnequip();

	*Bending = 0.0f;
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);

	for (int32 i = Arrows.Num() - 1; i >= 0; i--)
	{
		if (!!Arrows[i]->GetAttachParentActor())
			Arrows[i]->Destroy();
	}
}


void UCDoAction_Bow::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

	bool bCheck = true;
	bCheck &= (*bEquipped == true);
	bCheck &= (bBeginAction == false);
	bCheck &= (bAttachedString == true);

	CheckFalse(bCheck);

	FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right");
	PoseableMesh->SetBoneLocationByName("bow_string_mid", handLocation, EBoneSpaces::WorldSpace);
}

void UCDoAction_Bow::End_BowString()
{
	*Bending = 100.0f;
	bAttachedString = true;
}

void UCDoAction_Bow::CreateArrow()
{
	// IsTearingDown�� ���尡 ����Ǿ��ٴ� �ǹ��̴�.
	// ���尡 ����Ǵ� ��Ȳ���� ������ ����Ǿ��� ���� ���̵��̳ĸ� üũ���ִ� ���̴�.
	if (World->bIsTearingDown == true)
		return;

	FTransform transform;
	ACArrow* arrow = World->SpawnActorDeferred<ACArrow>(ArrowClass, transform, NULL, NULL,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	CheckNull(arrow);

	arrow->AddIgnoreActor(OwnerCharacter);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Right_Arrow");

	Arrows.Add(arrow);
	UGameplayStatics::FinishSpawningActor(arrow, transform);
}

ACArrow* UCDoAction_Bow::GetAttachedArrow()
{
	for (int32 i = Arrows.Num() -1; i >= 0; i--)
	{
		if (!!Arrows[i]->GetAttachParentActor())
		{
			return Arrows[i];
		}
	}

	return nullptr;
}

void UCDoAction_Bow::OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
	// Guard Hitted, arrow Hitted Random
	int32 arrow_hitted = UKismetMathLibrary::RandomInteger64InRange(0, 2);
	int32 guard_hitted = UKismetMathLibrary::RandomIntegerInRange(0, GuardHitDatas.Num() - 1);

	// Bow�� Parrying XX
	{
		int32 RandomInt = UKismetMathLibrary::RandomIntegerInRange(
			0, GuardHitDatas.Num() - 1);

		// �´� ���� State
		UCStateComponent* hitted_state = CHelpers::GetComponent<UCStateComponent>(InOtherCharacter);
		UCWeaponComponent* hitted_weapon = CHelpers::GetComponent<UCWeaponComponent>(InOtherCharacter);
		CheckNull(hitted_state);
		CheckNull(hitted_weapon);

		CheckTrue(CHelpers::IsSameTeam(OwnerCharacter, InOtherCharacter));

		if (hitted_state->IsGuardMode())
		{
			CheckTrue(GuardHitDatas.Num() - 1 < 0);

			bool bGuardCheck = false;
			bGuardCheck = CHelpers::DotForward(InCauser, InOtherCharacter);

			// ���� ����
			if (bGuardCheck)
			{	
				GuardHitDatas[guard_hitted].PlayCameraShake(OwnerCharacter->GetWorld());
				GuardHitDatas[guard_hitted].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);

				hitted_state->SetGuardMode();
			}
			// ���� ����
			else
			{
				CheckTrue(HitDatas.Num() - 1 < 0);
				HitDatas[arrow_hitted].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);
			}
		}
		// �׳� ���� ��
		else
		{
			CheckTrue(HitDatas.Num() - 1 < 0);
			HitDatas[arrow_hitted].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);
		}

	}
}

void UCDoAction_Bow::OnArrowEndPlay(ACArrow* InDestroyer)
{
	Arrows.Remove(InDestroyer);
}

