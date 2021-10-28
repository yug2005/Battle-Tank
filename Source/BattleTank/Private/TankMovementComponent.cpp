// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack* SetLeftTrack, UTankTrack* SetRightTrack)
{
    LeftTrack = SetLeftTrack;
    RightTrack = SetRightTrack;
}

// Movement for Player
void UTankMovementComponent::MoveTank(float XAxis, float YAxis)
{
    if (!ensure(LeftTrack && RightTrack)) {return;}
    
    LeftTrack->SetThrottle(XAxis);
    RightTrack->SetThrottle(-XAxis);
    
    LeftTrack->SetThrottle(YAxis);
    RightTrack->SetThrottle(YAxis);
}

// Movement for AI Tanks 
void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    auto TankForwardVector = GetOwner()->GetActorForwardVector().GetSafeNormal();
    auto MoveDirection = MoveVelocity.GetSafeNormal();
    auto XAxisMovement = FVector::CrossProduct(TankForwardVector, MoveDirection).Z;
    auto YAxisMovement = FVector::DotProduct(TankForwardVector, MoveDirection);
    //Review cross product and dot product to understand this better
    UE_LOG(LogTemp, Warning, TEXT("%s : X Axis Movement : %f Y Axis Movement : %f"), *GetOwner()->GetName(), XAxisMovement, YAxisMovement);
    MoveTank(XAxisMovement, YAxisMovement);  
}