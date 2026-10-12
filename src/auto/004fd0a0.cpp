#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AttachRegionToEntity, 0x004FBBE0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall C_0083f448_AttachRegionsAndReleaseMapping(
    C_0083f448* self, void* /*edx*/)
{
    for (uint32_t regionIndex = 0; regionIndex < self->field_001c; ++regionIndex)
    {
        uint32_t entityIndex =
            reinterpret_cast<uint32_t*>(self->field_00c4)[regionIndex];
        void* entity = reinterpret_cast<void**>(self->field_0004)[entityIndex];
        void* region = reinterpret_cast<void**>(self->field_0014)[regionIndex];

        AttachRegionToEntity(region, nullptr, entity);

        region = reinterpret_cast<void**>(self->field_0014)[regionIndex];
        uint32_t entryId = CSF_AT(uint32_t, region, 0x10);
        if (entryId != uint32_t(-1))
        {
            int entryIndex = -1;
            uint32_t entryCount = self->field_002c;
            if (entryCount)
            {
                void** entries = reinterpret_cast<void**>(self->field_0024);
                for (uint32_t searchIndex = 0; searchIndex < entryCount; ++searchIndex)
                {
                    if (CSF_AT(uint32_t, entries[searchIndex], 0x0C) == entryId)
                    {
                        entryIndex = static_cast<int>(searchIndex);
                        break;
                    }
                }
            }

            void* lookup = reinterpret_cast<void*>(
                ResolveEntryLookupValue(region, nullptr));
            void* entry =
                reinterpret_cast<void**>(self->field_0024)[entryIndex];
            uint32_t value = CSF_AT(uint32_t, entry, 0x00);

            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, lookup, 0x00)[0x60 / 4])(
                    lookup, nullptr, value);
        }
    }

    int result = static_cast<int>(self->field_00c4);
    if (result)
        result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_00c4));

    self->field_00c4 = 0;
    self->field_00c8 = 0;
    self->field_00cc = 0;
    return result;
}

HOOK(0x004FD0A0, C_0083f448_AttachRegionsAndReleaseMapping, "auto");
