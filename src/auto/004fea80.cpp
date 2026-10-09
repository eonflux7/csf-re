#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(unsigned int, __fastcall, SceneLoader_CompleteNavigationLoad, 0x0051A550, (void* self, void* edx));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(bool, __cdecl, MenuManager_CheckPageAvailability, 0x00698B20, (const char* name, char mode));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static char __fastcall CSceneLoader_LoadNavigationData(
    CSceneLoader* self, void* /*edx*/, const char* sceneName, char completeLoad)
{
    char success = 1;
    char path[256];

    if (CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x48))
    {
        void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        uint32_t capacity = CSF_AT(uint32_t, settings, 0x4C);
        void* nameStorage = reinterpret_cast<uint8_t*>(settings) + 52;
        const char* source =
            capacity < 0x10
                ? reinterpret_cast<const char*>(nameStorage) + 4
                : CSF_AT(const char*, nameStorage, 0x04);
        std::strcpy(path, source);
    }
    else
    {
        std::strcpy(path, sceneName);
    }

    Path_ReplaceExtension(
        path, 256, const_cast<char*>(CSF_STR(0x0083F544, ".and")));

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalInt()), 0x04) ||
        !CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xA5C))
    {
        MenuManager_CheckPageAvailability(path, 0);
    }
    else
    {
        void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));

        void* navigation = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, accessor, 0)[5])(accessor, nullptr);

        if (((bool(__fastcall*)(void*, void*, char*))
            CSF_AT(void**, navigation, 0)[10])(navigation, nullptr, path))
        {
            if (completeLoad)
            {
                void* context = reinterpret_cast<void*>(GetValue8B0554());
                SceneLoader_CompleteNavigationLoad(
                    CSF_AT(void*, context, 0x1FC4), nullptr);
            }
        }
        else
        {
            success = 0;
        }
    }

    if (CSF_AT(uint8_t, self, 0xB6) &&
        CSF_AT(void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x1C))
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        void* controller = CSF_AT(void*, state, 0x1C);
        ((void(__fastcall*)(void*, void*, float))
            CSF_AT(void**, controller, 0)[17])(controller, nullptr, 10.0f);
    }

    return success;
}

HOOK(0x004FEA80, CSceneLoader_LoadNavigationData, "auto");
