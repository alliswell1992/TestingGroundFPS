// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseNextWayPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"
EBTNodeResult::Type UChooseNextWayPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	// get the patrol route
	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetPawn();
	auto PatrolRouteComp = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRouteComp)) return EBTNodeResult::Type::Failed;
	auto PatrolPoints = PatrolRouteComp->GetPatrolRoute();
	if (PatrolPoints.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT(" A guard is missing patrol points"));
		return EBTNodeResult::Type::Failed;
	}

	// set next waypoint
	auto BlackBoardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackBoardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackBoardComp->SetValueAsObject(PatrolPointKey.SelectedKeyName, PatrolPoints[Index]);
	//UE_LOG(LogTemp, Warning, TEXT("execute task C++"));

	// cycle the index
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackBoardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
	return EBTNodeResult::Type::Succeeded;
}