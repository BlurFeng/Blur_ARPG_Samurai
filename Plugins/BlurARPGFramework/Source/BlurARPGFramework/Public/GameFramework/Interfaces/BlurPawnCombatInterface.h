// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BlurPawnCombatInterface.generated.h"

class UBlurCombatComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBlurPawnCombatInterface : public UInterface
{
	GENERATED_BODY()
};

// 战斗Pawn接口。
class BLURARPGFRAMEWORK_API IBlurPawnCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//Notes：纯虚函数
	//在函数后添加 = 0 使此函数成为纯虚函数。
	//纯虚函数必须被继承者实现，因为纯虚函数没有默认实现。所以在接口中，我们经常使用纯虚函数。
	//包含纯虚函数的类是 抽象类（abstract class），不能直接被实例化。
	
	virtual UBlurCombatComponent* GetPawnCombatComponent() const = 0;
};
