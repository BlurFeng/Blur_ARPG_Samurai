// Blur Feng All Rights Reserved.


#include "GameFramework/Items/Weapons/BlurWeaponObject.h"

#include "Components/BoxComponent.h"
#include "GameFramework/BlurFunctionLibrary.h"

ABlurWeaponObject::ABlurWeaponObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 创建武器Mesh。
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	// TODO: 并不是每个武器物体都需要碰撞器。
	
	// 创建武器碰撞盒。
	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	// 默认设置为无碰撞，仅在需要进行伤害检测时开启。
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 绑定武器碰撞盒回调方法。
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
}

void ABlurWeaponObject::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Notes：GetInstigator。
	// 在 Unreal Engine 中，GetInstigator() 是一个用于获取 Instigator（发起者） 的方法。
	// 通常定义在 AActor 类中。它的作用是返回与该 Actor 相关联的一个 Pawn 或 Controller，作为它的 "发起者"（Instigator）。
	
	const APawn* WeaponOwningPawn = GetInstigator<APawn>();
	checkf(WeaponOwningPawn, TEXT("Forgot to assign an instigator as the owning pawn for the weapon: %s"), *GetName());
	
	if (const APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		// 确认攻击可以产生效果的目标。
		if (EffectTargets == 0 ||
			EffectTargets & 1 << static_cast<int32>(UBlurFunctionLibrary::CheckTeamAttitude(WeaponOwningPawn, HitPawn)))
		{
			// 攻击对目标生效。
			OnWeaponMeleeHitTarget.ExecuteIfBound(OtherActor);
		}
	}
}

void ABlurWeaponObject::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const APawn* WeaponOwningPawn = GetInstigator<APawn>();
	checkf(WeaponOwningPawn, TEXT("Forgot to assign an instigator as the owning pawn for the weapon: %s"), *GetName());
	
	if (const APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		// 确认攻击可以产生效果的目标。
		if (EffectTargets == 0 ||
			EffectTargets & 1 << static_cast<int32>(UBlurFunctionLibrary::CheckTeamAttitude(WeaponOwningPawn, HitPawn)))
		{
			OnWeaponMeleePulledFromTarget.ExecuteIfBound(OtherActor);
		}
	}
}
