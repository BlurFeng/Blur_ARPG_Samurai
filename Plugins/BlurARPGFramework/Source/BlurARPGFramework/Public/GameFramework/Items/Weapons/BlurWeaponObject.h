// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Items/BlurItemBase.h"
#include "GameFramework/Types/BlurStructTypes.h"
#include "BlurWeaponObject.generated.h"

class UBoxComponent;
// 武器物体基类。用于生成使用的武器部件，比如刀鞘和刀。

// 委托。用于通知CombatComponent。
DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*);

UCLASS()
class BLURARPGFRAMEWORK_API ABlurWeaponObject : public ABlurItemBase
{
	GENERATED_BODY()

public:
	ABlurWeaponObject();

	// 部件数据，由所属的 Weapon 类的 WeaponData.WeaponObjectInfos 数据中配置。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blur ARPG Framework | Weapons")
	FWeaponObjectInfo WeaponObjectInfo;

	FORCEINLINE UBoxComponent* GetCollisionBox() const {return WeaponCollisionBox;}

	FOnTargetInteractedDelegate OnWeaponMeleeHitTarget;
	FOnTargetInteractedDelegate OnWeaponMeleePulledFromTarget;
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blur ARPG Framework | Weapons")
	UStaticMeshComponent* WeaponMesh;

	// 武器碰撞盒，用于检测攻击碰撞。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blur ARPG Framework | Weapons")
	UBoxComponent* WeaponCollisionBox;

	// 武器可以造成效果的目标类型。未配置时默认所有目标。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "ETeamAttitudeFlags"))
	int32 EffectTargets;
	
	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
