#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwarder to the linked-object notification routine of the C_00833908 base.
// Called with ecx = this and no stack arguments; returns its eax.
CSF_FUNC(int, __fastcall, C_00833908_NotifyLinkedObjects, 0x48E650, (void* self, void* edx));

// Notifies linked objects, then clears bit 0 of the 0x2E4 flags dword.
// Returns the notification result unchanged.
static int __fastcall CPlayerCharacter_NotifyLinkedAndClearBit0(CPlayerCharacter* self, void* /*edx*/)
{
    int result = C_00833908_NotifyLinkedObjects(self, nullptr);
    self->field_02e4 &= ~1u;
    return result;
}

HOOK(0x495300, CPlayerCharacter_NotifyLinkedAndClearBit0, "auto");
