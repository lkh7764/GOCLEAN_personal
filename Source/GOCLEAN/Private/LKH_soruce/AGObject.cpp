// AGObejct: default object abstract class in GOCLEAN


#include "LKH_soruce/AGObject.h"

// Sets default values
AAGObject::AAGObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAGObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAGObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ChangeObjId(int id)
{
	if (id<0 && id>ObjMax)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ChangeObjId] Invalid id"));
		return;
	}

	objid = id;
	// set mesh
}

