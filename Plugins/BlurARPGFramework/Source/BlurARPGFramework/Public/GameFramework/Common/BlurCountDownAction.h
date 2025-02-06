// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Types/BlurEnumTypes.h"

class FBlurCountDownAction : public FPendingLatentAction
{
public:

	// Notes：Member Initializer List 成员初始化列表。
	// 成员初始化列表是在对象的内存初始化阶段直接初始化成员变量。构造函数内部赋值是在成员变量已经默认初始化后，再进行赋值操作。
	// 使用 成员初始化列表 更高效，因为直接初始化避免了多余的默认构造和赋值操作。
	// 常量（const）成员变量、引用（&）类型的成员变量、没有默认构造函数的类类型成员必须使用初始化列表。

	
	FBlurCountDownAction(
		const UObject* WorldContextObject, const float InTotalCountTime, const float InUpdateInterval, const bool InExecuteOnFirst, const bool InPausedWithGame,
		float& InOutRemainingTime, float& InOutDeltaTime, ELatentActionOutput& InCountDownOutput, const FLatentActionInfo& LatentInfo)
	: WorldContextObject(WorldContextObject), bNeedToCancel(false),
	TotalCountDownTime(InTotalCountTime), UpdateInterval(InUpdateInterval), ExecuteOnFirst(InExecuteOnFirst), PausedWithGame(InPausedWithGame),
	OutRemainingTime(InOutRemainingTime), OutDeltaTime(InOutDeltaTime), CountDownOutput(InCountDownOutput),
	ExecutionFunction(LatentInfo.ExecutionFunction), OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget),
	TimerFromStart(0.f), IntervalTimer(0.f)
	{
		
	}

	// 更新。
	virtual void UpdateOperation(FLatentResponse& Response) override;

	// 取消异步事件。
	void CancelAction();
	
private:
	const UObject* WorldContextObject;
	bool bNeedToCancel; // 确认是否触发了取消。
	const float TotalCountDownTime; // 总倒计时时间。
	const float UpdateInterval; // 更新间隔，为零时每帧执行Update回调。
	bool ExecuteOnFirst; // 在第一帧执行。
	const bool PausedWithGame; // 随游戏停止。
	float& OutRemainingTime; // 剩余时间。
	float& OutDeltaTime;
	ELatentActionOutput& CountDownOutput; // 输出执行引脚。
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	float TimerFromStart; // 经过总时间。
	float IntervalTimer;
};