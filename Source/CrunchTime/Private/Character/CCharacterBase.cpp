// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CCharacterBase.h"

#include "GameplayAbilities/CAbilitySystemComponent.h"
#include "GameplayAbilities/CAttributeSet.h"
#include "GameplayAbilities/CAbilityGenericTags.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"

#include "Targeting/TargetingBoxComponent.h"

#include "Widgets/StatusGuage.h"

// Sets default values
ACCharacterBase::ACCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UCAbilitySystemComponent>("Ability System Component");
	AbilitySystemComponent->SetIsReplicated(true); // replicate means it is synced with the server.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCAttributeSet>("Attribute Set");

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetHealthAttribute()).AddUObject(this, &ACCharacterBase::HealthUpdated);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &ACCharacterBase::MaxHealthUpdated);
	
	StatusWidgetComp = CreateDefaultSubobject<UWidgetComponent>("Status Widget Comp");
	StatusWidgetComp->SetupAttachment(GetRootComponent());

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,  ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TargetingBoxComponent = CreateDefaultSubobject<UTargetingBoxComponent>("Targeting Box Component");
	TargetingBoxComponent->SetupAttachment(GetMesh());
}

void ACCharacterBase::SetupAbilitySystemComponent()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ACCharacterBase::InitAttributes()
{
	AbilitySystemComponent->ApplyInitialEffects();
}

void ACCharacterBase::InitAbilities()
{
	AbilitySystemComponent->GrantInitialAbilities();
}

// Called when the game starts or when spawned
void ACCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	InitStatusHUD();
}

// Called every frame
void ACCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACCharacterBase::PlayMontage(UAnimMontage* MontageToPlay)
{
	if (MontageToPlay)
	{
		if (GetMesh()->GetAnimInstance())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		}
	}
}

UAbilitySystemComponent* ACCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACCharacterBase::InitStatusHUD()
{
	StatusGuage = Cast<UStatusGuage>(StatusWidgetComp->GetUserWidgetObject());
	if (!StatusGuage)
	{
		UE_LOG(LogTemp, Error, TEXT("%s can't spawn status guage hud, status widget component has the wrong widget setup"), *GetName());


		return;
	}

	StatusGuage->SetRenderScale(FVector2D{0.5f});

	StatusGuage->SetHealth(AttributeSet->GetHealth(), AttributeSet->GetMaxHealth());

	if (IsLocallyControlled())
	{
		if(GetController() && GetController()->IsPlayerController())
			StatusGuage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ACCharacterBase::HealthUpdated(const FOnAttributeChangeData& ChangeData)
{
	if(StatusGuage)
		StatusGuage->SetHealth(ChangeData.NewValue, AttributeSet->GetMaxHealth());

	if (HasAuthority())
	{
		if (ChangeData.NewValue >= AttributeSet->GetMaxHealth())
		{
			AbilitySystemComponent->AddLooseGameplayTag(UCAbilityGenericTags::GetFullHealthTag());
		}
		else
		{
			AbilitySystemComponent->RemoveLooseGameplayTag(UCAbilityGenericTags::GetFullHealthTag());
		}
	}

	if (ChangeData.NewValue <= 0)
	{
		StartDeath();
	}
}

void ACCharacterBase::MaxHealthUpdated(const FOnAttributeChangeData& ChangeData)
{
	if(StatusGuage)
		StatusGuage->SetHealth(AttributeSet->GetHealth(), ChangeData.NewValue);
}

void ACCharacterBase::PlayHitReaction()
{
	PlayMontage(HitReactionMontage);
}

void ACCharacterBase::StartDeath()
{
	PlayMontage(DeathMontage);
	AbilitySystemComponent->ApplyGameplayEffect(DeathEffect);
}

