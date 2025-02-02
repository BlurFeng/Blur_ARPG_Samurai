// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/BlurActorBase.h"
#include "BlurWeapon.generated.h"

class UBoxComponent;

// 委托。用于通知CombatComponent。
DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*);

// 武器基类。武器能够赋予角色新的技能。
UCLASS()
class BLURARPGFRAMEWORK_API ABlurWeapon : public ABlurActorBase
{
	GENERATED_BODY()

public:
	ABlurWeapon();

	// Tips：武器只处理近战攻击。弓箭或投射物使用 BlurProjectile 类。
	
	FOnTargetInteractedDelegate OnWeaponMeleeHitTarget;
	FOnTargetInteractedDelegate OnWeaponMeleePulledFromTarget;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blur ARPG Framework | Weapons")
	UStaticMeshComponent* WeaponMesh;

	// 武器碰撞盒，用于检测攻击碰撞。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blur ARPG Framework | Weapons")
	UBoxComponent* WeaponCollisionBox;

	// 武器可以造成效果的目标。未配置时默认所有目标。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "ETeamAttitudeFlags"))
	int32 EffectTargets;
	
	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const {return WeaponCollisionBox;}
};
