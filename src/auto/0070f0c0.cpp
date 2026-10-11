#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Calls virtual slot 0xAC (index 43) with a2, stores the 32-bit result in field_0004,
// and returns whether that result was nonzero.
static bool __fastcall C_00863ac0_CallSlotAcAndStore(C_00863ac0* self, void* /*edx*/, int a2) {
    uint32_t result = ((uint32_t(__fastcall*)(void*, void*, int))self->vtbl[0xac / 4])(self, nullptr, a2);
    self->field_0004 = result;
    return result != 0;
}

HOOK(0x0070F0C0, C_00863ac0_CallSlotAcAndStore, "auto");
