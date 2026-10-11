#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_0083b4a0_LoadPairOrReset(C_0083b4a0* self, void* /*edx*/, int a2) {
    int result = *reinterpret_cast<int*>(static_cast<uintptr_t>(a2) + 4);
    if (result == -1) {
        self->field_004c = static_cast<uint32_t>(-1);
        self->field_0050 = static_cast<uint32_t>(-1);
    } else {
        self->field_004c = static_cast<uint32_t>(result);
        result = *reinterpret_cast<int*>(static_cast<uintptr_t>(a2) + 8);
        self->field_0050 = static_cast<uint32_t>(result);
    }
    return result;
}

HOOK(0x004CFA70, C_0083b4a0_LoadPairOrReset, "auto");
