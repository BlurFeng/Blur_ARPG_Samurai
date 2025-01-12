// Blur Feng All Rights Reserved.


#include "GameFramework/Components/UI/BlurPawnUIComponent.h"

#include "GameFramework/UserWidgets/BlurWidgetBase.h"

void UBlurPawnUIComponent::RegisterDrawnWidget(UBlurWidgetBase* InWidgetToRegister)
{
	if (DrawnWidgets.Contains(InWidgetToRegister)) return;
	
	DrawnWidgets.Add(InWidgetToRegister);
}

void UBlurPawnUIComponent::RemoveDrawnWidgetsAll()
{
	if (DrawnWidgets.IsEmpty()) return;

	for (UBlurWidgetBase* DrawnWidget : DrawnWidgets)
	{
		if (!DrawnWidget) continue;

		DrawnWidget->RemoveFromParent();
	}

	DrawnWidgets.Empty();
}
