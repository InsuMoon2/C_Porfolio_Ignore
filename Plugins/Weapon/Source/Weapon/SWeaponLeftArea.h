#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STableRow.h"

struct FWeaponRowData
{
	int Number;
	FString Name;
	class UCWeaponAsset* Asset;

	FWeaponRowData()
	{
		
	}

	FWeaponRowData(int32 InNumber, FString InName, class UCWeaponAsset* InAsset)
		: Number(InNumber), Name(InName), Asset(InAsset)
	{
		
	}

	static TSharedPtr<FWeaponRowData> Make(int32 InNumber, FString InName, class UCWeaponAsset* InAsset)
	{
		return MakeShareable(new FWeaponRowData(InNumber, InName, InAsset));
	}
};
typedef TSharedPtr<FWeaponRowData> FWeaponRowDataPtr;

///////////////////////////////////////////////////////////////////////////////

class WEAPON_API SWeaponTableRow
// ������ ����� �ڷ����� FWeaponRowDataPtr�� �־��־���.
	: public SMultiColumnTableRow<FWeaponRowDataPtr>
{
public:
	SLATE_BEGIN_ARGS(SWeaponTableRow) {};
	// �Ϲ� ������ �� ������ SLATE_ARGUMNET ���, �տ� �ڷ���, �ڿ� ������
	SLATE_ARGUMENT(FWeaponRowDataPtr, RowData)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable);

protected:
	TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:
	FWeaponRowDataPtr Data;
};

//////////////////////////////////////////////////////////////////////////////

DECLARE_DELEGATE_OneParam(FOnWeaponListViewSelectedItem, FWeaponRowDataPtr);

class WEAPON_API SWeaponLeftArea
	: public SCompoundWidget
{
public:
	// ����� ���� ��ũ�� ������ ���� ������ ��ũ ������ ����.
	SLATE_BEGIN_ARGS(SWeaponLeftArea) {};
	SLATE_EVENT(FOnWeaponListViewSelectedItem, OnSelectedItem)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

	// �ϳ��� �����Ͱ� �ִ��� üũ
	bool HasRowDataPtr() { return RowDatas.Num() > 0; }
	// ù��° ��� üũ
	FWeaponRowDataPtr GetFirstDataPtr() { return RowDatas[0]; }

	/* ���õ� ������ �� ���̴�. ���ο��� ������ �����ϰ� �����,
	 �¿� ���� ������ ���� �ٸ��� ���̰� ���� ���̴�.*/
	void SelectDataPtr(class UCWeaponAsset* InAsset);

	FWeaponRowDataPtr GetRowDataPtrByName(FString InAssetName);
	FString SelectedRowDataPtrName();

private:
	// �갡 ���� ���ٿ� ���� ����� ������ִ� ���̴�.
	TSharedRef<ITableRow> OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable);
	void OnSelectionChanged(FWeaponRowDataPtr InDataPtr, ESelectInfo::Type InType);

	// ���� �갡 FText�ε�, �׷��� �������� �ݵ�� �װ��̴�.
	FText OnGetAssetCount() const; //��� ������ ���� ���̱� ������ const

	void OnTextChanged(const FText& InText); // �Է��� �ؽ�Ʈ�� ��
	// ����ó�� ���� �Է��� �Ϸ�Ǿ��� �� ȣ��Ǵ� �̺�Ʈ
	void OnTextCommitted(const FText& InText, ETextCommit::Type InType);

private:
	void ReadDataAssetList();

private:
	FOnWeaponListViewSelectedItem OnListViewSelectedItem;

private:
	TArray<FWeaponRowDataPtr> RowDatas;
	TSharedPtr<SListView<FWeaponRowDataPtr>> ListView;

private:
	
	TSharedPtr<class SSearchBox> SearchBox;
	FText SearchText; // �˻� ���ڿ��� ������ �͵��̴�.
};

 