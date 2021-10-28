// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"

void ATankAIController::BeginPlay() 
{
    Super::BeginPlay();
    AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    // To Do : Look for the nearest tank rather than the player tank everytime
}

void ATankAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TargetTank = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (!ensure(TargetTank && AimingComponent)) {return;}
    MoveToActor(TargetTank, AcceptanceRadius);
    Aim(); // Calls Aim function in the Tank Aiming Component
    if (AimingComponent->GetFiringState() == EFiringState::Locked)
    {
        AimingComponent->Fire(); // To do : Only fire if the AI Tank is locked
    }
}

void ATankAIController::Aim()
{
    // To do : AI Tank looks for nearby tank NOT player tank
    auto TargetLocation = TargetTank->GetActorLocation();
    auto Distance = FVector::Distance(TargetLocation, GetPawn()->GetActorLocation());
    if (Distance < AimingComponent->TankRange)
    {
        AimingComponent->Aim(TargetLocation);
    }
}