#include "DebuggingModule.h"
#include "DebuggerCategory.h"

#include "GameplayDebugger.h"

// �������� �� �������ִ� ���ø��̶�� �����ϸ� �ȴ�.
#define LOCTEXT_NAMESPACE "FDebuggingModule"

// ����� ������
IMPLEMENT_MODULE(FDebuggingModule, Debugging)

void FDebuggingModule::StartupModule()
{
	IGameplayDebugger::FOnGetCategory category;
	category.BindStatic(&FDebuggerCategory::MakeInstance);

	IGameplayDebugger::Get().RegisterCategory("Debugger", category, EGameplayDebuggerCategoryState::EnabledInGameAndSimulate, 5);
	IGameplayDebugger::Get().NotifyCategoriesChanged();
}

void FDebuggingModule::ShutdownModule()
{
	if (IGameplayDebugger::IsAvailable())
		IGameplayDebugger::Get().UnregisterCategory("Debugger");
}

#undef LOCTEXT_NAMESPACE
	
