#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneStreaming_InsertClumpFile, 0x006CDFB0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneStreaming_CanLoadPath, 0x006D4520, (void* self, void* edx, char* a2));

// Loads a world clump from a path: resolves the scene accessor and provider,
// checks that the path is loadable, then rebuilds the path with the fixed
// extension and inserts it into the world object collection.
static char __stdcall GfxScene_AddWorldClumpFromPath(const char* path)
{
    if (!path)
        return 0;

    auto* globalInt = reinterpret_cast<C_00860130*>(GetGlobalInt());
    auto* accessor = reinterpret_cast<C_0085e7b8*>(GlobalInt_ResolveAccessor(globalInt, nullptr));
    void* provider = GetCachedObjectProvider(accessor, nullptr);

    char result = SceneStreaming_CanLoadPath(provider, nullptr, const_cast<char*>(path));
    if (result)
    {
        char pathBuffer[256];
        size_t i = 0;
        for (;; ++i)
        {
            pathBuffer[i] = path[i];
            if (path[i] == '\0')
                break;
        }

        Path_ReplaceExtension(pathBuffer, 256, reinterpret_cast<char*>(CSF_ADDR_DATA(0x860BAC)));
        int worldCollection = SceneLoader_GetWorldObjectCollection(accessor, nullptr);
        return SceneStreaming_InsertClumpFile(reinterpret_cast<void*>(worldCollection), nullptr,
                                              reinterpret_cast<int>(pathBuffer));
    }
    return result;
}

HOOK(0x6CAA30, GfxScene_AddWorldClumpFromPath, "auto");
