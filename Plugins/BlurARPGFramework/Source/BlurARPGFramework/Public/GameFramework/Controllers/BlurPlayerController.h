// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"

#include "BlurPlayerController.generated.h"

//玩家控制器基类。
UCLASS()
class BLURARPGFRAMEWORK_API ABlurPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ABlurPlayerController();

	//~ Begin IGenericTeamAgentInterface Interface.
	virtual FGenericTeamId GetGenericTeamId() const override;
	//~ End IGenericTeamAgentInterface Interface.

private:
	
	// 玩家队伍Id。用于确认队伍来决定一些功能逻辑的执行策略。
	// Tips：查询 ETeamAttitudeFlags 枚举，来确认相关的功能位置。
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	FGenericTeamId TeamId = FGenericTeamId(0);
};
