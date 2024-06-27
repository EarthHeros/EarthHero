#include "Options.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "EHGameInstance.h"

void UOptions::NativeConstruct()
{
    Super::NativeConstruct();

    if (ResolutionComboBox)
    {
        ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UOptions::OnResolutionChanged);
    }

    if (ScreenModeComboBox)
    {
        ScreenModeComboBox->OnSelectionChanged.AddDynamic(this, &UOptions::OnScreenModeChanged);
    }

    if (MaxFrameComboBox)
    {
        MaxFrameComboBox->OnSelectionChanged.AddDynamic(this, &UOptions::OnMaxFrameChanged);
    }

    if (VSyncComboBox)
    {
        VSyncComboBox->OnSelectionChanged.AddDynamic(this, &UOptions::OnVSyncChanged);
    }

    if (OverallQualityComboBox)
    {
        OverallQualityComboBox->OnSelectionChanged.AddDynamic(this, &UOptions::OnOverallQualityChanged);
    }

    if (AntiAliasingComboBox)
    {
        AntiAliasingComboBox->OnSelectionChanged.AddDynamic(this, &UOptions::OnAntiAliasingChanged);
    }

    if (PostProcessingComboBox)
    {
        PostProcessingComboBox->OnSelectionChanged.AddDynamic(this, &UOptions::OnPostProcessingChanged);
    }

    if (MasterVolumeSlider)
    {
        MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UOptions::OnMasterVolumeChanged);
    }

    if (BackgroundVolumeSlider)
    {
        BackgroundVolumeSlider->OnValueChanged.AddDynamic(this, &UOptions::OnBackgroundVolumeChanged);
    }

    if (SFXVolumeSlider)
    {
        SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UOptions::OnSFXVolumeChanged);
    }

    if (MouseSensitivitySlider)
    {
        MouseSensitivitySlider->OnValueChanged.AddDynamic(this, &UOptions::OnMouseSensitivityChanged);
    }

    InitializeOptions();
}

void UOptions::InitializeOptions()
{
    auto GameInstance = Cast<UEHGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        // Set ComboBox selections based on loaded settings
        if (ResolutionComboBox)
        {
            ResolutionComboBox->SetSelectedOption(FString::FromInt(GameInstance->Resolution));
        }

        if (ScreenModeComboBox)
        {
            ScreenModeComboBox->SetSelectedOption(ScreenModeToString(GameInstance->ScreenMode));
        }

        if (MaxFrameComboBox)
        {
            MaxFrameComboBox->SetSelectedOption(FString::FromInt(GameInstance->MaxFrame));
        }

        if (VSyncComboBox)
        {
            VSyncComboBox->SetSelectedOption(GameInstance->bVSyncEnabled ? TEXT("켜기") : TEXT("끄기"));
        }

        if (OverallQualityComboBox)
        {
            OverallQualityComboBox->SetSelectedOption(QualityToString(GameInstance->OverallQuality));
        }

        if (AntiAliasingComboBox)
        {
            AntiAliasingComboBox->SetSelectedOption(QualityToString(GameInstance->AntiAliasing));
        }

        if (PostProcessingComboBox)
        {
            PostProcessingComboBox->SetSelectedOption(QualityToString(GameInstance->PostProcessing));
        }

        // Set Slider values based on loaded settings
        if (MasterVolumeSlider)
        {
            MasterVolumeSlider->SetValue(GameInstance->MasterVolume);
        }

        if (BackgroundVolumeSlider)
        {
            BackgroundVolumeSlider->SetValue(GameInstance->BackgroundVolume);
        }

        if (SFXVolumeSlider)
        {
            SFXVolumeSlider->SetValue(GameInstance->SFXVolume);
        }

        if (MouseSensitivitySlider)
        {
            MouseSensitivitySlider->SetValue(GameInstance->MouseSensitivity);
        }
    }
}

void UOptions::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    auto GameInstance = Cast<UEHGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->Resolution = FCString::Atoi(*SelectedItem);
        GameInstance->SaveSettings();
    }
}

void UOptions::OnScreenModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    auto GameInstance = Cast<UEHGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->ScreenMode = ScreenModeFromString(SelectedItem);
        GameInstance->SaveSettings();
    }
}

void UOptions::OnMaxFrameChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    auto GameInstance = Cast<UEHGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->MaxFrame = FCString::Atoi(*SelectedItem);
        GameInstance->SaveSettings();
    }
}

void UOptions::OnVSyncChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    auto GameInstance = Cast<UEHGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->bVSyncEnabled = (SelectedItem == TEXT("켜기"));
        GameInstance->SaveSettings();
    }
}

void UOptions::OnOverallQualityChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    auto GameInstance = Cast<UEHGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->OverallQuality = QualityFromString(SelectedItem);
        GameInstance->SaveSettings();
    }
}

void UOptions::OnAntiAliasingChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    auto GameInstance = Cast<UEHGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->AntiAliasing = QualityFromString(SelectedItem);
        GameInstance->SaveSettings();
    }
}

void UOptions::OnPostProcessingChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    auto GameInstance = Cast<UEHGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->PostProcessing = QualityFromString(SelectedItem);
        GameInstance->SaveSettings();
    }
}

void UOptions::OnMasterVolumeChanged(float Value)
{
    auto GameInstance = Cast<UEHGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->MasterVolume = Value;
        GameInstance->SaveSettings();
    }
}

void UOptions::OnBackgroundVolumeChanged(float Value)
{
    auto GameInstance = Cast<UEHGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->BackgroundVolume = Value;
        GameInstance->SaveSettings();
    }
}

void UOptions::OnSFXVolumeChanged(float Value)
{
    auto GameInstance = Cast<UEHGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->SFXVolume = Value;
        GameInstance->SaveSettings();
    }
}

void UOptions::OnMouseSensitivityChanged(float Value)
{
    auto GameInstance = Cast<UEHGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->MouseSensitivity = Value;
        GameInstance->SaveSettings();
    }
}

FString UOptions::QualityToString(int32 Quality)
{
    switch (Quality)
    {
        case 0: return TEXT("최하");
        case 1: return TEXT("하");
        case 2: return TEXT("중");
        case 3: return TEXT("상");
        case 4: return TEXT("최상");
        case 5: return TEXT("커스텀");
        default: return TEXT("커스텀");
    }
}

int32 UOptions::QualityFromString(const FString& QualityString)
{
    if (QualityString == TEXT("최하")) return 0;
    if (QualityString == TEXT("하")) return 1;
    if (QualityString == TEXT("중")) return 2;
    if (QualityString == TEXT("상")) return 3;
    if (QualityString == TEXT("최상")) return 4;
    if (QualityString == TEXT("커스텀")) return 5;
    return 5;
}

FString UOptions::ScreenModeToString(int32 ScreenMode)
{
    switch (ScreenMode)
    {
        case 0: return TEXT("전체 화면");
        case 1: return TEXT("전체 창모드");
        case 2: return TEXT("창모드");
        default: return TEXT("창모드");
    }
}

int32 UOptions::ScreenModeFromString(const FString& ScreenModeString)
{
    if (ScreenModeString == TEXT("전체 화면")) return 0;
    if (ScreenModeString == TEXT("전체 창모드")) return 1;
    if (ScreenModeString == TEXT("창모드")) return 2;
    return 2;
}
