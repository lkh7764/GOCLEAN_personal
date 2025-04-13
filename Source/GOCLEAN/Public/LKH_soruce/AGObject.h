// AGObejct: default object abstract class in GOCLEAN

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AGObject.generated.h"

UCLASS()
class GOCLEAN_API AAGObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAGObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void ChangeObjId(int id);


private:
	int objid;
	
	UPROPERTY()
	UStaticMesh* meshcomponent;

protected:
	static int ObjMax = 70;
};
