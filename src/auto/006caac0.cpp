#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(int, __cdecl, GfxScene_IsObjectProcessingEnabled, 0x00697420, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, GfxScene_ProcessWorldObjectPath, 0x006CF490, (void* self, void* edx, char* a2));
CSF_FUNC(char, __fastcall, GfxScene_ProcessCachedObjectPath, 0x006D56E0, (void* self, void* edx, char* a2));

static char __stdcall GfxScene_ProcessObjectPath(S_006d4450* objectPath)
{
    if (!objectPath)
        return 0;

    if (static_cast<std::uint8_t>(GfxScene_IsObjectProcessingEnabled()) == 0)
        return 1;

    void* globalAccessor = reinterpret_cast<void*>(GetGlobalInt());
    void* sceneLoader = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(globalAccessor, nullptr));

    char path[256];
    char* originalPath = reinterpret_cast<char*>(objectPath);
    const char* source = originalPath;
    char* destination = path;
    do
    {
        *destination++ = *source;
    } while (*source++ != '\0');

    Path_ReplaceExtension(
        path, 256, reinterpret_cast<char*>(CSF_ADDR_DATA(0x00860BAC)));

    void* collection = reinterpret_cast<void*>(
        SceneLoader_GetWorldObjectCollection(sceneLoader, nullptr));
    if (!GfxScene_ProcessWorldObjectPath(collection, nullptr, path))
        return 0;

    void* provider = GetCachedObjectProvider(sceneLoader, nullptr);
    return GfxScene_ProcessCachedObjectPath(provider, nullptr, originalPath);
}

HOOK(0x006CAAC0, GfxScene_ProcessObjectPath, "auto");
