// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RETURN.h"


UBTTask_RETURN::UBTTask_RETURN()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}


EBTNodeResult::Type UBTTask_RETURN::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	GetGlobalCharacter(OwnerComp)->SetAniState(AIState::RETURN);
	return EBTNodeResult::Type::InProgress;

}

void UBTTask_RETURN::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DelataSeconds);
	FVector TargetPos = GetGlobalCharacter(OwnerComp)->WorldPosition;
	FVector ThisPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
	// 혹시라도 z축이 있을 가능성을 없애는게 보통입니다.
	TargetPos.Z = 0.0f;
	ThisPos.Z = 0.0f;

	FVector Distance = TargetPos - ThisPos;
	FVector Dir = TargetPos - ThisPos;
	Dir.Normalize();

	FVector OtherForward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
	OtherForward.Normalize();

	FVector Cross = FVector::CrossProduct(OtherForward, Dir);

	float Angle0 = Dir.Rotation().Yaw;
	float Angle1 = OtherForward.Rotation().Yaw;

	if (FMath::Abs(Angle0 - Angle1) >= 10.0f)
	{
		FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 500.0f * DelataSeconds });
		GetGlobalCharacter(OwnerComp)->AddActorWorldRotation(Rot);
	}
	else {
		FRotator Rot = Dir.Rotation();
		GetGlobalCharacter(OwnerComp)->SetActorRotation(Rot);
	}


	if (Distance.SizeSquared() > 1.f)
	{
		GetGlobalCharacter(OwnerComp)->AddMovementInput(Dir);
	}

	else
	{
		GetGlobalCharacter(OwnerComp)->SetActorLocation(TargetPos);
		SetStateChange(OwnerComp, AIState::IDLE);
	}
}



