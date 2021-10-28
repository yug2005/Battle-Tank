// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();
    AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if (!ensure(AimingComponent)) {return;}
    GetAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    Aim(); // Calls the Aim function in the Tank Aiming Component
}

// Aims the tank at the aim point
void ATankPlayerController::Aim()
{
    if (!ensure(AimingComponent)) {return;}
    if (GetHitLocation())
    {
        AimingComponent->Aim(HitLocation);
    }
}

// Get the location of the hit point 
bool ATankPlayerController::GetHitLocation() 
{
    if (GetLookDirection())
    {
        return GetLineTraceHitLocation();
    }
    return false;
}

// Gets the look direction of the UI Crosshair
bool ATankPlayerController::GetLookDirection() 
{
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    FVector CameraWorldLocation;
    return DeprojectScreenPositionToWorld(ViewportSizeX * CrosshairX, ViewportSizeY * CrosshairY, CameraWorldLocation, LookDirection);
}

// Single Line Trace in the aim direction until interception and returns the location of the interception.
bool ATankPlayerController::GetLineTraceHitLocation()
{
    FHitResult OutHit;
    FVector LineTraceStart = PlayerCameraManager->GetCameraLocation();
    FVector LineTraceEnd = LineTraceStart + (LookDirection * AimingComponent->TankRange);  
    FCollisionQueryParams Params(NAME_None, false, GetPawn());
    if (GetWorld()->LineTraceSingleByChannel(
            OutHit,
            LineTraceStart,
            LineTraceEnd,
            ECollisionChannel::ECC_Visibility,
            Params
            )
        ) 
    {
        HitLocation = OutHit.Location;
        return true;
    }
    return false;
}