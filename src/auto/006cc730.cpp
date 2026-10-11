#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using ClumpSearchCallback = int (__cdecl*)(uint32_t, int);

CSF_FUNC(bool, __fastcall, GfxScene_VisitClumps, 0x006BCA60,
         (uint32_t* self, void* edx, ClumpSearchCallback callback, int context));
CSF_FUNC(int, __fastcall, C_00860bb8_CreateClumpEntry, 0x006CB8C0,
         (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

static int __fastcall C_00860bb8_FindOrAddClump(
    C_00860bb8* self, void* /*edx*/, int clumpId)
{
    alignas(8) uint8_t searchBytes[0xC];
    CSF_AT(int, searchBytes, 0x8) = -1;

    uint32_t index = 0;
    bool searching = true;
    if (self->field_005c != 0)
    {
        do
        {
            if (!searching)
                return CSF_AT(int, searchBytes, 0x8);

            void* entry =
                reinterpret_cast<void**>(self->field_0054)[index];
            int entryId =
                reinterpret_cast<int (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, entry, 0)[0x20 / 4])(entry, nullptr);

            if (entryId == clumpId)
            {
                CSF_AT(int, searchBytes, 0x8) = static_cast<int>(index);
                searching = false;
            }

            ++index;
        } while (index < self->field_005c);

        if (!searching)
            return CSF_AT(int, searchBytes, 0x8);
    }

    CSF_AT(int, searchBytes, 0x0) = clumpId;
    CSF_AT(int, searchBytes, 0x4) = 0;
    GfxScene_VisitClumps(
        reinterpret_cast<uint32_t*>(self->field_0008),
        nullptr,
        reinterpret_cast<ClumpSearchCallback>(CSF_ADDR_FN(0x006CA930)),
        reinterpret_cast<int>(searchBytes));

    if (CSF_AT(int, searchBytes, 0x4) == 0)
        return CSF_AT(int, searchBytes, 0x8);

    int entry = C_00860bb8_CreateClumpEntry(
        self, nullptr, CSF_AT(int, searchBytes, 0x4));

    if (self->field_005c >= CSF_AT(uint32_t, self, 0x58))
    {
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<uint8_t*>(self) + 0x54),
            nullptr,
            CSF_AT(int, self, 0x60));
    }

    uint32_t insertionIndex = self->field_005c;
    int* destination =
        reinterpret_cast<int*>(self->field_0054) + insertionIndex;
    self->field_005c = insertionIndex + 1;
    *destination = entry;
    return static_cast<int>(self->field_005c - 1);
}

HOOK(0x006CC730, C_00860bb8_FindOrAddClump, "auto");
