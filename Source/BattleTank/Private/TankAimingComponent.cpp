// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankProjectile.h"
#include "TankTurret.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTankAimingComponent::Initialize(UTankBarrel* SetBarrel, UTankTurret* SetTurret)
{
	Barrel = SetBarrel;
	Turret = SetTurret;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{	
	if (Ammo == 0)
	{
		FiringState = EFiringState::OutofAmmo;
	}
	else if ((GetWorld()->GetTimeSeconds() - LastFireTime) < ReloadTime)
	{
		FiringState = EFiringState::Reloading;
	}
	else if (!IsBarrelLocked())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

int32 UTankAimingComponent::GetAmmo() const
{
	return Ammo;
}

void UTankAimingComponent::Aim(FVector AimLocation)
{
	if (!ensure(Barrel && Turret)) {return;}
	if (GetAimDirection(AimLocation))
	{
		MoveBarrelAndTurret();
	}
}

bool UTankAimingComponent::GetAimDirection(FVector AimLocation)
{
	FVector FireVelocity;
	bool bGetFireVelocity = UGameplayStatics::SuggestProjectileVelocity(
		this,
		FireVelocity,
		Barrel->GetSocketLocation(FName("Projectile")),
		AimLocation,
		FireSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	if (bGetFireVelocity)
	{
		AimDirection = FireVelocity.GetSafeNormal();
	}
	return bGetFireVelocity;
}

void UTankAimingComponent::MoveBarrelAndTurret()
{
	FRotator BarrelRotation = Barrel->GetForwardVector().Rotation();
	FRotator AimRotation = AimDirection.Rotation();
	FRotator DeltaRotation = AimRotation - BarrelRotation;

	Barrel->Elevate(DeltaRotation.Pitch);
	if (FMath::Abs(DeltaRotation.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotation.Yaw);
	}
	else 
	{
		Turret->Rotate(-DeltaRotation.Yaw);
	}
}

bool UTankAimingComponent::IsBarrelLocked()
{
	if (!ensure(Barrel)) {return false;}
	auto BarrelForwardVector = Barrel->GetForwardVector();
	return BarrelForwardVector.Equals(AimDirection, 0.01);
}

void UTankAimingComponent::Fire()
{
	if (FiringState == EFiringState::Aiming || FiringState == EFiringState::Locked)
	{
		if (!ensure(Barrel)) {return;}
		if (!ensure(TankProjectile)) {return;}
		auto Projectile = GetWorld()->SpawnActor<ATankProjectile>(
			TankProjectile, 
			Barrel->GetSocketLocation(FName("Projectile")), 
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->Fire(FireSpeed);
		LastFireTime = GetWorld()->GetTimeSeconds();
		Ammo--;
	}
}

