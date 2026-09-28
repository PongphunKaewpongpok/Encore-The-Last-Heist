// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Weretiger_HumanChase.generated.h"

/**
 * 
 */
UCLASS()
class ENCORE_THELASTHEIST1_API UBTTask_Weretiger_HumanChase : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Weretiger_HumanChase();
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	bool IsOnScreen(ACharacter* Me, ACharacter* Target);
};
