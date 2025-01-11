// Blur Feng All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EBlurComparisonOp : uint8
{
	//相等。
	Equals,
	
	//不等。
	NotEquals,

	//大于。
	GreaterThan,

	//大于等于。
	GreaterThanEquals,

	//小于。
	LessThan,

	//小于等于。
	LessThanEquals,
};

UENUM()
enum class EBlurConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EBlurValidType : uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EBlurSuccessType : uint8
{
	Successful,
	Failed
};

UENUM()
enum class EBlurCountDownActionInput : uint8
{
	//开始。
	Start,
	//取消。
	Cancel
};

UENUM()
enum class EBlurCountDownActionOutput : uint8
{
	//更新。
	Update,
	//完成。
	Completed,
	//取消。
	Cancelled
};

//游戏难度。
UENUM(BlueprintType)
enum class EBlurGameDifficulty : uint8
{
	Easy = 0,
	Normal = 1,
	Hard = 2,
	VeryHard =3
};

//输入模式。
UENUM()
enum class EBlurInputMode : uint8
{
	GameOnly,
	UIOnly
};

//输入类型。
UENUM()
enum class EBlurInputType : uint8
{
	//普通形式，按下时触发。
	Normal,

	//切换形式。在激活和取消之间切换。比如可主动切换的愤怒状态。
	Toggleable,

	//持续按住形式。松开时取消技能。比如防御。
	MustBeHeld,
};

// 队伍态度Flags。可用于确认某个功能对于不同队伍的处理方式进行区别。
UENUM(BlueprintType, meta = (Bitflags))
enum class ETeamAttitudeFlags : uint8
{
	None      UMETA(DisplayName = "None"),
	Friendly  UMETA(DisplayName = "Friendly"),
	Neutral   UMETA(DisplayName = "Neutral"),
	Hostile   UMETA(DisplayName = "Hostile")
};