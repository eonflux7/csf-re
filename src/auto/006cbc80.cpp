#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, GfxScene_TestSpatialEntry, 0x006CB260,
         (void* self, void* edx, float* a2, float* a3));

static char __fastcall S_006cbc80_TestSpatialMatch(
    S_006cbc80* self, void* /*edx*/,
    float* a2, float* a3, int cacheAddress)
{
    void* cache = self->field_0094
        ? reinterpret_cast<void*>(cacheAddress)
        : nullptr;

    if (cache)
    {
        const int32_t cachedIndex = CSF_AT(int32_t, cache, 0);
        void* cachedEntry = nullptr;

        if (cachedIndex >= 0 &&
            cachedIndex < static_cast<int32_t>(self->field_007c))
        {
            void* slot = reinterpret_cast<void*>(
                self->field_0074 + 8u * static_cast<uint32_t>(cachedIndex));
            if (CSF_AT(uint8_t, slot, 4))
                cachedEntry = CSF_AT(void*, slot, 0);
        }

        if (cachedEntry)
        {
            if (GfxScene_TestSpatialEntry(cachedEntry, nullptr, a2, a3))
            {
                CSF_AT(uint32_t, cache, 4) = self->field_00f4;
                return 1;
            }
            CSF_AT(int32_t, cache, 0) = -1;
        }
        else if (self->field_00f4 - CSF_AT(uint32_t, cache, 4) <= 0x28)
        {
            return 0;
        }
    }

    uint32_t index = 0;
    if (!CSF_AT(uint32_t, self, 0x8C))
        return 0;

    uint8_t* entry = CSF_AT(uint8_t*, self, 0x84);
    while (!GfxScene_TestSpatialEntry(
        CSF_AT(void*, entry, 0), nullptr, a2, a3))
    {
        ++index;
        entry += 8;
        if (index >= CSF_AT(uint32_t, self, 0x8C))
            return 0;
    }

    if (cache)
    {
        CSF_AT(uint32_t, cache, 4) = self->field_00f4;
        uint8_t* entries = CSF_AT(uint8_t*, self, 0x84);
        CSF_AT(uint32_t, cache, 0) =
            CSF_AT(uint32_t, entries + 8u * index, 4);
    }
    return 1;
}

HOOK(0x006CBC80, S_006cbc80_TestSpatialMatch, "auto");
