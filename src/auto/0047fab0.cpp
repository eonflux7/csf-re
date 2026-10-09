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

// Copies a block of parameters from the source record (a2) into this FSM object
// and returns the last byte copied (the value left in al).
static char __fastcall CharActionFsm_CopyParamsFromSource(void* self, void* /*edx*/, int a2) {
    void* src = reinterpret_cast<void*>(a2);

    CSF_AT(int, self, 0x3C) = CSF_AT(int, src, 0x04);
    CSF_AT(int, self, 0x44) = CSF_AT(int, src, 0x0C);
    CSF_AT(int, self, 0x48) = CSF_AT(int, src, 0x10);
    CSF_AT(int, self, 0x4C) = CSF_AT(int, src, 0x14);
    CSF_AT(int, self, 0x50) = CSF_AT(int, src, 0x18);
    CSF_AT(int, self, 0x54) = CSF_AT(int, src, 0x1C);
    CSF_AT(char, self, 0x5C) = CSF_AT(char, src, 0x20);
    CSF_AT(char, self, 0x5F) = CSF_AT(char, src, 0x21);

    char result = CSF_AT(char, src, 0x22);
    CSF_AT(char, self, 0x60) = result;
    return result;
}

HOOK(0x0047FAB0, CharActionFsm_CopyParamsFromSource, "auto");
