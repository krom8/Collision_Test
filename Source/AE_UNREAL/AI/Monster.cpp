// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Monster.h"
#include <Global/GlobalGameInstance.h>
#include <Global/Data/MonsterData.h>
#include <Global/ProjectTile.h>
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "AIEnum.h"


void AMonster::BeginPlay()
{
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr != Inst)
	{
		CurMonsterData = Inst->GetMonsterData(DataName);

		SetAllAnimation(CurMonsterData->MapAnimation);
		SetAniState(AIState::DEATH);
	}

	Super::BeginPlay();

	GetBlackboardComponent()->SetValueAsEnum(TEXT("AIState"), static_cast<uint8>(AIState::IDLE));
	GetBlackboardComponent()->SetValueAsString(TEXT("TargetTag"), TEXT("Player"));
	GetBlackboardComponent()->SetValueAsFloat(TEXT("SearchRange"), 1500.0f);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), 200.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMonster::BeginOverLap);
	GetGlobalAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AMonster::AnimNotifyBegin);
}

void AMonster::BeginOverLap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	if (OtherComp->GetCollisionProfileName() == "PlayerAttack" && OtherActor->IsA(AGlobalCharacter::StaticClass()))
	{
		AGlobalCharacter* OtherGlobalActor = static_cast<AGlobalCharacter*>(OtherActor);
		AIAniState OtherAniStateValue = OtherGlobalActor->GetAniState<AIAniState>();
		if (OtherAniStateValue == AIAniState::Attack)
		{
			GetBlackboardComponent()->SetValueAsBool(TEXT("Attacked"), true);
		}
	}

	if (OtherActor->IsA(AProjectTile::StaticClass()))
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("Attacked"), true);
	}
}


void AMonster::AnimNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == TEXT("DeathEnd"))
	{
		Destroy();
	}
}