// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BlurAIController.generated.h"

struct FAIStimulus;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;

// AI组件基类。
UCLASS()
class BLURARPGFRAMEWORK_API ABlurAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit ABlurAIController(const FObjectInitializer& ObjectInitializer);

	//~ Begin IGenericTeamAgentInterface Interface.
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~ End IGenericTeamAgentInterface Interface

	// 角色死亡时关闭。防止死亡的角色对绕行避障组件产生不可知的影响。
	UFUNCTION(BlueprintCallable, Category = "Default Crowd Avoidance Config")
	void SetCrowdComponentDeactivate() const;
protected:
	virtual void BeginPlay() override;

	// AI感知组件。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* AIPerceptionComponent;

	// AI感知配置。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	// 感知更新时回调。
	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	// AI队伍ID。
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	FGenericTeamId TeamId = FGenericTeamId(1);
	
	// 开关 绕行人群避让 功能
	UPROPERTY(EditDefaultsOnly, Category = "Default Crowd Avoidance Config")
	bool bEnableDetourCrowdAvoidance = true;

	// Notes：meta = (EditCondition = "boolValue") 编辑条件
	// 当指定的 bool 值为True时才会在Edit面板上允许调整。
	// Notes：UIMin = "1", UIMax = "4" 调整滑块
	// 在Edit面板显示滑块UI用于在限定范围内调整参数。

	// 算法质量。越高效果更好但计算消耗更大。
	UPROPERTY(EditDefaultsOnly, Category = "Default Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = "1", UIMax = "4"))
	int32 DetourCrowdAvoidanceQuality = 4;

	// 碰撞查询范围。
	UPROPERTY(EditDefaultsOnly, Category = "Default Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.f;
};
