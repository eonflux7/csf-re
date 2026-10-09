#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, FindGameDataRecordIndex, 0x00776890,
         (void* a1, void* a2, int a3));
CSF_FUNC(int, __cdecl, UpdateGameDataRecordLinks, 0x007754B0,
         (int a1, int a2, int a3, int a4));

static S_00778050** __fastcall GameDataDefs_ReplaceCurrentRecord(
    S_00778050* self,
    void* /*edx*/,
    S_00778050** resultSlot,
    S_00778050* arg4,
    S_00778050* arg8)
{
    if (arg4 != arg8)
    {
        std::uint32_t newIndex = static_cast<std::uint32_t>(
            FindGameDataRecordIndex(
                reinterpret_cast<void*>(arg8),
                reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0008)),
                static_cast<int>(reinterpret_cast<std::intptr_t>(arg4))));

        UpdateGameDataRecordLinks(
            static_cast<int>(newIndex),
            static_cast<int>(self->field_0008),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(resultSlot)));

        self->field_0008 = newIndex;
    }

    *resultSlot = arg4;
    return resultSlot;
}

HOOK(0x00778050, GameDataDefs_ReplaceCurrentRecord, "auto");
