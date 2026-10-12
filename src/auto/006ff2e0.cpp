#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, C_00863044_FindObjectEntry, 0x006FE4F0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_00863044_RemoveObjectEntry, 0x006FE540, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, C_00863044_AddObjectEntry, 0x006FE940, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00863044_ApplyObjectEntryData, 0x006FEA00, (void* self, void* edx, void** a2, uint32_t* a3, int a4));
CSF_FUNC(int, __fastcall, C_00863044_CollectObjectEntryData, 0x006FF080, (void* self, void* edx, uint32_t* a2, uint32_t* a3));

static void __fastcall C_00863044_UpdateObjectEntry(
    C_00863044* self, void* /*edx*/, uint32_t* object, int updateArgument)
{
    uint32_t entryIndex = C_00863044_FindObjectEntry(
        self, nullptr, reinterpret_cast<int>(object));

    alignas(8) uint8_t queryBytes[0x8];
    void* queryResult =
        reinterpret_cast<void* (__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, object, 0)[0x3C / 4])(
                object, nullptr, queryBytes);

    if (CSF_AT(float, queryResult, 0x10) < CSF_AT(float, self, 0x1C))
    {
        if (entryIndex == static_cast<uint32_t>(-1))
            entryIndex = C_00863044_AddObjectEntry(self, nullptr, object);

        uint32_t* entry =
            reinterpret_cast<uint32_t*>(self->field_0024 + 12 * entryIndex);

        alignas(8) uint8_t entryDataBytes[0x10];
        uint32_t* entryData = reinterpret_cast<uint32_t*>(entryDataBytes);
        std::memset(entryDataBytes, 0, 12);
        entryData[3] = 10;

        if (static_cast<uint8_t>(
                C_00863044_CollectObjectEntryData(
                    self, nullptr, entry, entryData)) != 0)
        {
            C_00863044_ApplyObjectEntryData(
                self, nullptr, reinterpret_cast<void**>(entry),
                entryData, updateArgument);
        }

        if (entryData[0] != 0)
            Mem_Free(reinterpret_cast<void*>(entryData[0]));
    }
    else if (entryIndex != static_cast<uint32_t>(-1))
    {
        C_00863044_RemoveObjectEntry(self, nullptr, entryIndex);
    }
}

HOOK(0x006FF2E0, C_00863044_UpdateObjectEntry, "auto");
