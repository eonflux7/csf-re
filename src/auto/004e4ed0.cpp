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

CSF_FUNC(int, __fastcall, C_0083e7d8_ClearPendingState, 0x004E4910, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083e160_UpdateAfterReset, 0x004E1320, (void* self, void* edx));

// Resets the object's state counters, notifies the owner object (via its
// vtable slot 22) with a temporary argument record, then runs the two
// follow-up update steps.
static int __fastcall C_0083e7d8_ResetAndNotifyOwner(C_0083e7d8* self, void* /*edx*/)
{
    // Owner object pointer, read before the stores below (as in the original).
    void* owner = reinterpret_cast<void*>(self->field_000c);

    // Temporary 12-byte argument record on the stack:
    //   [0] = vtable of C_0082d448, [1] = -1, [2] = -1
    alignas(8) uint8_t argRecord[12];
    CSF_AT(int, argRecord, 8) = -1;
    CSF_AT(int, argRecord, 4) = -1;
    CSF_AT(void*, argRecord, 0) = CSF_VTBL(0x0082D448);

    self->field_0008 = 0;
    self->field_0024 = 0;

    // Owner->vtbl[88 / 4 = 22](owner, &argRecord)
    ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, owner, 0)[22])(owner, nullptr, argRecord);

    C_0083e7d8_ClearPendingState(self, nullptr);
    return C_0083e160_UpdateAfterReset(self, nullptr);
}

HOOK(0x004E4ED0, C_0083e7d8_ResetAndNotifyOwner, "auto");
