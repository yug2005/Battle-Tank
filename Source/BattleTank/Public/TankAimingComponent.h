// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

UENUM()
enum class EFiringState : uint8
{
	Reloading, 
	Aiming, 
	Locked,
	OutofAmmo
};

class UTankBarrel; 
class UTankTurret;
class ATankProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTankAimingComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UTankBarrel* SetBarrel, UTankTurret* SetTurret);

	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringState FiringState = EFiringState::Reloading;

public:
	void Aim(FVector AimLocation);
	bool GetAimDirection(FVector AimLocation);
	void MoveBarrelAndTurret();
	bool IsBarrelLocked();

	EFiringState GetFiringState() const;
	
	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();

	UPROPERTY(EditAnywhere, Category = "Firing")
	float TankRange = 500000;

private:
	UTankBarrel* Barrel = nullptr;		
	UTankTurret* Turret = nullptr;

	FVector AimDirection; 

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ATankProjectile> TankProjectile = nullptr;

	UPROPERTY(EditAnywhere, Category = "Firing")
	float FireSpeed = 7500;

	UPROPERTY(EditAnywhere, Category = "Firing")
	float ReloadTime = 2;

	UPROPERTY(EditAnywhere, Category = "Firing")
	int32 Ammo = 20;
	
	float GameStart = 5;
	double LastFireTime = GameStart - ReloadTime;
};
