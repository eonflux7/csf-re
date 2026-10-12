#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobalValue_69B700, 0x0069B700, ());
CSF_FUNC(void, __fastcall, SceneLoader_SetTextDatabasePath, 0x0069BD90, (void* self, void* edx, unsigned int a2, char* a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

static void __fastcall C_0085ec18_InitializeConsoleResources(
    C_0085ec18* self, void* /*edx*/)
{
    void* controls = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    CSF_AT(uint32_t, self, 0x18C) =
        ((uint32_t(__fastcall*)(void*, void*))
            CSF_AT(void**, controls, 0)[0x48 / 4])(controls, nullptr);

    controls = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* font =
        ((void*(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, controls, 0)[0x50 / 4])(
                controls, nullptr,
                CSF_STR(0x008343B0, "menus\\Ingame_01.fnt"));
    CSF_AT(uint32_t, self, 0x190) = reinterpret_cast<uint32_t>(font);

    float color[4] = {255.0f, 0.0f, 0.0f, 255.0f};
    ((void(__fastcall*)(void*, void*, float*))
        CSF_AT(void**, font, 0)[0x60 / 4])(font, nullptr, color);

    void* textDatabase = reinterpret_cast<void*>(GetGlobalValue_69B700());
    SceneLoader_SetTextDatabasePath(
        textDatabase, nullptr, 0x10u,
        const_cast<char*>(CSF_STR(0x0083F26C, "texts/console.fli")));
}

HOOK(0x004FA6C0, C_0085ec18_InitializeConsoleResources, "auto");
