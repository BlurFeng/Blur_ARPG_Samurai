// Blur Feng All Rights Reserved.


#include "GameFramework/Items/Weapons/BlurWeapon.h"

#include "GameFramework/Animation/BlurLinkedAnimLayer.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Components/BoxComponent.h"
#include "GameFramework/BlurFunctionLibrary.h"
#include "GameFramework/Characters/BlurCharacterBase.h"
#include "GameFramework/Characters/BlurPlayableCharacter.h"
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

	// 生成武器的Actor。
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

	// 附加武器到对应拥有者骨骼插槽。
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
	if (WeaponData.AnimLayerWithEquip)
		BlurCharacterBase->GetCharacterMesh()->LinkAnimClassLayers(WeaponData.AnimLayerWithEquip);

	// 添加装备武器时的输入映射到本地玩家。
	if (ABlurPlayableCharacter* PlayableCharacter = Cast<ABlurPlayableCharacter>(OwnerCharacterWeakPtr.Get()))
	{
		PlayableCharacter->AddInputConfigWithAbilities(WeaponData.InputConfigWithEquip, InputBindingHandles_Equip);
	}
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
	if (WeaponData.AnimLayerWithEquip)
		BlurCharacterBase->GetMesh()->UnlinkAnimClassLayers(WeaponData.AnimLayerWithEquip);

	// 移除装备武器时的输入映射从本地玩家。
	if (ABlurPlayableCharacter* PlayableCharacter = Cast<ABlurPlayableCharacter>(OwnerCharacterWeakPtr.Get()))
	{
		PlayableCharacter->RemoveInputConfigWithAbilities(WeaponData.InputConfigWithEquip, InputBindingHandles_Equip);
	}
}

void ABlurWeapon::EnterCombat()
{
	ABlurCharacterBase* BlurCharacterBase;
	if (!GetOwnerCharacter(BlurCharacterBase)) return;
	
	// 改变动画连接层。根据武器不同，我们会有不同的动画表现。
	// Tips: 如果你使用PoseSearch等插件来实现动画，根据实际情况可以不配置链接动画层。
	if (WeaponData.AnimLayerWithEquip)
		BlurCharacterBase->GetMesh()->UnlinkAnimClassLayers(WeaponData.AnimLayerWithEquip);
	if (WeaponData.AnimLayerWithEnterCombat)
		BlurCharacterBase->GetMesh()->LinkAnimClassLayers(WeaponData.AnimLayerWithEnterCombat);

	// 添加使用武器时的输入映射到本地玩家。
	if (ABlurPlayableCharacter* PlayableCharacter = Cast<ABlurPlayableCharacter>(OwnerCharacterWeakPtr.Get()))
	{
		PlayableCharacter->AddInputConfigWithAbilities(WeaponData.InputConfigWithCombat, InputBindingHandles_Combat);
	}

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
	
	// 改变动画连接层。
	if (WeaponData.AnimLayerWithEnterCombat)
		BlurCharacterBase->GetMesh()->UnlinkAnimClassLayers(WeaponData.AnimLayerWithEnterCombat);
	if (WeaponData.AnimLayerWithEquip)
		BlurCharacterBase->GetMesh()->LinkAnimClassLayers(WeaponData.AnimLayerWithEquip);

	// 移除使用武器时的输入映射从本地玩家。
	if (ABlurPlayableCharacter* PlayableCharacter = Cast<ABlurPlayableCharacter>(OwnerCharacterWeakPtr.Get()))
	{
		PlayableCharacter->RemoveInputConfigWithAbilities(WeaponData.InputConfigWithCombat, InputBindingHandles_Combat);
	}

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

ABlurWeaponObject* ABlurWeapon::GetWeaponObjectFirst()
{
	if (WeaponObjects.IsEmpty()) return nullptr;

	return WeaponObjects[0];
}

ABlurWeaponObject* ABlurWeapon::GetWeaponObject(const FName WeaponObjectName)
{
	if (WeaponObjects.IsEmpty()) return nullptr;

	for (ABlurWeaponObject* WeaponObject : WeaponObjects)
	{
		if (WeaponObject->WeaponObjectInfo.Name.IsEqual(WeaponObjectName))
			return WeaponObject;
	}

	return nullptr;
}

void ABlurWeapon::SetCollisionEnabled(const ECollisionEnabled::Type CollisionEnabled)
{
	for (const ABlurWeaponObject* WeaponObject : WeaponObjects)
	{
		if (!WeaponObject) continue;
		WeaponObject->GetCollisionBox()->SetCollisionEnabled(CollisionEnabled);
	}
}
