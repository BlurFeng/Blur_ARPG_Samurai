// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Items/BlurItemBase.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Types/BlurEnumTypes.h"

#include "BlurProjectileBase.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class UNiagaraComponent;
struct FGameplayEventData;

// 炮弹造成伤害方式。
UENUM(BlueprintType)
enum class EProjectileDamagePolicy : uint8
{
	// 当击中时造成效果，且在击中后销毁。
	OnHit,
	
	// 当开始穿过时造成效果，且在击中目标后穿过，不销毁。
	OnBeginOverlap,
};

// 炮弹击中目标事件Tag数据。
// 如果击中的目标可以格挡或有被击中反应，添加Tag配置用于确认和通知。
USTRUCT()
struct FHitTargetEventTagContainer
{
	GENERATED_BODY()

public:
	FGameplayTag IdentityTag;
	FGameplayTag BlockingStatusTag;
	FGameplayTag SuccessfulBlockEventTag;
	FGameplayTag HitReactEventTag;
};

// 炮弹抛射物基类。可以用于实现箭矢，火焰弹等游戏功能。
UCLASS()
class BLURARPGFRAMEWORK_API ABlurProjectileBase : public ABlurItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlurProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 炮弹伤害策略，击中还是穿过。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Projectile")
	EProjectileDamagePolicy ProjectileDamagePolicy;

	// 炮弹可以造成效果的目标。未配置时默认所有目标。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "ETeamAttitudeFlags"))
	int32 EffectTargets;

	// 可以阻挡炮弹目标。当 ProjectileDamagePolicy == OnHit 时，但炮弹击中第一个可阻挡自身的目标时会销毁。未配置时默认所有目标。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "ETeamAttitudeFlags"))
	int32 BlockTargets;

	// 炮弹碰撞器。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, category = "Projectile")
	UBoxComponent* ProjectileCollisionBox;

	// 炮弹Niagara特效。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, category = "Projectile")
	UNiagaraComponent* ProjectileNiagaraComponent;

	// 炮弹移动组件。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	// Notes：ExposeOnSpawn 暴露数据在生成时。
	// 使用 ExposeOnSpawn 可以让某个成员参数在蓝图中Spawn生成时，直接显示在Spawn节点中供传入。
	
	// 炮弹伤害效果配置数据。
	UPROPERTY(BlueprintReadOnly, category = "Projectile", meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle;

	// 当炮弹撞击时。
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	// 当炮弹开始穿过时。
	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);

	// 生成炮弹击中特效事件。由蓝图实现。
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Spawn Projectile Hit FX"))
	void BP_OnSpawnProjectileHitFX(const FVector& HitLocation);

private:

	void OnHitTarget(AActor* OtherActor, const FHitResult& HitResult);
	
	// 应用炸弹伤害GE到目标Pawn。
	bool HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload);

	// 被穿过的物体。
	TArray<TWeakObjectPtr<AActor>> OverlappedActors;

	// 有格挡或被击中反应的目标，添加Tag数据用于确认和通知。
	TArray<FHitTargetEventTagContainer> HitTargetEventTagContainers;
};
