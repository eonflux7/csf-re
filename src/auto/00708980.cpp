#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpClumpStreamWrite, 0x0058FDF0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSectorCount, 0x006D4190, (S_006d4450* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_GetSectorEntry, 0x006D4720, (S_006d4450* self, void* , int sectorIndex));

static char __stdcall GfxAnimInterpolator_WriteSectorClumpsWithExtension(char* extension)
{
    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(globalObject, nullptr));
    auto* provider = static_cast<S_006d4450*>(
        GetCachedObjectProvider(accessor, nullptr));

    if (!provider)
        return 0;

    const uint32_t sectorCount = static_cast<uint32_t>(
        SceneLoader_GetSectorCount(provider, nullptr));

    alignas(8) uint8_t pathBytes[0x208];
    char* path = reinterpret_cast<char*>(pathBytes);

    for (uint32_t sectorIndex = 0; sectorIndex < sectorCount; ++sectorIndex)
    {
        void* entry = reinterpret_cast<void*>(
            SceneLoader_GetSectorEntry(
                provider, nullptr, static_cast<int>(sectorIndex)));

        if (!CSF_AT(uint32_t, entry, 0x1C))
            continue;

        const int clump = CSF_AT(int, CSF_AT(void*, entry, 0x20), 0);
        const char* sourcePath;
        if (CSF_AT(uint32_t, entry, 0x18) < 0x10u)
            sourcePath = reinterpret_cast<const char*>(entry) + 4;
        else
            sourcePath = CSF_AT(const char*, entry, 0x04);

        std::strcpy(path, sourcePath);
        Path_ReplaceExtension(path, 512, extension);

        uint32_t* stream = RwStreamOpen(2, 2, reinterpret_cast<int>(path));
        if (!RpClumpStreamWrite(clump, reinterpret_cast<int>(stream)))
            return 0;

        RwStreamClose(stream, nullptr);
    }

    return 1;
}

HOOK(0x00708980, GfxAnimInterpolator_WriteSectorClumpsWithExtension, "auto");
