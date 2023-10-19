#include "WeaponAssetEditor.h"
#include "SWeaponDetailsView.h"
#include "SWeaponLeftArea.h"
#include "SWeaponEquipmentData.h"
#include "SWeaponHitData.h"
#include "SWeaponDoActionData.h"
#include "Weapon/CWeaponAsset.h"
#include "SWeaponAirActionData.h"

const FName FWeaponAssetEditor::EditorName = "WeaponAssetEditor";
const FName FWeaponAssetEditor::LeftAreaTabId = "LeftArea";
const FName FWeaponAssetEditor::DetailTabId = "Details";

TSharedPtr<FWeaponAssetEditor> FWeaponAssetEditor::Instance = nullptr;

void FWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	//â�� �ѹ��̶� ������ �ν��Ͻ��� �Ҵ��� �� ���̴�.
	if(Instance.IsValid())
	{
		//â�� �����ִ� ���¶��
		if(Instance->LeftArea.IsValid())
		{
			FWeaponRowDataPtr ptr = nullptr;

			// ������ �������� ����Ŭ���� �ߴٸ� AssetName���� ptr�� ã�ƿ´�.
			if (InAssetName.Len() > 0)
				ptr = Instance->LeftArea->GetRowDataPtrByName(InAssetName);

			// Ptr�� ���ٸ� FirstData ù��° �����͸� ���õǰ� �����ָ� �� ���̴�.
			if (ptr.IsValid() == false)
				ptr = Instance->LeftArea->GetFirstDataPtr();

			// ã�Ҵٸ� LeftArea�� SelectDataPtr���� ptr->Asset�� �ָ� �� ���̴�.
			Instance->LeftArea->SelectDataPtr(ptr->Asset);

			// �׷����ϰ� �� �̻� ������ �ʿ䰡 �����ϱ� return�� ���� ���̴�.
			return;
		}

		Instance->CloseWindow();

		Instance.Reset();
		Instance = nullptr;
	}

	Instance = MakeShareable(new FWeaponAssetEditor());
	Instance->Open(InAssetName);
}

void FWeaponAssetEditor::Shutdown()
{
	if (Instance.IsValid())
	{
		Instance->CloseWindow();

		Instance.Reset();
		Instance = nullptr;
	}
}

void FWeaponAssetEditor::Open(FString InAssetName)
{
	LeftArea = SNew(SWeaponLeftArea)
	.OnSelectedItem(this, &FWeaponAssetEditor::OnListViewSelectedItem);

	FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	//DetailsView
	{
		// �⺻�� ����. ActorsUseNameArea, ObjectsUserNameArea, HideNameArea
		FDetailsViewArgs args(false, false, true, FDetailsViewArgs::HideNameArea);
		/* ������ �䰡 ������� ������ ������ �ĺ��ڸ� �ִ� ���̴�.
		���߿� ���ӿ� �����ϴ� ��찡 ����� �� �̸����� ã�� �ȴ�.*/
		args.ViewIdentifier = "WeaponAssetEditorDetailsView";
		DetailsView = prop.CreateDetailView(args);

		FOnGetDetailCustomizationInstance detailView;
		detailView.BindStatic(&SWeaponDetailsView::MakeInstance);
		DetailsView->SetGenericLayoutDetailsDelegate(detailView);
	}
	
	//EquipmentData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponEquipmentData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("EquipmentData", instance);
	}

	//DoActionData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponDoActionData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("DoActionData", instance);
	}

	//HitData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponHitData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("HitData", instance);
	}

	//AirActionData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponHitData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("AirActionData", instance);
	}


	// ���⼭���� �ؿ��� �����εǴ� �κ��̴ϱ� �� ���� ��ü�� ������ ���־�� �� ���̴�.
	TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("WeaponAssetEditor_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.4f)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.175f) // ������ 17.5%
					->AddTab(LeftAreaTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.725f)
					->AddTab(DetailTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
			)
		);

	UCWeaponAsset* asset = nullptr;
	// �̰� �ƴ϶� ���� �츮�� ������ �������� ������ ���ָ� �ȴ�.
	//asset = LeftArea->GetFirstDataPtr()->Asset;

	if(InAssetName.Len() > 0)
	{
		FWeaponRowDataPtr ptr = LeftArea->GetRowDataPtrByName(InAssetName);

		if (LeftArea->SelectedRowDataPtrName() == InAssetName)
			return;

		if (ptr.IsValid())
			asset = ptr->Asset;
		
	}

	if(asset == nullptr)
		asset = LeftArea->GetFirstDataPtr()->Asset;


	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), 
		EditorName, layout, true, true, asset);

	//DetailsView->SetObject(asset);
	LeftArea->SelectDataPtr(asset);
}

bool FWeaponAssetEditor::OnRequestClose()
{
	if (!!DetailsView)
	{
		if (!!GEditor && !!GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->NotifyAssetClosed(GetEditingObject(), this);

		if(FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			prop.UnregisterCustomPropertyTypeLayout("EquipmentData");
			prop.UnregisterCustomPropertyTypeLayout("DoActionData");
			prop.UnregisterCustomPropertyTypeLayout("HitData");
			prop.UnregisterCustomPropertyTypeLayout("AirActionData");
		}
	}

	if (LeftArea.IsValid())
		LeftArea.Reset();

	if (DetailsView.IsValid())
		DetailsView.Reset();

	return true;
}

void FWeaponAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	FOnSpawnTab tab;
	tab.BindSP(this, &FWeaponAssetEditor::Spawn_LeftAreaTab);
	TabManager->RegisterTabSpawner(LeftAreaTabId, tab);

	FOnSpawnTab tab2;
	tab2.BindSP(this, &FWeaponAssetEditor::Spawn_DetailsViewTab);
	TabManager->RegisterTabSpawner(DetailTabId, tab2);
}

TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_LeftAreaTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
		[
			LeftArea.ToSharedRef()
	];
}

TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_DetailsViewTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
		[
			DetailsView.ToSharedRef()
		];
}

void FWeaponAssetEditor::OnListViewSelectedItem(FWeaponRowDataPtr InDataPtr)
{
	if (InDataPtr == nullptr)
		return;

	if (!!GetEditingObject())
		RemoveEditingObject(GetEditingObject());

	// �ٽ� ���� â�� �������� ��ü�� ������־���.
	AddEditingObject(InDataPtr->Asset);
	// â�ȿ� Detail View�� �ٲ�� ���־���.
	DetailsView->SetObject(InDataPtr->Asset);

}

FName FWeaponAssetEditor::GetToolkitFName() const
{
	return EditorName;
}

FText FWeaponAssetEditor::GetBaseToolkitName() const
{
	return FText::FromName(EditorName);
}

FString FWeaponAssetEditor::GetWorldCentricTabPrefix() const
{
	return EditorName.ToString();
}

FLinearColor FWeaponAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0, 0, 1);
}

FReply FWeaponAssetEditor::OnClicked()
{
	GLog->Log("OnClicked!!");

	return FReply::Handled();
}
