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

// Callee bound by address (original sub_5182B0, __thiscall, ret 0).
CSF_FUNC(int, __fastcall, sub_5182B0, 0x005182B0, (void* self, void* edx));

// Clears any pending poll state of a CNetProfile and resets its flags to the
// default "idle/enabled" values. Returns the enable flag (field_0008).
static char __fastcall CNetProfile_ClearPendingPollAndResetFlags(CNetProfile* self, void* /*edx*/) {
    // Virtual slot 3 (+0x0C): returns a byte; if nonzero, run slot 2 (+0x08).
    bool hasPending = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[3])(self, nullptr);
    if (hasPending) {
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[2])(self, nullptr);
    }

    self->field_1704 = 0;
    self->field_1708 = 1;
    self->field_0008 = 1;

    sub_5182B0(self, nullptr);

    return static_cast<char>(self->field_0008);
}

HOOK(0x518380, CNetProfile_ClearPendingPollAndResetFlags, "auto");
