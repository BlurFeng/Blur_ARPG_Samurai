// Blur Feng All Rights Reserved.


#include "GameFramework/Components/UI/BlurPawnUIComponent.h"

#include "GameFramework/UserWidgets/BlurUserWidgetBase.h"

void UBlurPawnUIComponent::RegisterDrawnWidget(UBlurUserWidgetBase* InWidgetToRegister)
{
	if (DrawnWidgets.Contains(InWidgetToRegister)) return;
	
	DrawnWidgets.Add(InWidgetToRegister);
}

void UBlurPawnUIComponent::RemoveDrawnWidgetsAll()
{
	if (DrawnWidgets.IsEmpty()) return;

	for (UBlurUserWidgetBase* DrawnWidget : DrawnWidgets)
	{
		if (!DrawnWidget) continue;

		DrawnWidget->RemoveFromParent();
	}

	DrawnWidgets.Empty();
}
