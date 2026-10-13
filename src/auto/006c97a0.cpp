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

CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(int, __fastcall, C_00860924_BaseCleanup, 0x006C95C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MenuSaveGame_ClearSlotRange, 0x00544340, (int* first, int* last));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static void __fastcall C_00860924_ReleaseSlotsAndReset(C_00860924* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860954);
    C_00860924_BaseCleanup(self, nullptr);

    uint32_t slotBlock = self->field_0008;
    if (slotBlock) {
        MenuSaveGame_ClearSlotRange(reinterpret_cast<int*>(slotBlock),
                                    reinterpret_cast<int*>(self->field_000c));
        if (self->field_0008) {
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));
        }
    }

    self->field_0008 = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860924);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006C97A0, C_00860924_ReleaseSlotsAndReset, "auto");
