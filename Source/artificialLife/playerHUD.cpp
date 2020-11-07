// Fill out your copyright notice in the Description page of Project Settings.


#include "playerHUD.h"
//#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "CanvasItem.h"


AplayerHUD::AplayerHUD() {
    static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("/Game/playerHUD/playerHPbar"));
    HUDWidgetClass = HealthBarObj.Class;
}


// Code from unreals crosshair tutorial documentation
void AplayerHUD::DrawHUD() {
    Super::DrawHUD();
    //UE_LOG(LogTemp, Warning, TEXT("playerHUD start"));
    if (CrosshairTexture)
    {
        // Find the center of our canvas.
        FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

        // Offset by half of the texture's dimensions so that the center of the texture aligns with the center of the Canvas.
        FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));

        // Draw the crosshair at the centerpoint.
        FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
        TileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TileItem);
        //UE_LOG(LogTemp, Warning, TEXT("playerHUD start inside IF"));
    }

}

void AplayerHUD::BeginPlay() {

    Super::BeginPlay();

    if (HUDWidgetClass != nullptr) {
        playerHPWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

        if (playerHPWidget) {
            playerHPWidget->AddToViewport();
        }
    }
}