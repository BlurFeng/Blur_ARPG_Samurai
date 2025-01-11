// Blur Feng All Rights Reserved.

#include "GameFramework/Items/BlurProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/BlurFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Common/WeakObjectPtrExtensions.h"

// Sets default values
ABlurProjectile::ABlurProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Notes：在C++中设置碰撞器通道.

	// 创建炮弹碰撞器组件。
	ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	SetRootComponent(ProjectileCollisionBox);
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	// 碰撞回调函数。
	ProjectileCollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit);
	ProjectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);

	// 创建Niagara特效组件。
	ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
	ProjectileNiagaraComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 300.f; //炮弹初始速度。
	ProjectileMovementComponent->MaxSpeed = 400.f; //炮弹最大速度。
	ProjectileMovementComponent->Velocity = FVector(1.f, 0, 0); //炮弹发射力。
	ProjectileMovementComponent->ProjectileGravityScale = 0.f; //自定义重力。

	// Actor的存活时间。到时间后自动销毁。
	InitialLifeSpan = 4.f;
}

// Called when the game starts or when spawned
void ABlurProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnBeginOverlap)
	{
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

void ABlurProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	OnHitTarget(OtherActor, HitResult);
}

void ABlurProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep,
	const FHitResult& HitResult)
{
	// 确认是否已经是穿过的Actor。
	// Tips：我们没有清空OverlappedActors，因为投射物在销毁时会清空。
	if (WeakObjectPtrExtensions::Contains(OverlappedActors, OtherActor)) return;
	WeakObjectPtrExtensions::Add(OverlappedActors, OtherActor);

	OnHitTarget(OtherActor, HitResult);
}

void ABlurProjectile::OnHitTarget(AActor* OtherActor, const FHitResult& HitResult)
{
	bool bDestroy = false;
	
	// 击中了Pawn。
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		// 确认队伍。
		ETeamAttitudeFlags HitTargetTeamFlag = UBlurFunctionLibrary::CheckTeamAttitude(GetInstigator(), HitPawn);
		
		// 可以对目标造成GE效果。
		if (EffectTargets == 0 || EffectTargets & 1 << static_cast<int32>(HitTargetTeamFlag))
		{
			// 创建游玩事件数据。
			FGameplayEventData Data;
			Data.Instigator = this;
			Data.Target = HitPawn;

			bool Executed = false;

			// 确认目标是否配置了额外的击中目标事件Tag。
			for (const FHitTargetEventTagContainer EventTagContainer : HitTargetEventTagContainers)
			{
				// 确认目标身份。
				if (!UBlurFunctionLibrary::ActorHasMatchingGameplayTag(HitPawn, EventTagContainer.IdentityTag)) continue;

				Executed = true;
		
				// 确认是否被防御格挡。
				bool bIsValidBlock = false;
				if (UBlurFunctionLibrary::ActorHasMatchingGameplayTag(HitPawn, EventTagContainer.BlockingStatusTag))
				{
					bIsValidBlock = UBlurFunctionLibrary::IsValidBlock(this, HitPawn);
				}

				// 被格挡。
				if (bIsValidBlock)
				{
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						HitPawn,
						EventTagContainer.SuccessfulBlockEventTag,
						Data);
				}
				// 造成GE效果。
				else
				{
					// 对目标造成GE效果。比如伤害或回复。
					if (HandleApplyProjectileDamage(HitPawn, Data))
					{
						UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
							HitPawn,
							EventTagContainer.HitReactEventTag,
							Data);
					}
				}
			}

			// 没有配置击中目标事件时，直接尝试对HitPawn应用GE效果。
			if (!Executed)
			{
				HandleApplyProjectileDamage(HitPawn, Data);
			}
		}

		// 伤害策略为击中，而不是穿过。
		if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnHit)
		{
			// 可以阻挡炮弹。
			if (BlockTargets == 0 || BlockTargets & 1 << static_cast<int32>(HitTargetTeamFlag))
			{
				bDestroy = true;
			}
		}
	}
	// 击中了其他目标。
	else
	{
		// TODO：击中其他物体的效果。比如击中物理对象加力。或击中可破坏的物体。
		
		if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnHit)
		{
			bDestroy = true;
		}
	}
	
	if (bDestroy)
	{
		Destroy();
		// 通知蓝图炮弹击中，播放特效。
		BP_OnSpawnProjectileHitFX(HitResult.Location);
	}
}

bool ABlurProjectile::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload)
{
	checkf(ProjectileDamageEffectSpecHandle.IsValid(), TEXT("Forgot to assign a valid spec handle to the projectile: %s"), *GetActorNameOrLabel());

	// 将GE应用到击中目标。
	return UBlurFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), InHitPawn, ProjectileDamageEffectSpecHandle);
}
