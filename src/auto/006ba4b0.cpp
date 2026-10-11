#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, GfxObjectProps_ClearPrimaryCollection, 0x006B9620,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxObjectProps_ClearSecondCollection, 0x006B9C10,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxObjectProps_ClearThirdCollection, 0x006B9D80,
         (void* self, void* edx));

static int __cdecl GfxObjectProps_DestroyEnginePropertyCollections(
    int object, int propertyOffset)
{
    auto* properties =
        reinterpret_cast<std::uint8_t*>(g_pRwEngineInstance) + propertyOffset;

    void* primary = CSF_AT(void*, properties, 0x0);
    void* second = CSF_AT(void*, properties, 0x4);
    void* third = CSF_AT(void*, properties, 0x8);

    if (second)
    {
        GfxObjectProps_ClearSecondCollection(second, nullptr);
        void* storage = CSF_AT(void*, second, 0x4);
        if (storage)
            Mem_FreeDebug(storage);
        CSF_AT(std::uint32_t, second, 0x4) = 0;
        CSF_AT(std::uint32_t, second, 0x8) = 0;
        CSF_AT(std::uint32_t, second, 0xC) = 0;
        Mem_Free(second);
    }

    if (third)
    {
        GfxObjectProps_ClearThirdCollection(third, nullptr);
        void* storage = CSF_AT(void*, third, 0x4);
        if (storage)
            Mem_FreeDebug(storage);
        CSF_AT(std::uint32_t, third, 0x4) = 0;
        CSF_AT(std::uint32_t, third, 0x8) = 0;
        CSF_AT(std::uint32_t, third, 0xC) = 0;
        Mem_Free(third);
    }

    if (primary)
    {
        GfxObjectProps_ClearPrimaryCollection(primary, nullptr);
        void* storage = CSF_AT(void*, primary, 0x4);
        if (storage)
            Mem_FreeDebug(storage);
        CSF_AT(std::uint32_t, primary, 0x4) = 0;
        CSF_AT(std::uint32_t, primary, 0x8) = 0;
        CSF_AT(std::uint32_t, primary, 0xC) = 0;
        Mem_Free(primary);
    }

    return object;
}

HOOK(0x006BA4B0, GfxObjectProps_DestroyEnginePropertyCollections, "auto");
