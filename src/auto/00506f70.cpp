#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, AppMain_InitializeSubsystems, 0x004EF010, ());
CSF_FUNC(uint8_t*, __fastcall, AppMain_InitializeVideoConfiguration, 0x00506B70, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AppMain_PrepareWindowLoader, 0x00506CF0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AppMain_ConstructNetProfile, 0x00506D80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CycleVideoResolution, 0x0050B5B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AppMain_ConstructOptionsFile, 0x0050BA50, (void* self, void* edx));
CSF_FUNC(CSaveGameManager*, __fastcall, CSaveGameManager_InitializeDefaults, 0x00513BA0, (CSaveGameManager* self, void*));
CSF_FUNC(int, __fastcall, CProfileManager_InitFields, 0x00515670, (CProfileManager* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(char, __fastcall, CSoundInstance_StartSoundWithHandle, 0x00521CF0, (void* self, void*, int a2, int a3, int a4));
CSF_FUNC(void, __fastcall, AudioSoundManager_SetPositiveFlagAndForward, 0x00521D60, (void* self, void* edx, float volume));
CSF_FUNC(void, __fastcall, AudioSound_ForwardToGlobalObjectSlot29, 0x00521D90, (void* self, void* edx, int enabled));
CSF_FUNC(void*, __cdecl, CSoundManager_GetSingleton, 0x00524120, ());
CSF_FUNC(int, __cdecl, RwEngineInit, 0x00627360, (uint32_t* a1, char a2, int a3));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int32_t*, __cdecl, GetGlobal8BAD64, 0x00690DB0, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_69B700, 0x0069B700, ());
CSF_FUNC(void, __fastcall, SceneLoader_SetTextDatabasePath, 0x0069BD90, (void* self, void* edx, unsigned int a2, char* a3));
CSF_FUNC(int, __cdecl, AppMain_ReportInitializationError, 0x0069CDF0, (const char* caption, char* format, ...));
CSF_FUNC(void, __cdecl, AppMain_ConfigureWindow, 0x0069CEE0, (void* window, char mode));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AppMain_SetDisplayParameters, 0x006B1550, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __cdecl, AppMain_InitializeVideo, 0x006B1F20, (int a2, int a3));
CSF_FUNC(char, __fastcall, AppMain_SetRenderQuality, 0x006DB620, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

static char __fastcall AppMain_InitializeWindow(void* self, void* /*edx*/, void* window)
{
    alignas(8) uint8_t configurationBytes[0x74];

    AppMain_ConfigureWindow(
        window,
        CSF_AT(char, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x8FD));
    AppMain_InitializeSubsystems();

    void* textDatabase = reinterpret_cast<void*>(GetGlobalValue_69B700());
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, textDatabase, 0)[1])(
        textDatabase, nullptr);
    textDatabase = reinterpret_cast<void*>(GetGlobalValue_69B700());
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, textDatabase, 0)[2])(
        textDatabase, nullptr);

    const int engineMemory = static_cast<int>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x58) << 10);
    const bool engineInitialized = RwEngineInit(
        reinterpret_cast<uint32_t*>(GetGlobal8BAD64()), 0, engineMemory) != 0;
    if (!engineInitialized)
    {
        AppMain_ReportInitializationError(
            CSF_STR(0x0083F688, "ERROR"),
            const_cast<char*>(CSF_STR(0x0083F744, "RwEngineInit() ha fallado")));
    }

    CSF_AT(int, configurationBytes, 0x70) = 0;
    if (!engineInitialized)
    {
        CSF_AT(void*, self, 0) = window;
        return 0;
    }

    AppMain_PrepareWindowLoader(self, nullptr);

    void* options = CSF_AT(void*, self, 0x18);
    if (!options)
    {
        void* storage = Mem_Alloc(80);
        options = storage
            ? reinterpret_cast<void*>(AppMain_ConstructOptionsFile(storage, nullptr))
            : nullptr;
    }
    CSF_AT(void*, self, 0x18) = options;
    AppMain_InitializeVideoConfiguration(configurationBytes, nullptr);

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0x111))
    {
        textDatabase = reinterpret_cast<void*>(GetGlobalValue_69B700());
        SceneLoader_SetTextDatabasePath(
            textDatabase, nullptr, 0x200u,
            const_cast<char*>(CSF_STR(0x0083F734, "Texts/Demo.fli")));
    }

    CSF_AT(uint8_t, self, 0xC) = 1;
    auto* saveStorage = static_cast<CSaveGameManager*>(Mem_Alloc(176));
    CSF_AT(CSaveGameManager*, self, 4) = saveStorage
        ? CSaveGameManager_InitializeDefaults(saveStorage, nullptr)
        : nullptr;

    auto* profileStorage = static_cast<CProfileManager*>(Mem_Alloc(160));
    CSF_AT(int, self, 8) = profileStorage
        ? CProfileManager_InitFields(profileStorage, nullptr)
        : 0;

    void* netStorage = Mem_Alloc(5984);
    CSF_AT(uint32_t*, self, 0x14) = netStorage
        ? AppMain_ConstructNetProfile(netStorage, nullptr)
        : nullptr;

    options = CSF_AT(void*, self, 0x18);
    S_0050af90* videoOptions = options
        ? CSF_AT(S_0050af90*, options, 0xC)
        : nullptr;

    if (videoOptions)
    {
        void* resolution =
            ((void*(__fastcall*)(void*, void*))
                reinterpret_cast<void**>(videoOptions->field_0000)[8])(
                    videoOptions, nullptr);
        CSF_AT(int, configurationBytes, 0) = CSF_AT(int, resolution, 0);
        resolution =
            ((void*(__fastcall*)(void*, void*))
                reinterpret_cast<void**>(videoOptions->field_0000)[8])(
                    videoOptions, nullptr);
        CSF_AT(int, configurationBytes, 4) = CSF_AT(int, resolution, 4);

        CSF_AT(uint8_t, configurationBytes, 0x24) =
            static_cast<uint8_t>(((int(__fastcall*)(void*, void*))
                reinterpret_cast<void**>(videoOptions->field_0000)[14])(
                    videoOptions, nullptr));

        switch (((int(__fastcall*)(void*, void*))
            reinterpret_cast<void**>(videoOptions->field_0000)[16])(
                videoOptions, nullptr))
        {
        case 0: CSF_AT(int, configurationBytes, 0x18) = 0; break;
        case 1: CSF_AT(int, configurationBytes, 0x18) = 2; break;
        case 2: CSF_AT(int, configurationBytes, 0x18) = 4; break;
        case 3: CSF_AT(int, configurationBytes, 0x18) = 8; break;
        case 4: CSF_AT(int, configurationBytes, 0x18) = 16; break;
        default: break;
        }

        switch (((int(__fastcall*)(void*, void*))
            reinterpret_cast<void**>(videoOptions->field_0000)[18])(
                videoOptions, nullptr))
        {
        case 0: CSF_AT(int, configurationBytes, 0x70) = 4; break;
        case 1: CSF_AT(int, configurationBytes, 0x70) = 2; break;
        case 2: CSF_AT(int, configurationBytes, 0x70) = 1; break;
        case 3: CSF_AT(int, configurationBytes, 0x70) = 0; break;
        default: break;
        }

        switch (((int(__fastcall*)(void*, void*))
            reinterpret_cast<void**>(videoOptions->field_0000)[20])(
                videoOptions, nullptr))
        {
        case 0:
            CSF_AT(int, configurationBytes, 0x1C) = 0;
            CSF_AT(int, configurationBytes, 0x20) = 2;
            break;
        case 1:
            CSF_AT(int, configurationBytes, 0x1C) = 2;
            CSF_AT(int, configurationBytes, 0x20) = 2;
            break;
        case 2:
            CSF_AT(int, configurationBytes, 0x1C) = 4;
            CSF_AT(int, configurationBytes, 0x20) = 2;
            break;
        default:
            break;
        }
    }
    else
    {
        CSF_AT(int, configurationBytes, 0) = static_cast<int>(
            CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xB0));
        CSF_AT(int, configurationBytes, 4) = static_cast<int>(
            CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xB4));
        CSF_AT(int, configurationBytes, 0x18) =
            CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x11D)
            ? 2 : 0;
        CSF_AT(uint8_t, configurationBytes, 0x24) =
            CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x139);
        CSF_AT(int, configurationBytes, 0x1C) =
            CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x120);
        CSF_AT(int, configurationBytes, 0x20) =
            CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x124);
    }

    void* netProfile = CSF_AT(void*, self, 0x14);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, netProfile, 0)[1])(
        netProfile, nullptr);

    CSF_AT(int, configurationBytes, 0x10) =
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x204);
    CSF_AT(uint8_t, configurationBytes, 0x14) =
        CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x11C);
    CSF_AT(uint8_t, configurationBytes, 0x25) =
        CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x138);
    CSF_AT(const char*, configurationBytes, 0x28) =
        CSF_STR(0x0083F718, "menus\\textures\\banner2.raw");
    CSF_AT(uint8_t, configurationBytes, 0x30) =
        CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x8FC);
    CSF_AT(int, configurationBytes, 0x2C) =
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1EC);
    CSF_AT(int, configurationBytes, 0x34) =
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1F4);
    CSF_AT(int, configurationBytes, 0x38) =
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1F8);
    CSF_AT(int, configurationBytes, 0x3C) =
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1FC);
    CSF_AT(int, configurationBytes, 0x50) = 16;
    CSF_AT(int, configurationBytes, 0x40) =
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1FC);
    CSF_AT(int, configurationBytes, 0x44) =
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x148);
    CSF_AT(int, configurationBytes, 0x48) =
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x14C);
    CSF_AT(int, configurationBytes, 0x54) =
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x150);
    CSF_AT(int, configurationBytes, 0x64) =
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x154);
    CSF_AT(int, configurationBytes, 0x68) =
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x158);
    CSF_AT(int, configurationBytes, 8) = static_cast<int>(
        CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xB0));
    CSF_AT(int, configurationBytes, 0xC) = static_cast<int>(
        CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xB4));

    const char videoInitialized = AppMain_InitializeVideo(
        reinterpret_cast<int>(window), reinterpret_cast<int>(configurationBytes));
    if (CSF_AT(uint8_t, self, 0x20))
        CycleVideoResolution(videoOptions, nullptr);

    if (!videoInitialized)
    {
        CSF_AT(void*, self, 0) = window;
        return 0;
    }

    void* firstDisplaySettings = reinterpret_cast<void*>(GetGlobalValue_753A30());
    void* secondDisplaySettings = reinterpret_cast<void*>(GetGlobalValue_753A30());
    const int secondParameter = CSF_AT(int, firstDisplaySettings, 0x20C);
    const int firstParameter = CSF_AT(int, secondDisplaySettings, 0x208);
    void* engine = reinterpret_cast<void*>(GetGlobalInt());
    AppMain_SetDisplayParameters(engine, nullptr, firstParameter, secondParameter);

    const int managerId = GetGlobalInt();
    const int clumpManager = ResolveClumpManager(managerId, nullptr);
    const int renderSettings = ResolveEntryLookupValue(
        reinterpret_cast<void*>(clumpManager), nullptr);
    AppMain_SetRenderQuality(
        reinterpret_cast<void*>(renderSettings), nullptr,
        CSF_AT(int, configurationBytes, 0x70));

    const int volume =
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x54);
    CSF_AT(int, configurationBytes, 0x6C) = volume;
    const int soundParameter =
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1BC);
    options = CSF_AT(void*, self, 0x18);
    CSF_AT(uint8_t, configurationBytes, 0x70) = 0;
    if (options)
    {
        void* soundOptions = CSF_AT(void*, options, 8);
        if (soundOptions)
        {
            CSF_AT(uint8_t, configurationBytes, 0x70) =
                static_cast<uint8_t>(
                    ((int(__fastcall*)(void*, void*))
                        CSF_AT(void**, soundOptions, 0)[12])(
                            soundOptions, nullptr));
        }
    }

    CSoundManager_GetSingleton();
    const int soundEnabled = volume > 0;
    const int soundOption = CSF_AT(int, configurationBytes, 0x70);
    void* soundManager = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    const char started = CSoundInstance_StartSoundWithHandle(
        soundManager, nullptr, soundEnabled, soundOption, soundParameter);
    if (started)
    {
        const float normalizedVolume =
            static_cast<float>(CSF_AT(int, configurationBytes, 0x6C)) * g_ProgressScale;
        soundManager = reinterpret_cast<void*>(GetGlobalValue_521BB0());
        AudioSoundManager_SetPositiveFlagAndForward(
            soundManager, nullptr, normalizedVolume);
        soundManager = reinterpret_cast<void*>(GetGlobalValue_521BB0());
        AudioSound_ForwardToGlobalObjectSlot29(soundManager, nullptr, 1);

        void* menuManager = reinterpret_cast<void*>(GetGlobalInt());
        void* menuControls = MenuControls_ResolveObject(menuManager, nullptr);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, menuControls, 0)[6])(
            menuControls, nullptr);
    }

    CSF_AT(void*, self, 0) = window;
    return started ? started : 0;
}

HOOK(0x00506F70, AppMain_InitializeWindow, "auto");
