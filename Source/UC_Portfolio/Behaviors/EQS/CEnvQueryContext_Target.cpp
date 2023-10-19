#include "Behaviors/EQS/CEnvQueryContext_Target.h"
#include "Global.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Character/Enemies/CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	// ������̵� �� ���� �� �־�� �ؼ� UObject�� ������ �ؾ��Ѵ�.
	// Querier�� Character�� �� ���� �ְ�, �ٸ� �͵� �� ���� �ֱ� ������ �ֻ��������� ������ �ϴ� ���̴�.
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());
	ACAIController* controller = ai->GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	AActor* target = Cast<AActor>(blackboard->GetValueAsObject("Target"));

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, target);

}
