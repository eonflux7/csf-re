#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pMenuManager, 0x008B0570);

CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(unsigned int, __fastcall, GameBddLoader_LoadResourceRange, 0x0043E830,
         (void* self, void* edx, unsigned int a2, unsigned int a3));
CSF_FUNC(void*, __cdecl, GameBddLoader_PrepareResources, 0x00441910, ());
CSF_FUNC(char, __fastcall, GameBddLoader_LoadResources, 0x00441940,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GameBddLoader_GetSceneLoader, 0x00506CD0, ());
CSF_FUNC(char, __fastcall, GameBddLoader_InitializeWindow, 0x00506F70,
         (void* self, void* edx, int window));
CSF_FUNC(int, __fastcall, GameBddLoader_AttachMenuManager, 0x0050A0A0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GameBddLoader_InitializeSceneResources, 0x0050BBB0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuSystem_InitCommonMenuBuilder, 0x0050D270,
         (void* self, void*));
CSF_FUNC(int, __fastcall, GameBddLoader_FinalizeMenuResources, 0x0050E070,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, GameBddLoader_InitializeLoadingScreen, 0x0050E330,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, GameBddLoader_UpdateLoadingScreen, 0x0050E430,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, GameBddLoader_InitializeMenuResources, 0x0050EB10,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GameBddLoader_ConfigureHud, 0x0050F060, (int a1));
CSF_FUNC(unsigned int, __fastcall, GameBddLoader_InitializeMenuState, 0x0050F980,
         (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GameBddLoader_ConstructMenuManager, 0x005113E0,
         (void* self, void* edx, int a2, int a3, char* String1));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, SetGlobalValue8BAD60, 0x006910A0, (int32_t value));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, SetGlobalValue8BB3F8, 0x006973B0, (int value));
CSF_FUNC(char, __cdecl, SceneLoader_ResetArchiveSelection, 0x006980D0,
         (char* String1));
CSF_FUNC(void, __cdecl, SceneLoader_SelectArchive, 0x00698960,
         (char* String1));
CSF_FUNC(int*, __cdecl, GetGlobalIntPtr, 0x0069C580, ());
CSF_FUNC(int, __cdecl, GameBddLoader_ShouldPreloadResources, 0x0069D5C0,
         (int a1));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390,
         (void* self, void* edx));
CSF_FUNC(bool, __cdecl, GameBddLoader_PrepareGraphics, 0x0073F630, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(void*, __cdecl, InputBindings_GetSingleton, 0x007702B0, ());

static void GameBddLoader_ReportProgress(int percent)
{
    void* loadingObject =
        LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, loadingObject, 0)[80 / 4])(
            loadingObject, nullptr, percent);

    GameBddLoader_UpdateLoadingScreen(g_pMenuManager, nullptr);

    loadingObject =
        LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, loadingObject, 0)[44 / 4])(
            loadingObject, nullptr);
}

static char __cdecl GameBddLoader_CreateMenuManager(int window, char* archiveName)
{
    char success = 1;
    if (!g_pMenuManager)
    {
        GameBddLoader_PrepareGraphics();
        GetGlobalValue_8B056C();
        void* sceneLoader =
            reinterpret_cast<void*>(GameBddLoader_GetSceneLoader());

        success = ((char(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, sceneLoader, 0)[4 / 4])(
                sceneLoader, nullptr, window, 1);

        if (CSF_AT(uint8_t,
                   reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xA5C))
        {
            SceneLoader_SelectArchive(
                reinterpret_cast<char*>(GetGlobalIntPtr()));
        }

        if (success)
        {
            success = GameBddLoader_InitializeWindow(
                reinterpret_cast<void*>(GetGlobalValue_8B056C()),
                nullptr, window);
        }

        if (success)
        {
            void* storage = Mem_Alloc(324);
            g_pMenuManager = storage
                ? GameBddLoader_ConstructMenuManager(
                    storage, nullptr, reinterpret_cast<int>(sceneLoader),
                    window, archiveName)
                : nullptr;

            void* menu = g_pMenuManager;
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, menu, 0)[152 / 4])(menu, nullptr);

            GameBddLoader_AttachMenuManager(
                CSF_AT(void*, g_pMenuManager, 0x7C), nullptr,
                reinterpret_cast<int>(g_pMenuManager));
            // The supplied binding has an unused stack argument.
            GameBddLoader_ConfigureHud(0);
            GameBddLoader_InitializeLoadingScreen(g_pMenuManager, nullptr);
            GameBddLoader_ReportProgress(25);

            void* controls = MenuControls_ResolveObject(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr);
            int controlMode = CSF_AT(uint8_t,
                reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x2244);
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, controls, 0)[188 / 4])(
                    controls, nullptr, controlMode);

            GameBddLoader_InitializeMenuResources(g_pMenuManager, nullptr);
            MenuSystem_InitCommonMenuBuilder(g_pMenuManager, nullptr);
            GameBddLoader_InitializeMenuState(g_pMenuManager, nullptr);
            InputBindings_GetSingleton();
            GameBddLoader_PrepareResources();
            success = GameBddLoader_LoadResources(
                reinterpret_cast<void*>(GetGlobal8afc7c()), nullptr);

            menu = g_pMenuManager;
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, menu, 0)[116 / 4])(menu, nullptr, window);
            menu = g_pMenuManager;
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, menu, 0)[120 / 4])(menu, nullptr);

            GameBddLoader_ReportProgress(50);
            GameBddLoader_FinalizeMenuResources(g_pMenuManager, nullptr);

            int resourceValue = CSF_AT(int,
                reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x98);
            CSF_AT(int, g_pMenuManager, 0x84) = resourceValue;

            if (success)
            {
                void* sceneResources = CSF_AT(void*,
                    reinterpret_cast<void*>(GetGlobalValue_8B056C()), 0x18);
                if (sceneResources)
                    GameBddLoader_InitializeSceneResources(
                        sceneResources, nullptr);

                GameBddLoader_ReportProgress(75);

                menu = g_pMenuManager;
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, menu, 0)[24 / 4])(menu, nullptr, 0);

                SetGlobalValue8BB3F8(
                    static_cast<int>(CSF_ADDR_FN(0x0050F920)));

                GameBddLoader_ReportProgress(100);

                if (static_cast<uint8_t>(
                        GameBddLoader_ShouldPreloadResources(1)))
                {
                    unsigned int firstResource = 0;
                    for (int percent = 25; percent < 125; percent += 25)
                    {
                        GameBddLoader_LoadResourceRange(
                            reinterpret_cast<void*>(GetGlobal8afc7c()),
                            nullptr, firstResource, firstResource + 3);
                        GameBddLoader_ReportProgress(percent);
                        firstResource += 4;
                    }
                }

                menu = g_pMenuManager;
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, menu, 0)[32 / 4])(menu, nullptr, 0);
            }

            void* loadingObject = LookupObjectById(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr);
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, loadingObject, 0)[84 / 4])(
                    loadingObject, nullptr);

            void* temporaryObject = CSF_AT(void*, g_pMenuManager, 0x104);
            if (temporaryObject)
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, temporaryObject, 0)[0])(
                        temporaryObject, nullptr, 1);
                CSF_AT(void*, g_pMenuManager, 0x104) = nullptr;
            }
        }
        else
        {
            if (CSF_AT(uint8_t,
                       reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xA5C))
            {
                SceneLoader_ResetArchiveSelection(
                    reinterpret_cast<char*>(GetGlobalIntPtr()));
            }

            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, sceneLoader, 0)[0])(sceneLoader, nullptr, 1);

            void* loadingObject = LookupObjectById(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr);
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, loadingObject, 0)[84 / 4])(
                    loadingObject, nullptr);
        }
    }

    SetGlobalValue8BAD60(reinterpret_cast<int32_t>(g_pMenuManager));
    return success;
}

HOOK(0x0050FBD0, GameBddLoader_CreateMenuManager, "auto");
