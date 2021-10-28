// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
public: 
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Aim();

private:
	APawn* TargetTank = nullptr;
	UTankAimingComponent* AimingComponent = nullptr;

	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 7500;
};
