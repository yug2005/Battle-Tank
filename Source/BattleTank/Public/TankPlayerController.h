// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void GetAimingComponent(UTankAimingComponent* AimingComponentReference);

public:
	void Aim(); // Calls the Aim function in the Tank Aiming Component
	bool GetHitLocation();
	bool GetLookDirection();
	bool GetLineTraceHitLocation();
	
private:
	UTankAimingComponent* AimingComponent = nullptr;
	
	// direction of the UI crosshair
	FVector LookDirection;
	// Location of where the crosshair hits the real world
	FVector HitLocation;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairX = 0.5;
	UPROPERTY(EditDefaultsOnly)
	float CrosshairY = 0.45;

};
