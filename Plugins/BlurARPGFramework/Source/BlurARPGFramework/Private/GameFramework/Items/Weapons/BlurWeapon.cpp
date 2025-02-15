// Blur Feng All Rights Reserved.


#include "GameFramework/Items/Weapons/BlurWeapon.h"

#include "GameFramework/Animation/BlurLinkedAnimLayer.h"
#include "EnhancedInputSubsystems.h"
#include "Components/BoxComponent.h"
#include "GameFramework/BlurFunctionLibrary.h"
#include "GameFramework/Characters/BlurCharacterBase.h"
#include "GameFramework/Components/UI/BlurCharacterUIComponent.h"

ABlurWeapon::ABlurWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);
}

void ABlurWeapon::BeginDestroy()
{
	Super::BeginDestroy();

	for (ABlurWeaponObject* WeaponObject : WeaponObjects)
	{
		WeaponObject->Destroy();
	}
	WeaponObjects.Empty();
}

void ABlurWeapon::Equip(AActor* SelfOwner)
{
	ABlurCharacterBase* BlurCharacterBase = Cast<ABlurCharacterBase>(SelfOwner);
	if (!BlurCharacterBase) return;
	OwnerCharacterWeakPtr = TWeakObjectPtr<ABlurCharacterBase>(BlurCharacterBase);

	// 生成武器的Actor并附加到相应的骨骼节点。
	if (WeaponObjects.IsEmpty())
	{
		if (UWorld* World = GetWorld())
		{
			// 生成参数。
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			SpawnParams.Owner = BlurCharacterBase;
			SpawnParams.Instigator = BlurCharacterBase;
		
			for (const FWeaponObjectInfo& WeaponObjectInfo : WeaponData.WeaponObjectInfos)
			{
				if (!WeaponObjectInfo.WeaponObjectClass) continue;

				// 创建武器部件。
				ABlurWeaponObject* WeaponObject =
					World->SpawnActor<ABlurWeaponObject>(WeaponObjectInfo.WeaponObjectClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
				WeaponObject->WeaponObjectInfo = WeaponObjectInfo;

				WeaponObjects.Add(WeaponObject);
			}
		}
	}
	
	if (!WeaponObjects.IsEmpty())
	{
		const FAttachmentTransformRules AttachmentTransformRules =
			FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepRelative,
				EAttachmentRule::KeepWorld,
				true);
		
		for (ABlurWeaponObject* WeaponObject : WeaponObjects)
		{
			WeaponObject->SetActorHiddenInGame(false);
			WeaponObject->SetActorEnableCollision(true);
			WeaponObject->SetActorTickEnabled(true);

			// 附加到拥有者Mesh的对应名称插槽。
			WeaponObject->AttachToComponent(
				BlurCharacterBase->GetCharacterMesh(),
				AttachmentTransformRules,
				WeaponObject->WeaponObjectInfo.AttachToSocketName);

			// 20250209 TODO: 绑定碰撞盒事件。暂不处理，我们可能使用新的碰撞盒方案。
		}
	}

	// 改变动画连接层。
	if (WeaponData.AnimLayerForEquip)
		BlurCharacterBase->GetCharacterMesh()->LinkAnimClassLayers(WeaponData.AnimLayerForEquip);
}

void ABlurWeapon::Unequip()
{
	ABlurCharacterBase* BlurCharacterBase;
	if (!GetOwnerCharacter(BlurCharacterBase)) return;

	const FDetachmentTransformRules AttachmentTransformRules =
			FDetachmentTransformRules(
				EDetachmentRule::KeepRelative,
				EDetachmentRule::KeepRelative,
				EDetachmentRule::KeepWorld,
				true);

	// 移除武器的Actor从骨骼节点。
	for (ABlurWeaponObject* WeaponObject : WeaponObjects)
	{
		WeaponObject->DetachFromActor(AttachmentTransformRules);
		WeaponObject->SetActorHiddenInGame(true);
		WeaponObject->SetActorEnableCollision(false);
		WeaponObject->SetActorTickEnabled(false);
	}

	// 改变动画连接层。
	if (WeaponData.AnimLayerForEquip)
		BlurCharacterBase->GetMesh()->UnlinkAnimClassLayers(WeaponData.AnimLayerForEquip);
}

void ABlurWeapon::EnterCombat()
{
	ABlurCharacterBase* BlurCharacterBase;
	if (!GetOwnerCharacter(BlurCharacterBase)) return;
	
	// 添加武器的输入映射到本地玩家。
	if (const APlayerController* PlayerController = Cast<APlayerController>(BlurCharacterBase->GetController()))
	{
		if (const ULocalPlayer* LocalPlayer =  PlayerController->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

			// 用优先级更高的武器输入映射覆盖角色的默认输入映射。
			// 多个输入映射可以同时存在，只有发生冲突的输入会优先使用优先级高的。所以在武器输入映射中，没必要重新配置基础移动的部分。
			Subsystem->AddMappingContext(WeaponData.WeaponInputMappingContext, WeaponData.WeaponInputMappingPriority);
		}
	}

	// 改变动画连接层。根据武器不同，我们会有不同的动画表现。
	// Tips: 如果你使用PoseSearch等插件来实现动画，根据实际情况可以不配置链接动画层。
	if (WeaponData.AnimLayerForEquip)
		BlurCharacterBase->GetMesh()->UnlinkAnimClassLayers(WeaponData.AnimLayerForEquip);
	if (WeaponData.AnimLayerForEnterCombat)
		BlurCharacterBase->GetMesh()->LinkAnimClassLayers(WeaponData.AnimLayerForEnterCombat);

	// 调用拿出武器时的UI更新委托。
	if (BlurCharacterBase->GetCharacterUIComponent())
	{
		// 更新武器Icon。
		BlurCharacterBase->GetCharacterUIComponent()->OnCombatWeaponChanged.Broadcast(WeaponData.SoftWeaponIconTexture);
	}
}

void ABlurWeapon::ExitCombat()
{
	ABlurCharacterBase* BlurCharacterBase;
	if (!GetOwnerCharacter(BlurCharacterBase)) return;
	
	// 移除武器的输入映射从本地玩家。
	if (const APlayerController* PlayerController = Cast<APlayerController>(BlurCharacterBase->GetController()))
	{
		if (const ULocalPlayer* LocalPlayer =  PlayerController->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

			// 用优先级更高的武器输入映射覆盖角色的默认输入映射。
			// 多个输入映射可以同时存在，只有发生冲突的输入会优先使用优先级高的。所以在武器输入映射中，没必要重新配置基础移动的部分。
			Subsystem->RemoveMappingContext(WeaponData.WeaponInputMappingContext);
		}
	}

	// 改变动画连接层。
	if (WeaponData.AnimLayerForEnterCombat)
		BlurCharacterBase->GetMesh()->UnlinkAnimClassLayers(WeaponData.AnimLayerForEnterCombat);
	if (WeaponData.AnimLayerForEquip)
		BlurCharacterBase->GetMesh()->LinkAnimClassLayers(WeaponData.AnimLayerForEquip);

	// 调用放回武器时的UI更新委托。
	if (BlurCharacterBase->GetCharacterUIComponent())
	{
		// 更新武器Icon为空。
		BlurCharacterBase->GetCharacterUIComponent()->OnCombatWeaponChanged.Broadcast(nullptr);
	}
}

bool ABlurWeapon::GetOwnerCharacter(ABlurCharacterBase*& OwnerCharacter) const
{
	if (!OwnerCharacterWeakPtr.IsValid()) return false;
	OwnerCharacter = OwnerCharacterWeakPtr.Get();
	return OwnerCharacter != nullptr;
}

void ABlurWeapon::SetCollisionEnabled(const ECollisionEnabled::Type CollisionEnabled)
{
	for (const ABlurWeaponObject* WeaponObject : WeaponObjects)
	{
		if (!WeaponObject) continue;
		WeaponObject->GetCollisionBox()->SetCollisionEnabled(CollisionEnabled);
	}
}