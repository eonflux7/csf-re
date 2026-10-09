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

// Clears the pending-state word at +0xA0, then sends state code 12 through
// the object's virtual slot 0x11C (vtable[71]) and returns its result.
static int __fastcall CharActionFsm_ResetA0AndSendState12(int* self, void* /*edx*/)
{
    CSF_AT(int, self, 0xA0) = 0;
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x11C / 4])(self, nullptr, 12);
}

HOOK(0x4837F0, CharActionFsm_ResetA0AndSendState12, "auto");
