#include "UI/Widget/Base/MCUIRootWidget.h"

#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "Widgets/SOverlay.h"

bool UMCUIRootWidget::AddWidget(UUserWidget* Widget, int32 ZOrder)
{
	if (!IsValid(Widget)
		|| Widget == this
		|| IsValid(Widget->GetParent())
		|| Widget->IsInViewport()
		|| ManagedWidgets.Contains(Widget))
	{
		return false;
	}

	ManagedWidgets.Add(Widget);
	WidgetZOrders.Add(Widget, ZOrder);

	if (RootOverlay.IsValid())
	{
		AddWidgetToSlate(Widget, ZOrder);
	}

	return true;
}

bool UMCUIRootWidget::RemoveWidget(UUserWidget* Widget)
{
	if (!IsValid(Widget) || !ManagedWidgets.Contains(Widget))
	{
		return false;
	}

	if (RootOverlay.IsValid())
	{
		if (const TSharedPtr<SWidget>* SlateWidget = WidgetSlateContents.Find(Widget))
		{
			if (SlateWidget->IsValid())
			{
				RootOverlay->RemoveSlot(SlateWidget->ToSharedRef());
			}
		}
	}

	WidgetSlateContents.Remove(Widget);
	WidgetZOrders.Remove(Widget);
	ManagedWidgets.Remove(Widget);
	return true;
}

void UMCUIRootWidget::ClearWidgets()
{
	if (RootOverlay.IsValid())
	{
		RootOverlay->ClearChildren();
	}

	WidgetSlateContents.Reset();
	WidgetZOrders.Reset();
	ManagedWidgets.Reset();
}

int32 UMCUIRootWidget::GetWidgetCount() const
{
	return ManagedWidgets.Num();
}

TSharedRef<SWidget> UMCUIRootWidget::RebuildWidget()
{
	RootOverlay = SNew(SOverlay);
	WidgetSlateContents.Reset();

	for (UUserWidget* Widget : ManagedWidgets)
	{
		if (!IsValid(Widget))
		{
			continue;
		}

		const int32 ZOrder = WidgetZOrders.FindRef(Widget);
		AddWidgetToSlate(Widget, ZOrder);
	}

	return RootOverlay.ToSharedRef();
}

void UMCUIRootWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	WidgetSlateContents.Reset();
	RootOverlay.Reset();
}

void UMCUIRootWidget::AddWidgetToSlate(UUserWidget* Widget, int32 ZOrder)
{
	if (!RootOverlay.IsValid() || !IsValid(Widget))
	{
		return;
	}

	const TSharedRef<SWidget> SlateWidget = Widget->TakeWidget();
	RootOverlay->AddSlot(ZOrder)
	[
		SlateWidget
	];
	WidgetSlateContents.Add(Widget, SlateWidget);
}
