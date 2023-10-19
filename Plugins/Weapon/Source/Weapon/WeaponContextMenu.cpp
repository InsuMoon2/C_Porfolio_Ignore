#include "WeaponContextMenu.h"
#include "WeaponAssetEditor.h"
#include "Weapon/CWeaponAsset.h"

FWeaponContextMenu::FWeaponContextMenu(EAssetTypeCategories::Type InCategory)
{
	Category = InCategory;
}

FText FWeaponContextMenu::GetName() const
{
	return FText::FromString("DataAsset");
}

UClass* FWeaponContextMenu::GetSupportedClass() const
{
	return UCWeaponAsset::StaticClass();
}

FColor FWeaponContextMenu::GetTypeColor() const
{
	return FColor::Blue;
}

uint32 FWeaponContextMenu::GetCategories()
{
	return Category;
}

void FWeaponContextMenu::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	/*for(UObject* obj : InObjects)
	{
		if (!!obj)
			GLog->Log(obj->GetName());
	}*/

	// �ϳ��� ������ �Ǿ����� �ʴٸ�, ���� ������ ����� ���� ���� ����� ���̴�.
	if (InObjects.Num() < 1)
		return;

	// ����Ŭ���� �ؼ� �� ���̴ϱ� �ϳ��� ������ �Ǿ��ִٰ� ������ ���̴�. 
	FWeaponAssetEditor::OpenWindow(InObjects[0]->GetName());
}
