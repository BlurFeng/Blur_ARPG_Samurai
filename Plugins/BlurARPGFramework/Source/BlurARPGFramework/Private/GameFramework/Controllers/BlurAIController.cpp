// Blur Feng All Rights Reserved.


#include "GameFramework/Controllers/BlurAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

// Notes：通过构造函数重载成员变量类型
// 通过这种方式。我们允许重载父类的一些成员变量类型为其子类。
// 而在蓝图中，我们可以直接通过Details面板中操作更改成员变量的类型。

ABlurAIController::ABlurAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	// 创建AI感知配置。
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseConfig_Sight");
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true; // 检测敌人。
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false; // 检测队友。
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false; // 检测中立。
	AISenseConfig_Sight->SightRadius = 5000.f; // 视野半径。
	AISenseConfig_Sight->LoseSightRadius = 0.f; // 丢失视野半径。
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f; // 周边视觉角度。

	// 创建AI感知组件。
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight); // 使用配置。
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass()); // 设置感知类型。
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnPerceptionUpdated);

	// 设置队伍ID。
	AAIController::SetGenericTeamId(TeamId);
}

ETeamAttitude::Type ABlurAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);

	// 获取目标队伍ID。
	if (const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(PawnToCheck->GetController()))
	{
		// 确认是不同队伍的目标。
		if (OtherTeamAgent->GetGenericTeamId() == GetGenericTeamId())
			return ETeamAttitude::Friendly; // 队友。
		else
			return ETeamAttitude::Hostile; // 敌对。
	}

	// 中立。
	return ETeamAttitude::Neutral;
}

void ABlurAIController::SetCrowdComponentDeactivate() const
{
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		// 关闭人群避障组件并清空数据。
		CrowdComp->Deactivate();
		CrowdComp->Cleanup();
	}
}

void ABlurAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		// 开关人群避让状态。
		CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		// 根据配置设置人群避障算法质量。
		switch (DetourCrowdAvoidanceQuality)
		{
		case 1: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low); break;
		case 2: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good); break;
		case 4: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High); break;
		default:
			break;
		}

		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange); //设置碰撞查询范围。
	}
}

void ABlurAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor")))
		{
			if (Stimulus.WasSuccessfullySensed() && Actor)
			{
				// 设置目标到自身黑板的 TargetActor。
				BlackboardComponent->SetValueAsObject("TargetActor", Actor);
			}
		}
	}
}
