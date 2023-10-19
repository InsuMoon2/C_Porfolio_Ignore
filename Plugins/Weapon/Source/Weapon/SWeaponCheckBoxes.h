#pragma once

#include "CoreMinimal.h"

class WEAPON_API SWeaponCheckBoxes
	: public TSharedFromThis<SWeaponCheckBoxes>
{
public:
	void AddProperties(TSharedPtr<IPropertyHandle> InHandle);

	TSharedRef<SWidget> Draw(bool bBackground = false);
	void DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder* InChildrenBuilder);


	void SetUtilities(TSharedPtr<class IPropertyUtilities> InUtilities);

private:
	void OnCheckStateChanged(ECheckBoxState InState, int32 InIndex);

public:
	static bool CanDraw(TSharedPtr<IPropertyHandle> InHandle, int InCount);

public:
	// Property ��ȣ, �⺻��
	void CheckDefaultObject(int32 InIndex, UObject* InValue);
	void CheckDefaultObject(int32 InIndex, USoundCue* InValue);
	void CheckDefaultObejct(int32 InIndex, TSubclassOf<class UMatineeCameraShake> CameraShakeClass);
	void CheckDefaultValue(int32 InIndex, float InValue);
	void CheckDefaultValue(int32 InIndex, bool InValue);
	void CheckDefaultValue(int32 InIndex, const FVector& InValue);
	void CheckDefaultValue(int32 InIndex, FVector& InValue);

private:
	struct FInternalData
	{
		bool bChecked; // üũ �Ǿ�����
		FString Name; // �̸�
		TSharedPtr<IPropertyHandle> Handle; // ���� �ĺ���

		// �ڵ��� �ϳ� �������μ� �ĺ��� �� ���̴�.
		FInternalData(TSharedPtr<IPropertyHandle> InHandle)
		{
			bChecked = false;
			Handle = InHandle;

			// Handle �ȿ� ������ ��µ� �̸��� DisplayName�� �ִ�.
			Name = Handle->GetPropertyDisplayName().ToString();
		}
	};

	// ���� ����ü�� InternalDatas�� ������־���.
	TArray<FInternalData> InternalDatas;

	TSharedPtr<class IPropertyUtilities> Utilities;
};
