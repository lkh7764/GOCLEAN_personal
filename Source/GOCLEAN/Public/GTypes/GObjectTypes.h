#pragma once

#include "CoreMinimal.h"

// enum 및 포함하는 타입이 언리얼 리플렉션 시스템을 사용하지 않는다면 제거해도 됨
#include "GObjectTypes.generated.h"



UENUM(BlueprintType)
enum class EGFixedObjState : uint8 {
	E_Static			UMETA(DisplayName = "Static - 정적"),
	E_Used				UMETA(DisplayName = "Used - 사용중"),
	E_Destroyed			UMETA(DisplayName = "Destroyed - 파괴됨"),

	E_MAX				UMETA(Hidden)
};



UENUM(BlueprintType)
enum class ENonfixedObjState : uint8 {
	E_Static			UMETA(DisplayName = "Static - 정적"),
	E_Kinematic			UMETA(DisplayName = "Kinematic - 운동중"),
	E_Invisible			UMETA(DisplayName = "Invisible - 비가시적"),
	E_Disintegrating	UMETA(DisplayName = "Disintegrating - 소각 중"),
	E_Destroyed			UMETA(DisplayName = "Destroyed - 파괴됨"),
	E_Temporary			UMETA(DisplayName = "Temporary - 임시 생성"),

	E_None				UMETA(DisplayName = "None - 오브젝트 할당되지 않음"),

	E_MAX			UMETA(Hidden)
};



USTRUCT(BlueprintType)
struct FGNonfixedObjData 
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SpawnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ENonfixedObjState SpawnState = ENonfixedObjState::E_Static;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseGravity = true;
};
