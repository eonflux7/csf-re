#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(CMenuLanguage*, __fastcall, CMenuLanguage_Ctor, 0x00534540, (CMenuLanguage* self, void* , int ownerArg));
CSF_FUNC(CMenuOptions*, __fastcall, CMenuOptions_Construct, 0x0053AF80, (CMenuOptions* self, void* , int argument));
CSF_FUNC(CMenuCredits*, __fastcall, CMenuCredits_Initialize, 0x0052F9E0, (CMenuCredits* self, void* , int));
CSF_FUNC(CMenuExtras*, __fastcall, CMenuExtras_Construct_547CB0, 0x00547CB0, (CMenuExtras* self, void* , int menuArg));
CSF_FUNC(CMenuOptionsPC*, __fastcall, CMenuOptionsPC_Construct, 0x0053B820, (CMenuOptionsPC* self, void* , int a2));
CSF_FUNC(CMenuTextInput*, __fastcall, CMenuTextInput_Construct, 0x0052C260, (CMenuTextInput* self, void* , int ownerArg));
CSF_FUNC(CMenuProgressBar*, __fastcall, CMenuProgressBar_Construct, 0x0052D680, (CMenuProgressBar* self, void* , int progressOwner));
CSF_FUNC(int, __fastcall, CMenuMessageBox_Construct, 0x0052B9C0, (void* self, void* edx, int a2));
CSF_FUNC(CMenuIntroVideo*, __fastcall, CMenuIntroVideo_Construct, 0x00535790, (CMenuIntroVideo* self, void* , int a2));
CSF_FUNC(int*, __fastcall, C_008414e8_Construct, 0x0052CFF0, (void* self, void* edx, int a2));

static int __fastcall CMenuManager_InitializeBaseScreens(
    CMenuManager* self, void* /*edx*/, int ownerArg)
{
    auto* language = reinterpret_cast<CMenuLanguage*>(CoreMemory_AllocUntagged(92));
    if (language)
        language = CMenuLanguage_Ctor(language, nullptr, ownerArg);
    uint32_t guiRoot = self->m_pGuiRoot;
    self->field_00ec = reinterpret_cast<uint32_t>(language);
    CSF_AT(uint32_t, language, 0x008) = guiRoot;

    auto* options = reinterpret_cast<CMenuOptions*>(CoreMemory_AllocUntagged(56));
    if (options)
        options = CMenuOptions_Construct(options, nullptr, ownerArg);
    guiRoot = self->m_pGuiRoot;
    self->field_003c = reinterpret_cast<uint32_t>(options);
    CSF_AT(uint32_t, options, 0x008) = guiRoot;

    auto* credits = reinterpret_cast<CMenuCredits*>(CoreMemory_AllocUntagged(136));
    if (credits)
        credits = CMenuCredits_Initialize(credits, nullptr, ownerArg);
    guiRoot = self->m_pGuiRoot;
    self->field_0040 = reinterpret_cast<uint32_t>(credits);
    CSF_AT(uint32_t, credits, 0x008) = guiRoot;

    auto* extras = reinterpret_cast<CMenuExtras*>(CoreMemory_AllocUntagged(96));
    if (extras)
        extras = CMenuExtras_Construct_547CB0(extras, nullptr, ownerArg);
    guiRoot = self->m_pGuiRoot;
    self->field_002c = reinterpret_cast<uint32_t>(extras);
    CSF_AT(uint32_t, extras, 0x008) = guiRoot;

    auto* optionsPC = reinterpret_cast<CMenuOptionsPC*>(CoreMemory_AllocUntagged(56));
    if (optionsPC)
        optionsPC = CMenuOptionsPC_Construct(optionsPC, nullptr, ownerArg);
    guiRoot = self->m_pGuiRoot;
    self->field_0044 = reinterpret_cast<uint32_t>(optionsPC);
    CSF_AT(uint32_t, optionsPC, 0x008) = guiRoot;

    auto* textInput = reinterpret_cast<CMenuTextInput*>(CoreMemory_AllocUntagged(92));
    if (textInput)
        textInput = CMenuTextInput_Construct(textInput, nullptr, ownerArg);
    guiRoot = self->m_pGuiRoot;
    self->field_0050 = reinterpret_cast<uint32_t>(textInput);
    CSF_AT(uint32_t, textInput, 0x008) = guiRoot;

    auto* progressBar = reinterpret_cast<CMenuProgressBar*>(CoreMemory_AllocUntagged(40));
    if (progressBar)
        progressBar = CMenuProgressBar_Construct(progressBar, nullptr, ownerArg);
    guiRoot = self->m_pGuiRoot;
    self->field_0058 = reinterpret_cast<uint32_t>(progressBar);
    CSF_AT(uint32_t, progressBar, 0x008) = guiRoot;

    void* messageBoxStorage = reinterpret_cast<void*>(CoreMemory_AllocUntagged(2484));
    int messageBox = messageBoxStorage
        ? CMenuMessageBox_Construct(messageBoxStorage, nullptr, ownerArg)
        : 0;
    guiRoot = self->m_pGuiRoot;
    self->m_pScreenMsgBox = static_cast<uint32_t>(messageBox);
    CSF_AT(uint32_t, reinterpret_cast<void*>(messageBox), 0x008) = guiRoot;

    auto* intro = reinterpret_cast<CMenuIntroVideo*>(CoreMemory_AllocUntagged(52));
    if (intro)
        intro = CMenuIntroVideo_Construct(intro, nullptr, ownerArg);
    guiRoot = self->m_pGuiRoot;
    self->m_pScreenIntro = reinterpret_cast<uint32_t>(intro);
    CSF_AT(uint32_t, intro, 0x008) = guiRoot;

    void* additionalScreenStorage = reinterpret_cast<void*>(CoreMemory_AllocUntagged(400));
    int* additionalScreen = additionalScreenStorage
        ? C_008414e8_Construct(additionalScreenStorage, nullptr, ownerArg)
        : nullptr;
    guiRoot = self->m_pGuiRoot;
    self->field_0060 = reinterpret_cast<uint32_t>(additionalScreen);
    CSF_AT(uint32_t, additionalScreen, 0x008) = guiRoot;

    void* finalScreen = reinterpret_cast<void*>(self->field_007c);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, finalScreen, 0)[1])(finalScreen, nullptr, ownerArg);
}

HOOK(0x0050D330, CMenuManager_InitializeBaseScreens, "auto");
