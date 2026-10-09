#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, SceneLoader_LoadCompiledSceneScript, 0x00444890,
         (void* self, void* edx, void* a1));
CSF_FUNC(char, __fastcall, SceneLoader_LoadGameSceneScript, 0x0046BC40,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0,
         (char* Str, int a2, char* a3));

static char __fastcall CSceneLoader_LoadSceneScripts(
    CSceneLoader* self, void* /*edx*/, char* filename)
{
    char path[256] = {};

    const char* source = filename;
    char* destination = path;
    do {
        *destination++ = *source;
    } while (*source++);

    Path_ReplaceExtension(
        path, 256, const_cast<char*>(CSF_STR(0x0083F51C, ".gsc")));
    const char loaded = SceneLoader_LoadGameSceneScript(
        CSF_AT(void*, g_pCurrentScene, 0xBC), nullptr,
        reinterpret_cast<int>(path));

    if (CSF_AT(char, self, 0xB6)) {
        if (CSF_AT(void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x1C)) {
            void* object = CSF_AT(
                void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x1C);
            reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
                CSF_AT(void**, object, 0)[0x44 / 4])(object, nullptr, 10.0f);
        }
    }

    if (!loaded)
        return 0;

    source = filename;
    destination = path;
    do {
        *destination++ = *source;
    } while (*source++);

    Path_ReplaceExtension(
        path, 256, const_cast<char*>(CSF_STR(0x0083F514, ".csc")));
    SceneLoader_LoadCompiledSceneScript(
        CSF_AT(void*, g_pCurrentScene, 0xC0), nullptr, path);

    if (CSF_AT(char, self, 0xB6)) {
        if (CSF_AT(void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x1C)) {
            void* object = CSF_AT(
                void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x1C);
            reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
                CSF_AT(void**, object, 0)[0x44 / 4])(object, nullptr, 10.0f);
        }
    }

    return loaded;
}

HOOK(0x004FE010, CSceneLoader_LoadSceneScripts, "auto");
