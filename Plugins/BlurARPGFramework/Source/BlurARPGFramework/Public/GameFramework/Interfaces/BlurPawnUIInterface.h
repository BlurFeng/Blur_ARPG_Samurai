// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BlurPawnUIInterface.generated.h"

class UBlurPawnUIComponent;
class UBlurCharacterUIComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBlurPawnUIInterface : public UInterface
{
	GENERATED_BODY()
};

// PawnUI接口，用于Pawn和UI之间进行通信。
class BLURARPGFRAMEWORK_API IBlurPawnUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//获取PawnUI组件。
	virtual UBlurPawnUIComponent* GetPawnUIComponent() const = 0;

	//此接口不是必须实现的，由继承的类决定是否实现内容。
	//获取角色UI组件
	virtual UBlurCharacterUIComponent* GetCharacterUIComponent() const;
	
};
