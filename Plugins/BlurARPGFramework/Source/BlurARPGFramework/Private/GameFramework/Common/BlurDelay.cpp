// Blur Feng All Rights Reserved.


#include "GameFramework/Common/BlurDelayAction.h"
#include "Kismet/GameplayStatics.h"

void FBlurDelayAction::UpdateOperation(FLatentResponse& Response)
{
	// FPendingLatentAction::UpdateOperation(Response);

	// 游戏暂停时不更新。
	if (UGameplayStatics::IsGamePaused(WorldContextObject) && PausedWithGame)
		return;

	// 取消。
	if (bNeedToCancel)
	{
		CountDownOutput = ELatentActionOutput::Cancelled;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	// 完成。
	if (IntervalTimer >= Duration)
	{
		CountDownOutput = ELatentActionOutput::Completed;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	// 计时器增加。
	IntervalTimer += Response.ElapsedTime();
}

void FBlurDelayAction::CancelAction()
{
	bNeedToCancel = true;
}