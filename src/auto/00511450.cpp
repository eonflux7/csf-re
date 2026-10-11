#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __fastcall, C_008407a8_InitializeBase, 0x0050D330, (void* self, void* edx, int a2));
CSF_FUNC(CMenuTextInput*, __fastcall, CMenuTextInput_Construct, 0x0052C260, (CMenuTextInput* self, void* , int ownerArg));
CSF_FUNC(CMenuScenarioSelect*, __fastcall, CMenuScenarioSelect_Construct, 0x0052E3B0, (CMenuScenarioSelect* self, void* , int context));
CSF_FUNC(CMenuSceneSelect*, __fastcall, CMenuSceneSelect_Construct, 0x0052FDA0, (CMenuSceneSelect* self, void* , int argument));
CSF_FUNC(int, __fastcall, GameBddLoader_ConstructLoadingScreen, 0x00538BE0, (void* self, void* edx, int a2));
CSF_FUNC(CMenuLoadGameInGame*, __fastcall, CMenuLoadGameInGame_Construct, 0x00539C00, (CMenuLoadGameInGame* self, void* , int a2));
CSF_FUNC(CMenuLoadGameMainMenu*, __fastcall, CMenuLoadGameMainMenu_Construct, 0x00539C70, (CMenuLoadGameMainMenu* self, void* , int a2));
CSF_FUNC(CMenuPause*, __fastcall, CMenuPause_Construct, 0x0053A500, (CMenuPause* self, void* , int restoreArg));
CSF_FUNC(CMenuDemoExit*, __fastcall, CMenuDemoExit_Construct, 0x0053C5E0, (CMenuDemoExit* self, void* , int parentOwner));
CSF_FUNC(CMenuSoundOptionsInGame*, __fastcall, CMenuSoundOptionsInGame_Construct, 0x0053D1F0, (CMenuSoundOptionsInGame* self, void* , int a2));
CSF_FUNC(CMenuControlsInGame*, __fastcall, CMenuControlsInGame_Construct, 0x00542AA0, (CMenuControlsInGame* self, void* , int a2));
CSF_FUNC(CMenuControls*, __fastcall, CMenuControls_Construct, 0x00543150, (CMenuControls* self, void* , int configuration));
CSF_FUNC(void*, __fastcall, C_00843720_Construct, 0x00543810, (void* self, void* , int a2, int a3));
CSF_FUNC(CMenuMainPC*, __fastcall, CMenuMainPC_Construct, 0x00543C40, (CMenuMainPC* self, void* , int a2));
CSF_FUNC(CMenuGameOptionsInGamePC*, __fastcall, CMenuGameOptionsInGamePC_Construct, 0x00543F50, (CMenuGameOptionsInGamePC* self, void* , int a2));
CSF_FUNC(void*, __fastcall, CMenuGameOptionsPC_Construct, 0x00543FC0, (void* self, void* , int a2));
CSF_FUNC(CMenuProfilesPC*, __fastcall, CMenuProfilesPC_Constructor, 0x00544670, (CMenuProfilesPC* self, void* , int a2));
CSF_FUNC(CMenuSaveGame*, __fastcall, CMenuSaveGame_Construct, 0x00545530, (CMenuSaveGame* self, void* , int ownerContext));
CSF_FUNC(CMenuSoundOptionsPC*, __fastcall, CMenuSoundOptionsPC_Construct, 0x00548920, (CMenuSoundOptionsPC* self, void* , int a2));
CSF_FUNC(C_00843ed0*, __fastcall, CMenuExtras_Construct_548BD0, 0x00548BD0, (C_00843ed0* self, void* , int a2));
CSF_FUNC(int*, __fastcall, CMenuVideoAdvanced_Construct, 0x00549450, (CMenuVideoAdvanced* self, void* , int argument));
CSF_FUNC(CMenuVideoOptionsInGame*, __fastcall, CMenuVideoOptionsInGame_Construct, 0x005494D0, (CMenuVideoOptionsInGame* self, void* , int a2));
CSF_FUNC(CMenuVideoOptions*, __fastcall, CMenuVideoOptions_Construct, 0x00549F00, (CMenuVideoOptions* self, void* , int argument));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

static void C_008407a8_AttachScreen(
    C_008407a8* self, uint32_t& destination, void* screen)
{
    const uint32_t guiRoot = self->m_pGuiRoot;
    destination = reinterpret_cast<uint32_t>(screen);
    // The original writes this even when allocation returned null.
    reinterpret_cast<CMenuManager*>(screen)->field_0008 = guiRoot;
}

static int __fastcall C_008407a8_InitializeScreens(
    C_008407a8* self, void* /*edx*/, int context)
{
    C_008407a8_InitializeBase(self, nullptr, context);

    auto* loadMain = static_cast<CMenuLoadGameMainMenu*>(Mem_Alloc(292));
    C_008407a8_AttachScreen(self, self->field_0114,
        loadMain ? CMenuLoadGameMainMenu_Construct(loadMain, nullptr, context) : nullptr);

    auto* loadInGame = static_cast<CMenuLoadGameInGame*>(Mem_Alloc(292));
    C_008407a8_AttachScreen(self, self->field_0110,
        loadInGame ? CMenuLoadGameInGame_Construct(loadInGame, nullptr, context) : nullptr);

    auto* sceneSelect = static_cast<CMenuSceneSelect*>(Mem_Alloc(84));
    C_008407a8_AttachScreen(self, self->field_011c,
        sceneSelect ? CMenuSceneSelect_Construct(sceneSelect, nullptr, context) : nullptr);

    void* gameStorage = Mem_Alloc(640);
    void* gameScreen = nullptr;
    if (gameStorage)
    {
        void* gameState = reinterpret_cast<void*>(GetGlobalValue_8B056C());
        gameScreen = C_00843720_Construct(
            gameStorage, nullptr, context, CSF_AT(int, gameState, 0x1C));
    }
    C_008407a8_AttachScreen(self, self->m_pScreenGame, gameScreen);

    void* loadingStorage = Mem_Alloc(284);
    void* loadingScreen = loadingStorage
        ? reinterpret_cast<void*>(GameBddLoader_ConstructLoadingScreen(
            loadingStorage, nullptr, context))
        : nullptr;
    C_008407a8_AttachScreen(self, self->m_pScreenLoading, loadingScreen);

    auto* pause = static_cast<CMenuPause*>(Mem_Alloc(72));
    C_008407a8_AttachScreen(self, self->m_pScreenGameMenu,
        pause ? CMenuPause_Construct(pause, nullptr, context) : nullptr);

    auto* saveGame = static_cast<CMenuSaveGame*>(Mem_Alloc(144));
    C_008407a8_AttachScreen(self, self->field_0118,
        saveGame ? CMenuSaveGame_Construct(saveGame, nullptr, context) : nullptr);

    auto* profiles = static_cast<CMenuProfilesPC*>(Mem_Alloc(120));
    C_008407a8_AttachScreen(self, self->m_pScreenProfiles,
        profiles ? CMenuProfilesPC_Constructor(profiles, nullptr, context) : nullptr);

    auto* scenarioSelect = static_cast<CMenuScenarioSelect*>(Mem_Alloc(152));
    C_008407a8_AttachScreen(self, self->field_0028,
        scenarioSelect ? CMenuScenarioSelect_Construct(scenarioSelect, nullptr, context) : nullptr);

    auto* mainMenu = static_cast<CMenuMainPC*>(Mem_Alloc(64));
    C_008407a8_AttachScreen(self, self->m_pScreenMainMenu,
        mainMenu ? CMenuMainPC_Construct(mainMenu, nullptr, context) : nullptr);

    auto* textInput = static_cast<CMenuTextInput*>(Mem_Alloc(92));
    C_008407a8_AttachScreen(self, self->field_0034,
        textInput ? CMenuTextInput_Construct(textInput, nullptr, context) : nullptr);

    auto* extras = static_cast<C_00843ed0*>(Mem_Alloc(180));
    C_008407a8_AttachScreen(self, self->m_pScreenTactic,
        extras ? CMenuExtras_Construct_548BD0(extras, nullptr, context) : nullptr);

    auto* soundOptions = static_cast<CMenuSoundOptionsPC*>(Mem_Alloc(108));
    C_008407a8_AttachScreen(self, self->field_0120,
        soundOptions ? CMenuSoundOptionsPC_Construct(soundOptions, nullptr, context) : nullptr);

    auto* soundInGame = static_cast<CMenuSoundOptionsInGame*>(Mem_Alloc(92));
    C_008407a8_AttachScreen(self, self->field_0124,
        soundInGame ? CMenuSoundOptionsInGame_Construct(soundInGame, nullptr, context) : nullptr);

    auto* videoOptions = static_cast<CMenuVideoOptions*>(Mem_Alloc(128));
    C_008407a8_AttachScreen(self, self->field_0128,
        videoOptions ? CMenuVideoOptions_Construct(videoOptions, nullptr, context) : nullptr);

    auto* videoInGame = static_cast<CMenuVideoOptionsInGame*>(Mem_Alloc(128));
    C_008407a8_AttachScreen(self, self->field_012c,
        videoInGame ? CMenuVideoOptionsInGame_Construct(videoInGame, nullptr, context) : nullptr);

    auto* controls = static_cast<CMenuControls*>(Mem_Alloc(736));
    C_008407a8_AttachScreen(self, self->field_0130,
        controls ? CMenuControls_Construct(controls, nullptr, context) : nullptr);

    auto* advancedVideo = static_cast<CMenuVideoAdvanced*>(Mem_Alloc(84));
    C_008407a8_AttachScreen(self, self->field_0134,
        advancedVideo ? CMenuVideoAdvanced_Construct(advancedVideo, nullptr, context) : nullptr);

    void* gameOptions = Mem_Alloc(100);
    C_008407a8_AttachScreen(self, self->m_pScreenGameOptions,
        gameOptions ? CMenuGameOptionsPC_Construct(gameOptions, nullptr, context) : nullptr);

    auto* gameOptionsInGame = static_cast<CMenuGameOptionsInGamePC*>(Mem_Alloc(100));
    C_008407a8_AttachScreen(self, self->m_pScreenGameOptionsInGame,
        gameOptionsInGame
            ? CMenuGameOptionsInGamePC_Construct(gameOptionsInGame, nullptr, context)
            : nullptr);

    auto* controlsInGame = static_cast<CMenuControlsInGame*>(Mem_Alloc(736));
    C_008407a8_AttachScreen(self, self->field_0138,
        controlsInGame ? CMenuControlsInGame_Construct(controlsInGame, nullptr, context) : nullptr);

    int result =
        CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xD3) ||
        CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0x110) ||
        CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0x112);
    self->field_0140 = static_cast<uint8_t>(result);

    if (self->field_0140)
    {
        auto* demoExitStorage = static_cast<CMenuDemoExit*>(Mem_Alloc(36));
        CMenuDemoExit* demoExit = demoExitStorage
            ? CMenuDemoExit_Construct(demoExitStorage, nullptr, context)
            : nullptr;
        result = reinterpret_cast<int>(demoExit);
        C_008407a8_AttachScreen(self, self->field_0064, demoExit);
    }

    return result;
}

HOOK(0x00511450, C_008407a8_InitializeScreens, "auto");
