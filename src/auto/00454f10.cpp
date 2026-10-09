#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_CleanupCurrentScene, 0x0044FB50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CScene_DestroyEmbeddedCollection, 0x00453860, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, CScene_DestroyEmbeddedStorage, 0x004538E0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_UpdateCurrentScene, 0x00453CB0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, C_008343a4_Destroy, 0x004A05F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall CScene_Destroy(C_008312b0* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x008312B0);
    CSF_AT(void**, self, 0x0004) = CSF_VTBL(0x00831294);

    SceneLoader_UpdateCurrentScene(self, nullptr, 0);
    SceneLoader_CleanupCurrentScene(self, nullptr);

    void* auxiliary = CSF_AT(void*, self, 0x0188);
    if (auxiliary)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, auxiliary, 0)[0])(auxiliary, nullptr, 1);
        CSF_AT(void*, self, 0x0188) = nullptr;
    }

    if (CSF_AT(void*, self, 0x018c))
    {
        Mem_Free(CSF_AT(void*, self, 0x018c));
        CSF_AT(void*, self, 0x018c) = nullptr;
    }

    void* subsystem12c = CSF_AT(void*, self, 0x012c);
    if (subsystem12c)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, subsystem12c, 0)[0])(subsystem12c, nullptr, 1);
        CSF_AT(void*, self, 0x012c) = nullptr;
    }

    S_0041e990* subsystem130 = CSF_AT(S_0041e990*, self, 0x0130);
    if (subsystem130)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, subsystem130, 0)[0])(subsystem130, nullptr, 1);
        CSF_AT(S_0041e990*, self, 0x0130) = nullptr;
    }

    C_0083114c* subsystemC4 = CSF_AT(C_0083114c*, self, 0x00c4);
    if (subsystemC4)
    {
        subsystemC4->vSlot0(1);
        CSF_AT(C_0083114c*, self, 0x00c4) = nullptr;
    }

    C_008343a4* subsystemAC = CSF_AT(C_008343a4*, self, 0x00ac);
    if (subsystemAC)
    {
        C_008343a4_Destroy(CSF_AT(C_008343a4*, self, 0x00ac), nullptr);
        Mem_Free(subsystemAC);
        CSF_AT(C_008343a4*, self, 0x00ac) = nullptr;
    }

    if (CSF_AT(void*, self, 0x00a8))
    {
        Mem_Free(CSF_AT(void*, self, 0x00a8));
        CSF_AT(void*, self, 0x00a8) = nullptr;
    }

    C_0083109c* subsystemB8 = CSF_AT(C_0083109c*, self, 0x00b8);
    if (subsystemB8)
    {
        subsystemB8->vSlot0(1);
        CSF_AT(C_0083109c*, self, 0x00b8) = nullptr;
    }

    uint8_t* subsystemB4 = CSF_AT(uint8_t*, self, 0x00b4);
    if (subsystemB4)
    {
        void* secondaryBase = subsystemB4 + 4;
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, secondaryBase, 0)[0])(secondaryBase, nullptr, 1);
        CSF_AT(uint8_t*, self, 0x00b4) = nullptr;
    }

    C_00831120* subsystemB0 = CSF_AT(C_00831120*, self, 0x00b0);
    if (subsystemB0)
    {
        subsystemB0->vSlot0(1);
        CSF_AT(C_00831120*, self, 0x00b0) = nullptr;
    }

    void* subsystemC0 = CSF_AT(void*, self, 0x00c0);
    if (subsystemC0)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, subsystemC0, 0)[0])(subsystemC0, nullptr, 1);
        CSF_AT(void*, self, 0x00c0) = nullptr;
    }

    uint8_t* subsystemBC = CSF_AT(uint8_t*, self, 0x00bc);
    if (subsystemBC)
    {
        void* secondaryBase = subsystemBC + 4;
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, secondaryBase, 0)[0])(secondaryBase, nullptr, 1);
        CSF_AT(uint8_t*, self, 0x00bc) = nullptr;
    }

    void* subsystem184 = CSF_AT(void*, self, 0x0184);
    if (subsystem184)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, subsystem184, 0)[0])(subsystem184, nullptr, 1);
        CSF_AT(void*, self, 0x0184) = nullptr;
    }

    CSF_AT(void**, self, 0x0198) = CSF_VTBL(0x0082D410);

    if (CSF_AT(void*, self, 0x0170))
    {
        Mem_Free(CSF_AT(void*, self, 0x0170));
        CSF_AT(void*, self, 0x0170) = nullptr;
    }

    if (CSF_AT(void*, self, 0x015c))
    {
        Mem_Free(CSF_AT(void*, self, 0x015c));
        CSF_AT(void*, self, 0x015c) = nullptr;
    }

    uint8_t* entries = CSF_AT(uint8_t*, self, 0x0148);
    if (entries)
    {
        const int32_t count = CSF_AT(int32_t, entries - 4, 0);
        void* allocation = entries - 4;
        const uint32_t entryDwords = 5u * static_cast<uint32_t>(count);
        const int32_t lastIndex =
            static_cast<int32_t>(static_cast<uint32_t>(count) - 1u);
        uint8_t* end = entries + 8u * entryDwords;

        if (lastIndex >= 0)
        {
            uint32_t* cursor = reinterpret_cast<uint32_t*>(end + 8);
            uint32_t remaining = static_cast<uint32_t>(lastIndex) + 1u;
            do
            {
                cursor -= 10;
                --remaining;
                CSF_AT(void**, cursor, 0) = CSF_VTBL(0x0082D410);
            }
            while (remaining);
        }

        Mem_Free(allocation);
        CSF_AT(uint8_t*, self, 0x0148) = nullptr;
    }

    if (CSF_AT(void*, self, 0x0134))
    {
        Mem_Free(CSF_AT(void*, self, 0x0134));
        CSF_AT(void*, self, 0x0134) = nullptr;
    }

    CScene_DestroyEmbeddedStorage(reinterpret_cast<uint8_t*>(self) + 0x010c, nullptr);

    if (CSF_AT(void*, self, 0x0100))
        Mem_FreeDebug(CSF_AT(void*, self, 0x0100));
    CSF_AT(uint32_t, self, 0x0100) = 0;
    CSF_AT(uint32_t, self, 0x0104) = 0;
    CSF_AT(uint32_t, self, 0x0108) = 0;

    if (CSF_AT(void*, self, 0x00e8))
    {
        Mem_Free(CSF_AT(void*, self, 0x00e8));
        CSF_AT(void*, self, 0x00e8) = nullptr;
    }

    if (CSF_AT(void*, self, 0x00d8))
    {
        Mem_Free(CSF_AT(void*, self, 0x00d8));
        CSF_AT(void*, self, 0x00d8) = nullptr;
    }

    CScene_DestroyEmbeddedCollection(reinterpret_cast<uint8_t*>(self) + 0x00c8, nullptr);

    if (CSF_AT(void*, self, 0x0098))
    {
        Mem_Free(CSF_AT(void*, self, 0x0098));
        CSF_AT(void*, self, 0x0098) = nullptr;
    }

    if (CSF_AT(void*, self, 0x0088))
    {
        Mem_Free(CSF_AT(void*, self, 0x0088));
        CSF_AT(void*, self, 0x0088) = nullptr;
    }

    if (CSF_AT(void*, self, 0x0074))
    {
        Mem_Free(CSF_AT(void*, self, 0x0074));
        CSF_AT(void*, self, 0x0074) = nullptr;
    }

    if (CSF_AT(void*, self, 0x0060))
    {
        Mem_Free(CSF_AT(void*, self, 0x0060));
        CSF_AT(void*, self, 0x0060) = nullptr;
    }

    if (CSF_AT(void*, self, 0x0050))
    {
        Mem_Free(CSF_AT(void*, self, 0x0050));
        CSF_AT(void*, self, 0x0050) = nullptr;
    }

    if (CSF_AT(void*, self, 0x0040))
    {
        Mem_Free(CSF_AT(void*, self, 0x0040));
        CSF_AT(void*, self, 0x0040) = nullptr;
    }

    if (CSF_AT(void*, self, 0x0030))
    {
        Mem_Free(CSF_AT(void*, self, 0x0030));
        CSF_AT(void*, self, 0x0030) = nullptr;
    }

    if (CSF_AT(void*, self, 0x0020))
    {
        Mem_Free(CSF_AT(void*, self, 0x0020));
        CSF_AT(void*, self, 0x0020) = nullptr;
    }

    int result = CSF_AT(int, self, 0x0010);
    if (result)
    {
        result = Mem_Free(CSF_AT(void*, self, 0x0010));
        CSF_AT(void*, self, 0x0010) = nullptr;
    }

    CSF_AT(void**, self, 0x0004) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x00454F10, CScene_Destroy, "auto");
