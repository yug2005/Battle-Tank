// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

UTankTrack::UTankTrack()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::BeginPlay()
{
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	MoveTrack();
    FixSlippage();
    CurrentThrottle = 0;
}

void UTankTrack::SetThrottle(float Throttle)
{
    CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);
}


void UTankTrack::MoveTrack()
{
    auto Force = CurrentThrottle * GetForwardVector() * DrivingForce;
    auto ForceLocation = GetComponentLocation();
    auto Tank = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    Tank->AddForceAtLocation(Force, ForceLocation);
}

void UTankTrack::FixSlippage()
{
    auto Tank = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
    auto SlippingSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
    auto CorrectionAcceleration = - SlippingSpeed * GetRightVector() / GetWorld()->GetDeltaSeconds();
    auto CorrectionForce = Tank->GetMass() * CorrectionAcceleration / 2;
    Tank->AddForce(CorrectionForce);
}