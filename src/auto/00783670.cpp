#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, sub_5112A0, 0x005112A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets every weapon slot record in the range [firstSlot, endSlot).
// Each record is 0x228 bytes. The original works through a cursor (esi) that
// points at record+0x20C; the other fields are addressed relative to that cursor:
//   cursor[0]    : buffer counter (record+0x20C)
//   cursor[-4]   : buffer count   (record+0x208)
//   cursor[-0x14]: buffer pointer / flag byte (record+0x1F8)
//   cursor-0x208 : sub-object passed to sub_5112A0 (record+4)
static int __cdecl WeaponDefs_ResetSlotRange(int firstSlot, int endSlot, int a3, int a4)
{
    (void)a3;
    (void)a4;

    if (firstSlot != endSlot)
    {
        uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(firstSlot));

        do
        {
            uint8_t* cursor = record + 0x20C;

            if (CSF_AT(uint32_t, cursor, 0) >= 0x10u)
            {
                void* buffer = CSF_AT(void*, cursor, -0x14);
                if (buffer != nullptr)
                {
                    Mem_FreeDebug(buffer);
                }
            }

            CSF_AT(uint32_t, cursor, 0) = 15;
            CSF_AT(uint32_t, cursor, -4) = 0;
            CSF_AT(uint8_t, cursor, -0x14) = 0;

            sub_5112A0(static_cast<void*>(cursor - 0x208), nullptr);

            record += 0x228;
        } while (static_cast<int>(reinterpret_cast<uintptr_t>(record)) != endSlot);
    }

    return endSlot;
}

HOOK(0x783670, WeaponDefs_ResetSlotRange, "auto");
