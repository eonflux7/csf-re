#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __cdecl, SceneLoader_CheckLoadResult, 0x00521DC0, ());
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(bool, __cdecl, MenuManager_CheckPageAvailability, 0x00698B20, (const char* name, char mode));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static char __fastcall CSceneLoader_LoadWorldArchive(
    C_0083f5d8* self, void* /*edx*/, const char* name, int mode)
{
    char path[256];
    char result = 1;
    const char* source;

    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    if (CSF_AT(uint32_t, settings, 0x48) != 0)
    {
        settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        uint32_t capacity = CSF_AT(uint32_t, settings, 0x4C);
        uint8_t* stringObject = reinterpret_cast<uint8_t*>(settings) + 52;
        if (capacity < 0x10)
            source = reinterpret_cast<const char*>(stringObject + 4);
        else
            source = CSF_AT(const char*, stringObject, 0x4);
    }
    else
    {
        source = name;
    }

    char* destination = path;
    do
    {
        *destination++ = *source;
    } while (*source++ != '\0');

    Path_ReplaceExtension(
        path, 256, const_cast<char*>(CSF_STR(0x0083F53C, ".wad")));

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalInt()), 0x4))
    {
        MenuManager_CheckPageAvailability(path, 0);
    }
    else
    {
        GetGlobalValue_521BB0();

        // The forwarder's final virtual callee consumes the pending arguments.
        if (!reinterpret_cast<bool (__stdcall*)(const char*, int)>(
                SceneLoader_CheckLoadResult)(path, mode))
        {
            result = 0;
        }
    }

    if (CSF_AT(uint8_t, self, 0xB6))
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        if (CSF_AT(void*, state, 0x1C))
        {
            state = reinterpret_cast<void*>(GetGlobalState_50D190());
            void* controller = CSF_AT(void*, state, 0x1C);
            reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
                CSF_AT(void**, controller, 0)[0x44 / 4])(
                    controller, nullptr, 10.0f);
        }
    }

    return result;
}

HOOK(0x004FE990, CSceneLoader_LoadWorldArchive, "auto");
