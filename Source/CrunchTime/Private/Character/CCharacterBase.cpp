// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CCharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "GameplayAbilities/CAbilitySystemComponent.h"
#include "GameplayAbilities/CAttributeSet.h"
#include "GameplayAbilities/CAbilityGenericTags.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"

#include "Net/UnrealNetwork.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Touch.h"

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
	AbilitySystemComponent->RegisterGameplayTagEvent(UCAbilityGenericTags::GetDeadTag()).AddUObject(this, &ACCharacterBase::DeathTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(UCAbilityGenericTags::GetAimingTag()).AddUObject(this, &ACCharacterBase::AimingTagChanged);

	StatusWidgetComp = CreateDefaultSubobject<UWidgetComponent>("Status Widget Comp");
	StatusWidgetComp->SetupAttachment(GetRootComponent());

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,  ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACCharacterBase::HitDetected);

	TargetingBoxComponent = CreateDefaultSubobject<UTargetingBoxComponent>("Targeting Box Component");
	TargetingBoxComponent->SetupAttachment(GetMesh());

	AIPerceptionSourceComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("AI Perception Source Comp");
	AIPerceptionSourceComp->RegisterForSense(UAISense_Sight::StaticClass());
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

void ACCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (NewController && !NewController->IsPlayerController())
	{
		SetupAbilitySystemComponent();
		InitAttributes();
		InitAbilities();
	}

	if (HasAuthority() && Controller && Controller->IsPlayerController())
	{
		APlayerController* OwningPlayerController = Cast<APlayerController>(Controller);
		//TODO: Figure out the ID
		TeamId = FGenericTeamId(1);
	}
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

void ACCharacterBase::AimingTagChanged(const FGameplayTag TagChanged, int32 NewStackCount)
{
	bIsAiming = NewStackCount != 0;
	AimingTagChanged(bIsAiming);
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
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AIPerceptionSourceComp->UnregisterFromPerceptionSystem();
	OnDeadStatusChanged.Broadcast(true);
}

void ACCharacterBase::DeathTagChanged(const FGameplayTag TagChanged, int32 NewStackCount)
{
	if (NewStackCount == 0)
	{
		StopAnimMontage(DeathMontage);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		AbilitySystemComponent->ApplyFullStat();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AIPerceptionSourceComp->RegisterWithPerceptionSystem();
		OnDeadStatusChanged.Broadcast(false);
	}
}

void ACCharacterBase::ClientPlayAnimMontage_Implementation(UAnimMontage* MontageToPlay)
{
	if(!HasAuthority())
		PlayAnimMontage(MontageToPlay);
}

void ACCharacterBase::ClientStopAnimMontage_Implementation(UAnimMontage* MontageToStop)
{
	if(!HasAuthority())
		StopAnimMontage(MontageToStop);
}

void ACCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ACCharacterBase, TeamId, COND_None);
}

void ACCharacterBase::HitDetected(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UAISense_Touch::ReportTouchEvent(this, OtherActor, this, GetActorLocation());
}

