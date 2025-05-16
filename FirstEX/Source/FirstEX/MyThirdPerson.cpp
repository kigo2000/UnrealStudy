// Fill out your copyright notice in the Description page of Project Settings.


#include "MyThirdPerson.h"
#include "Components/Capsulecomponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"

// Sets default values
AMyThirdPerson::AMyThirdPerson()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;

    CameraBoom
        = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera
        = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AMyThirdPerson::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyThirdPerson::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyThirdPerson::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    UEnhancedInputComponent* EnhancedPlayerInputComponent = 
        Cast<UEnhancedInputComponent>(PlayerInputComponent);

    if (EnhancedPlayerInputComponent != nullptr) {
        APlayerController* PlayerController = Cast<APlayerController>(GetController());

        if (PlayerController != nullptr) {
            UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

            if (EnhancedSubsystem != nullptr) {
                EnhancedSubsystem->AddMappingContext(IC_Character, 1);
            }
        }

        EnhancedPlayerInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMyThirdPerson::Move);
        EnhancedPlayerInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedPlayerInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
    }
}

void AMyThirdPerson::Move(const FInputActionValue& Value)
{
    FVector2D InputValue = Value.Get<FVector2D>();

    if (Controller != nullptr && (InputValue.X != 0.0f || InputValue.Y != 0.0f)) {
        const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

        if (InputValue.X != 0.0f) {
            const FVector RightDirection =
                UKismetMathLibrary::GetRightVector(YawRotation);
            AddMovementInput(RightDirection, InputValue.X);
        }

        if (InputValue.Y != 0.0f) {
            const FVector ForwardDirection = YawRotation.Vector();
            AddMovementInput(ForwardDirection, InputValue.Y);
        }
    }
}